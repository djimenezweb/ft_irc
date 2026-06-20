#include "bot_bonus.hpp"

Config loadEnv(const std::string& path)
{
    Config cfg;
    std::ifstream file(path.c_str());
    std::string line;

    while (std::getline(file, line))
    {
        if (line.empty() || line[0] == '#')
            continue;
        std::size_t pos = line.find('=');
        if (pos == std::string::npos)
            continue;
        std::string key   = line.substr(0, pos);
        std::string value = line.substr(pos + 1);
        if(key == "GROQ_KEY")   cfg.groq_key   = value;
        if(key == "GROQ_URL")   cfg.groq_url   = value;
        if(key == "GROQ_MODEL") cfg.groq_model = value;
        if(key == "HOST")       cfg.host       = value;
        if(key == "PASSWORD")   cfg.password   = value;
        if(key == "PORT")       cfg.port       = value;
        if(key == "BOT_NICK")   cfg.bot_nick   = value;
    }
    return cfg;
}

size_t writeAnswer(void *contents, size_t size, size_t nmemb, std::string *output)
{
    output->append((char *)contents, size *nmemb);
    return(size * nmemb);
}

std::string extractJason(const std::string &json, const std::string &key)
{
    std::string search = "\"" + key + "\":\"";

    size_t pos = json.find(search);
    if(pos == std::string::npos)
        return("");
    pos += search.size();

    size_t end = json.find("\"", pos);
    if(end == std::string::npos)
        return("");
    return(json.substr(pos, end - pos));
}

std::string extractContent(const std::string &json)
{
    std::string search = "\"content\":\"";
    size_t pos = json.find(search);
    if(pos == std::string::npos)
        return("No reponse");
    
    pos += search.size();
    std::string result;
    while(pos < json.size() && json[pos] != '"')
    {
        if(json[pos] == '\\' && pos + 1 < json.size())
        {
            pos++;
            if(json[pos] == 'n')
                result += ' ';
            else if(json[pos] == '"')
                result += '"';
            else
                result += json[pos];
        }
        else
            result += json[pos];
        pos++;
    }
    return (result);
}

std::string solverAI(std::string& question, Config cfg)
{
    CURL *curl = curl_easy_init();
    if (!curl)
        return ("Error initializing curl");

    std::string response;
    std::string jsonBody = "{\"model\":\"" + std::string(cfg.groq_model) + "\"," + "\"messages\":[{\"role\":\"user\",\"content\":\"" + question + "\"}]}";

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_append(headers, ("Authorization: Bearer " + std::string(cfg.groq_key)).c_str());

    curl_easy_setopt(curl, CURLOPT_URL, cfg.groq_url.c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonBody.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeAnswer);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);

    CURLcode res = curl_easy_perform(curl);
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    if(res != CURLE_OK)
        return("Error: " + std::string(curl_easy_strerror(res)));

    std::cout << "Groq response: " << response << std::endl;
    return (extractContent(response));
}

void sendMsg(int fd, std::string msg)
{
    msg += "\r\n";
    send(fd, msg.c_str(), msg.size(), 0);
}

std::string parseQuestion(const std::string& question)
{
    std::string parsed;

    for(size_t i = 0; i < question.size(); i++)
    {
        if(question[i] == '"')
            parsed += "\\\"";
        else if(question[i] == '\\')
            parsed += "\\\\";
        else if(question[i] == '\n')
            parsed += ' ';
        else if(question[i] == '\r')
            parsed += ' ';
        else
            parsed += question[i];
    }
    return(parsed);
}

std::string handlePrivmsg(std::string sender, std::string target, std::string msg, Config cfg)
{
    std::string responseTarget;

    if(target == cfg.bot_nick)
        responseTarget = sender;
    else
        responseTarget = target;

    while(!msg.empty() && (msg[msg.size() - 1] == '\r' || msg[msg.size() - 1] == '\n' || msg[msg.size() - 1] == ' '))
        msg.erase(msg.size() - 1);

    if(msg.substr(0, 4) == "!ask")
    {
        std::string question = parseQuestion((msg.substr(5) + " Please condense your answer to 400 characters or less if possible."));
        std::string answer = solverAI(question, cfg);
        if(answer.size() > 400)
            answer = answer.substr(0, 400) + "...";
        return("PRIVMSG " + responseTarget + " :" + answer);
    }
    return("");
}

void connectToChannels(const std::string& line, int fd, std::set<std::string>& joinedChannels, bool& listingDone, Config cfg)
{
    std::istringstream iss(line);
    std::string prefix;
    std::string command;

    if (line.empty())
        return;

    if (line[0] == ':')
        iss >> prefix >> command;
    else
        iss >> command;

    if (command == "322")
    {
        std::string nick, channel;
        iss >> nick >> channel;

        if (channel.empty() || channel[0] != '#')
            return;

        if (joinedChannels.find(channel) != joinedChannels.end())
            return;

        std::cout << "Joining channel: " << channel << std::endl;

        sendMsg(fd, "JOIN " + channel);
        usleep(200000);
    }
    else if (command == "323")
    {
        listingDone = true;
        std::cout << "Finished channel listing." << std::endl;
    }

    else if (command == "JOIN")
    {

        std::string sender = prefix.substr(1, prefix.find('!') - 1);

        std::string channel;
        iss >> channel;

        if (channel.empty())
            return;
        if (channel[0] == ':')
            channel = channel.substr(1);
        if (sender == cfg.bot_nick)
        {
            joinedChannels.insert(channel);
            std::cout << "Confirmed join: " << channel << std::endl;
        }
    }
    else if (command == "471" || command == "473" || command == "474" || command == "475")
        std::cout << "JOIN failed: " << line << std::endl;
}

