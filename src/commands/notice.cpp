#include "Server.hpp"

void noticeUser(Server &s, Client &client, std::string &target, std::string &text)
{
    // Target not found
    if (!s.findClientbyNick(target))
        return;

    // Send private message to target client
    Client &dest = *s.getClientbyNickname(target);
    dest.sendMsg(CMD_NOTICE(client.getNickname(), client.getUser(), client.getHostname(), target, text));
}

void noticeChannel(Server &s, Client &client, std::string &channelName, std::string &text)
{
    // Channel doesn't exist
    if (!s.findChannelbyName(channelName))
        return;

    // Sender doesn't belong to channel
    Channel *ch = s.getChannelbyName(channelName);
    if (!ch->hasClient(client))
        return;

    ch->sendMembers(CMD_NOTICE(client.getNickname(), client.getUser(), client.getHostname(), channelName, text), client.getFd());
}

void cmdNotice(Server &s, Client &client, std::string &line)
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
        return;

    // Iterate through comma-separated targets and send message to each one
    std::istringstream iss2(targets_full);
    for (std::string target; getline(iss2, target, ',');)
    {
        if (target.empty())
            continue;
        else if (target[0] == '#')
            noticeChannel(s, client, target, text);
        else
            noticeUser(s, client, target, text);
    }
}
