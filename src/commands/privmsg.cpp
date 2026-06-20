#include "Server.hpp"

void privmsgUser(Server &s, Client &client, std::string &target, std::string &text)
{
    // Target not found
    if (!s.findClientbyNick(target))
        return client.sendMsg(ERR_NOSUCHNICK(client.getNickname(), target));

    // Send private message to target client
    Client &dest = *s.getClientbyNickname(target);
    dest.sendMsg(CMD_PRIVMSG(client.getNickname(), client.getUser(), client.getHostname(), target, text));
}

void privmsgChannel(Server &s, Client &client, std::string &channelName, std::string &text)
{
    // Channel doesn't exist
    if (!s.findChannelbyName(channelName))
        return client.sendMsg(ERR_NOTONCHANNEL(client.getNickname(), channelName));

    // Sender doesn't belong to channel
    Channel *ch = s.getChannelbyName(channelName);
    if (!ch->hasClient(client))
        return client.sendMsg(ERR_CANNOTSENDTOCHAN(client.getNickname(), channelName));

    ch->sendMembers(CMD_PRIVMSG(client.getNickname(), client.getUser(), client.getHostname(), channelName, text), client.getFd());
}

void cmdPrivmsg(Server &s, Client &client, std::string &line)
{
    // Parse targets and text
    std::istringstream iss1(line);
    std::string text;
    std::string targets_full;
    std::vector<std::string> targets;

    getline(iss1 >> std::ws, targets_full, ' ');
    getline(iss1 >> std::ws, text);

    // Message is empty
    if (text.empty())
        return client.sendMsg(ERR_NOTEXTTOSEND(client.getNickname()));

    // Iterate through comma-separated targets and send message to each one
    std::istringstream iss2(targets_full);
    for (std::string target; getline(iss2, target, ',');)
    {
        if (target.empty())
            client.sendMsg(ERR_NORECIPIENT(client.getNickname(), client.getCliCmd()));
        else if (target[0] == '#')
            privmsgChannel(s, client, target, text);
        else
            privmsgUser(s, client, target, text);
    }
}
