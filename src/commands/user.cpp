#include "Client.hpp"
#include "utils.hpp"

void cmdUser(std::istringstream &iss, Client& client)
{
    std::string username;
    std::string mode;
    std::string desuso;
    std::string realname;

    iss >> username >> mode >> desuso;
    std::string resto;
    std::getline(iss, resto);
    
    if (client.getRegistered() && client.getAuthenticated())
        return(client.sendMsg(ERR_ALREADYREGISTERED(client.getNickname())));
    if (!isValidChar(username, " :@"))
        return(client.sendMsg("USER :Not enough parameters"));

    if (!resto.empty())
    {
        size_t colon_pos = 0;
        if (!resto.empty() && resto[0] == ' ')
            resto.erase(0, 1);
        if (!resto.empty() && resto[0] == ':')
            resto.erase(0, 1);
        while (!resto.empty() && resto[0] == ' ')
            resto.erase(0, 1);
        if (colon_pos  != std::string::npos)
            realname = resto.substr(colon_pos);
        else
            realname = resto;
    }
    
    if (username.empty() || realname.empty())
        return (client.sendMsg(ERR_NEEDMOREPARAMS(client.getNickname(), "USER")));
    
    client.setUser(username);
    client.setRealname(realname);
}
