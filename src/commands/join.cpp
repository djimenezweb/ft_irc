#include "Channel.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "replies.hpp"

#include <sstream>


void replyUser(Server &s, Client& client, std::string name)
{
    std::string topic = s.getChannelbyName(name)->getChannelTopic();
    std::string nick = client.getNickname();

    if(!topic.empty())
        client.sendMsg(RPL_TOPIC(nick, name, topic));
    else
        client.sendMsg(RPL_NOTOPIC(nick, name));
    
    namesCommand(s, *s.getChannelbyName(name), client);
}



void joinMessages(Server &s, Client& client, std::string name)
{
    std::string joinMsg;
    std::vector<int> clients = s.getChannelbyName(name)->getClientsArray();

    joinMsg = ":" + client.getNickname() + "!" + client.getUser() + "@" + client.getHostname()+ " JOIN " +  name;
    s.getChannelbyName(name)-> sendMembers(joinMsg, 0);
    replyUser(s, client, name);
}


bool checkName(std::string name)
{
    if(name[0] != '#' && name[0] != '&')
        return false;
    else if (name.size() == 1)
        return false;
    return(true);
}

std::map<std::string, std::string> getChData(std::string names, std::string keys, Client& client)
{
    std::istringstream streamNames(names);
    std::istringstream streamKeys(keys);
    std::string nick = client.getNickname();
    std::string chName;
    std::string chKeys;
    std::map<std::string, std::string> chData;
    std::string errorMsg;

    while(std::getline(streamNames, chName, ','))
    {
        if(!std::getline(streamKeys, chKeys, ','))
            chKeys = "";
        if(chName[0] == '#' && chName.size() == 1)
        {
            client.sendMsg(ERR_NEEDMOREPARAMS(nick, client.getCliCmd()));
            return std::map<std::string, std::string> ();
        }
        if(checkName(chName) == 0)
        {
            client.sendMsg(ERR_BADCHANMASK(nick, chName));
            return std::map<std::string, std::string> ();
        }
        chData[chName] = chKeys;
    }

    if (std::getline(streamKeys, chKeys, ','))
    {
        client.sendMsg(ERR_BADCHANMASK(nick, chName));
        return std::map<std::string, std::string> ();
    }
    return (chData);
}

bool validKey(Channel* channel, std::string key, int fd)
{
    if(channel->isModeEnabled('k'))
    {
        if(channel->getChannelKey().empty())
            return true;
        else if((channel->getChannelKey() == key) || (channel->isInvited(fd)))
        {
            if(channel->isInvited(fd))
                channel->removeInvit(fd);
            return true;
        }
        else
            return false;
    }
    return(true);
}

bool validCapacity(Channel *channel)
{
    if(channel->isModeEnabled('l') && channel->getLimit() > 0)
    {
        if(channel->getClientsArray().size() < (size_t)channel->getLimit())
            return(true);
        else
            return(false);
    }
    else
        return(true);
}

bool isPrivate(Channel *channel, int fd)
{
    if(channel->isModeEnabled('i'))
    {
        if(channel->isInvited(fd))
            return(true);
        else
            return(false);
    }
    else
        return(true);
}

void cmdJoin(Server &s, Client& client, std::string line)
{
    std::istringstream str(line);
    std::string nick = client.getNickname();
    std::string _chName;
    std::string _chKey;
    std::map <std::string, std::string> _chData;
    std::string errorMsg;

    str >> _chName;
    str >> _chKey;
    _chData = getChData(_chName, _chKey, client);

    for(std::map<std::string, std::string>::iterator it = _chData.begin(); it != _chData.end(); ++it)
    {
        std::string name = it->first;
        std::string key = it->second;
        
        if(s.findChannelbyName(name))
        {
            Channel *ch = s.getChannelbyName(name);
            if (!validKey(ch, key, client.getFd()))
                client.sendMsg(ERR_BADCHANNELKEY(nick, name));
            else if (!validCapacity(ch))
                client.sendMsg(ERR_CHANNELISFULL(nick, name));
            else if (!isPrivate(ch, client.getFd()))
                client.sendMsg(ERR_INVITEONLYCHAN(nick, name));
            else
            {
                ch->addMember(client.getFd());
                client.addChannel(*(ch));
                joinMessages(s, client, name);
            }
        }
        else
        {
            Channel* ch = new Channel (name, key, client.getFd(), &s);
            s.getChannels()[name] = ch;
            client.addChannel(*ch);
            joinMessages(s, client, name);
        }
    }
}
