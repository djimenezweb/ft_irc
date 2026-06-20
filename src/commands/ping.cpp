#include "Server.hpp"

void cmdPing(Client &client, std::string &line)
{
    if (line.empty())
        return client.sendMsg(ERR_NEEDMOREPARAMS(client.getNickname(), client.getCliCmd()));

    client.sendMsg(CMD_PONG(line));
}
