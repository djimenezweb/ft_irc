#include "../include/Channel.hpp"
#include "../include/Server.hpp"
#include "../include/Client.hpp"

void cmdTopic(Server &s, Client &client, std::string line)
{
    std::istringstream iss(line);
    std::string channelName;
    iss >> channelName;
    std::string resto;
    std::string topic;
    std::getline(iss, resto);
    if(!s.findChannelbyName(channelName))
        return(client.sendMsg(ERR_NOSUCHCHANNEL(client.getNickname(), channelName)));
    Channel &channel = *s.getChannelbyName(channelName);
    if (!channel.hasClient(client))
        return (client.sendMsg(ERR_NOTONCHANNEL(client.getNickname(), channelName)));

    size_t index = 0;
    while (index < resto.length() && isspace(resto[index]))
        index++;
    if (index < resto.length() && resto[index] == ':')
        index++;
    topic = resto.substr(index);

    if (topic.empty() && resto.find(':') == std::string::npos)
    {
        if (channel.getChannelTopic().empty())
            client.sendMsg(RPL_NOTOPIC(client.getNickname(), channelName));
        else
            client.sendMsg(RPL_TOPIC(client.getNickname(), channelName, channel.getChannelTopic()));
    }
    else
    {
        if((channel.isModeEnabled('t') && channel.isAdmin(client.getFd())) || !channel.isModeEnabled('t'))
        {
            channel.setChannelTopic(topic);
            std::string broadcast = ":" + client.getNickname() + "!" + client.getUser() + "@localhost" + " TOPIC " + channelName + " :" + channel.getChannelTopic();
            channel.sendMembers(broadcast);
        }
        else
            client.sendMsg(ERR_CHANOPRIVSNEEDED(client.getNickname(), channelName));
    }
}
