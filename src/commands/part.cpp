#include "Server.hpp"
#include "Client.hpp"
#include "replies.hpp"


void promoteAdmin(Server& s, Channel* ch)
{
    std::vector<int> admins = ch->getChannelAdmins();
    std::vector<int> members = ch->getClientsArray();

    if (admins.empty() && !members.empty())
    {
        int newAdminFd = members[0];
        ch->addAdmin(newAdminFd);

        std::string newAdminNick = s.getClients()[newAdminFd].getNickname();
        ch->sendMembers(CMD_MODE(ch->getChannelName(), "+o", newAdminNick));
    }
}


void cmdPart(Server &s, Client& client, std::string line)
{
    std::istringstream iss1(line);
    std::string text;
    std::string targets_full;
    std::vector<std::string> channels;

    getline(iss1 >> std::ws, targets_full, ' ');
    getline(iss1 >> std::ws, text);

    std::istringstream iss2(targets_full);
    for (std::string name; getline(iss2, name, ',');)
    {
        if (name.empty())
            client.sendMsg(ERR_NEEDMOREPARAMS(client.getNickname(), client.getCliCmd()));
        if (!s.findChannelbyName(name))
        {
            client.sendMsg(ERR_NOSUCHCHANNEL(client.getNickname(), name));
            continue ;
        }
        
        Channel* ch = s.getChannelbyName(name);

        if (!ch->hasClient(client))
        {
            client.sendMsg(ERR_NOTONCHANNEL(client.getNickname(), name));
            continue;
        }
        std::string msg = ":" + client.getNickname() + "!" + client.getUser() + "@" + client.getHostname()
                        + " PART " + name + " " + text;

        std::vector<int> clients = ch->getClientsArray();
        ch->sendMembers(msg);
        ch->removeClient(client.getFd());
        client.removeChannel(*ch);
        promoteAdmin(s, ch);
        s.eraseChannel(ch);
    }
}