void checkIfAlone(const std::string& line, int fd, std::set<std::string>& joinedChannels)
{
    std::istringstream iss(line);
    std::string prefix, command;

    if (line.empty())
        return;

    if (line[0] == ':')
        iss >> prefix >> command;
    else
        iss >> command;

    if (command == "PART" || command == "KICK" || command == "QUIT")
    {
        std::string channel;
        if (command == "KICK")
        {
            std::string dummy;
            iss >> channel >> dummy;
        }
        else
            iss >> channel;

        if (channel.empty() || channel[0] != '#')
            return;

        if (joinedChannels.find(channel) == joinedChannels.end())
            return;
        sendMsg(fd, "WHO " + channel);
    }
}

void handleWhoReply(const std::string& line, int fd, std::set<std::string>& joinedChannels,
                    std::map<std::string, int>& whoCount)
{
    std::istringstream iss(line);
    std::string prefix, command;

    if (line[0] == ':')
        iss >> prefix >> command;
    else
        iss >> command;

    if (command == "352")
    {
        std::string requester, channel;
        iss >> requester >> channel;
        whoCount[channel]++;
    }

    if (command == "315")
    {
        std::string requester, channel;
        iss >> requester >> channel;

        std::cout << "WHO finished for " << channel << ": " << whoCount[channel] << " users" << std::endl;

        if (whoCount[channel] <= 1)
        {
            std::cout << "Bot is alone in " << channel << ", leaving..." << std::endl;
            sendMsg(fd, "PART " + channel + " :I am alone here, leaving.");
            joinedChannels.erase(channel);
        }
        whoCount[channel] = 0;
    }
}

void checkPrivmsg(std::string &line, int fd, Config cfg)
{
    try
    {
        std::string sender = line.substr(1, line.find('!') - 1);
        std::string afPrivmsg = line.substr(line.find("PRIVMSG") + 8);
        std::string target = afPrivmsg.substr(0, afPrivmsg.find(' '));
        std::string msg = afPrivmsg.substr(afPrivmsg.find(':') + 1);
        if(sender != cfg.bot_nick)
        {
            std::string str = handlePrivmsg(sender, target, msg, cfg);
            if(!str.empty())
                sendMsg(fd, str);
        }
    }
    catch(std::exception &e)
    {
        std::cerr << "Parse error: " << e.what() << std::endl;
    }
}

bool checkInvite(std::string &line, int fd, Config cfg)
{
    try
    {
        std::istringstream iss(line);
        std::string prefix;
        std::string command;
        std::string target;
        std::string channel;
        iss >> prefix >> command >> target >> channel;
        if (!channel.empty() && channel[0] == ':')
            channel = channel.substr(1);
        if (target != cfg.bot_nick || channel.empty() || channel[0] != '#')
            return true;
        sendMsg(fd, "JOIN " + channel);
    }
    catch (std::exception& e)
    {
        std::cerr << "INVITE parse error: " << e.what() << std::endl;
        return true;
    }
    return false;
}

void executeCommand(std::string &pending, int fd, std::map<std::string, int>& whoCount, std::set<std::string>& channels, bool& registered, Config cfg)
{
    bool listing = false;
    std::string whoChannel;
    size_t pos;

    while((pos = pending.find("\r\n")) != std::string::npos)
    {
        std::string line = pending.substr(0, pos);
        pending.erase(0, pos + 2);
        std::cout << line << std::endl;
        if(line.substr(0, 4) == "PING")
            sendMsg(fd, "PONG " + line.substr(5));
        if(!registered && line.find("001") != std::string::npos)
        {
            sendMsg(fd, "JOIN #Omni-bot");
            sendMsg(fd, "LIST");
            registered = true;
        }
        if(registered)
        {
            connectToChannels(line, fd, channels, listing, cfg);
            checkIfAlone(line, fd, channels);
            handleWhoReply(line, fd, channels, whoCount);
        }
        if(line.find("PRIVMSG") != std::string::npos && line[0] == ':')
            checkPrivmsg(line, fd, cfg);
        if (line.find("INVITE") != std::string::npos && line[0] == ':')
        {
            if (checkInvite(line, fd, cfg))
                continue;
        }
    }
}

int main()
{
    int fd;
    struct sockaddr_in addr;

    fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd < 0)
    {
        std::cerr << "Socket error" << std::endl;
        return 1;
    }

    Config cfg = loadEnv(".env");

    std::memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
	addr.sin_port = htons(static_cast<uint16_t>(atoi(cfg.port.c_str())));
    inet_pton(AF_INET, cfg.host.c_str(), &addr.sin_addr);

    if(connect(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        std::cerr << "Connect error" << std::endl;
        close (fd);
        return (1);
    }

    bool registered = false;
    
    sendMsg(fd, "PASS " + std::string(cfg.password));
    sendMsg(fd, "NICK " + std::string(cfg.bot_nick));
    sendMsg(fd, "USER Omni-bot 0 * :Omni-bot");
    sendMsg(fd, "CAP END");
    
    char buffer[2048];
    std::map<std::string, int> whoCount;
    std::set<std::string> channels;
    while(true)
    {
        memset(buffer, 0, sizeof(buffer));

        int bytes = recv(fd, buffer, sizeof(buffer) - 1, 0);
        if(bytes <= 0)
            break ;
    
        std::string pending = std::string(buffer, bytes);
        executeCommand(pending, fd, whoCount, channels, registered, cfg);
    }
    close (fd);
    return (0);
}
