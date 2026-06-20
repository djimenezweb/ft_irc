#include "Server.hpp"
#include "Channel.hpp"
#include "Client.hpp"
#include "replies.hpp"

void cmdWho(Server& s, Client& client, std::string& line)
{
    std::istringstream iss(line);
    std::string chName;

    iss >> chName;
    if (chName.empty())
        return client.sendMsg(ERR_NEEDMOREPARAMS(client.getNickname(), client.getCliCmd()));
    if (!s.findChannelbyName(chName))
        return client.sendMsg(ERR_NOSUCHCHANNEL(client.getNickname(), chName));

    Channel *ch = s.getChannelbyName(chName);
    std::vector<int> members = ch->getClientsArray();
    std::string cliReq = client.getNickname();

    for (std::vector<int>::iterator it = members.begin(); it != members.end(); ++it)
    {
        Client &member = s.getClientbyFd(*it);
        std::string flags = "H";
        if (ch->isAdmin(*it))
            flags += "@";
        client.sendMsg(my_serv_name" 352 " + cliReq + " " + chName + " " + member.getUser() + " " + member.getHostname() + " my_serv_irc " + member.getNickname() + " " + flags + " :0 " + member.getRealname());
    }
    client.sendMsg(RPL_ENDOFWHO(cliReq, chName));
}
