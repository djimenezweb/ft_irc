#include "Server.hpp"

void cmdInvite(Server &s, Client &client, std::string &line)
{
    // Parse nickname and channel
    std::istringstream iss(line);
    std::string nickname;
    std::string channelName;

    iss >> nickname >> channelName;

    // Missing parameters
    if (nickname.empty() || channelName.empty())
        return client.sendMsg(ERR_NEEDMOREPARAMS(client.getNickname(), client.getCliCmd()));

    // Nickname doesn't exist
    if (!s.findClientbyNick(nickname))
        return client.sendMsg(ERR_NOSUCHNICK(client.getNickname(), nickname));

    // Channel doesn't exist
    if (!s.getChannelbyName(channelName))
        return client.sendMsg(ERR_NOSUCHCHANNEL(client.getNickname(), channelName));

    // Sender doesn't belong to channel
    Channel *ch = s.getChannelbyName(channelName);
    if (!ch->hasClient(client))
        return client.sendMsg(ERR_CANNOTSENDTOCHAN(client.getNickname(), channelName));

    // Channel has invite-only mode set and user is not a channel operator
    if (ch->isModeEnabled('i') && !ch->isAdmin(client.getFd()))
        return client.sendMsg(ERR_CHANOPRIVSNEEDED(client.getNickname(), channelName));

    // Target is already on the channel
    Client &target = *(s.getClientbyNickname(nickname));
    if (ch->hasClient(target))
        return client.sendMsg(ERR_USERONCHANNEL(client.getNickname(), nickname, channelName));

    ch->addInvited(target.getFd());
    client.sendMsg(RPL_INVITING(client.getNickname(), target.getNickname(), channelName));
    target.sendMsg(CMD_INVITE(client.getNickname(), client.getUser(), client.getHostname(), target.getNickname(), channelName));
}
