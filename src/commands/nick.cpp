#include "Client.hpp"
#include "utils.hpp"

void cmdNick(Server& s, Client &client, std::istringstream &iss)
{
    std::string nickname;
    std::vector<std::string> chanels = client.getChannels();
    iss >> nickname;

    std::string oldNickname = client.getNickname().empty() ? "*" : client.getNickname();
    std::string nick_msg = CMD_NICK(oldNickname, client.getUser(), client.getHostname(), nickname);

    if (nickname.empty())
        return(client.sendMsg(ERR_NONICKNAMEGIVEN(oldNickname)));

    if (!isValidChar(nickname, " ,:*?!@#&"))
        return(client.sendMsg(ERR_ERRONEUSNICKNAME(oldNickname, nickname)));

    for (std::map<std::string, int>::iterator it = s.getClientsByNick().begin(); it != s.getClientsByNick().end(); ++it)
    {
        std::string tester = it->first;
        std::string fakenick = nickname;
        ft_tolower(tester);
        ft_tolower(fakenick);
        if(tester == fakenick)
            return (client.sendMsg(ERR_NICKNAMEINUSE(oldNickname, nickname)));
    }

    if (!client.getNickname().empty())
        s.getClientsByNick().erase(client.getNickname());

    client.setNickname(nickname);
    s.getClientsByNick()[nickname] = client.getFd();

    client.sendMsg(CMD_NICK(oldNickname, client.getUser(), client.getHostname(), nickname));

    std::vector<std::string>::iterator it;
    for (it = chanels.begin(); it != chanels.end(); ++it)
        s.getChannels()[*it] -> sendMembers(nick_msg);
}
