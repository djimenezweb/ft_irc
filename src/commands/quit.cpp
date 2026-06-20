#include "Client.hpp"
#include "Channel.hpp"
#include "Server.hpp"
#include "utils.hpp"

void cmdQuit(Server &s, Client &cli, std::string line)
{
    std::istringstream iss(line);
    std::vector<std::string>::iterator it;
    std::vector<std::string> channels = cli.getChannels();
    std::string reason = getMsg(iss);
    std::string quit_msg = CMD_QUIT(cli.getNickname(), cli.getUser(), cli.getHostname(), reason);
    Channel *ch = NULL;

    for (it = channels.begin(); it != channels.end(); ++it)
    {
        ch = s.getChannelbyName(*it);
        if (ch)
            ch->removeClient(cli.getFd());
        ch->sendMembers(quit_msg);
        cli.removeChannel(*ch);
    }
    s.getDisconnectedSockets().push_back(cli.getFd());
    cli.sendMsg(quit_msg);
}
