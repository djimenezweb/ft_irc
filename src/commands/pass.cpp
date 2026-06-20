#include "Client.hpp"

void cmdPass(std::istringstream &iss, Client &client, std::string pass)
{
    std::string password;
    std::string nick = client.getNickname().empty() ? "*" : client.getNickname();

    iss >> password;
    if (client.getRegistered() && client.getAuthenticated())
        return(client.sendMsg(ERR_ALREADYREGISTERED(nick)));
    
    if (password.empty())
        return (client.sendMsg(ERR_NEEDMOREPARAMS(nick, "PASS")));

    if (client.getNickname().empty() && client.getUser().empty())
    {
        if (password == pass && !pass.empty())
            client.setHasPass(true);
        else
        {
            client.sendMsg(ERR_PASSWDMISMATCH(nick));
            client.setHasPass(false);
        }
    }
}
