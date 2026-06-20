#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

void cmdList(Server& s, Client& client, std::string& line)
{
    std::string nick = client.getNickname();
    std::map<std::string, Channel *>& channels = s.getChannels();

    if(!line.empty())
        return ;
    client.sendMsg(RPL_LISTSTART(nick));
    for(std::map<std::string, Channel *>::iterator it = channels.begin(); it != channels.end(); ++it)
    {
        Channel *ch = it->second;
        std::ostringstream numb;
        numb << ch->getClientsArray().size();
        std::string topic;
        topic = ch->getChannelTopic();
        if(topic.empty())
            topic = "NonTopic";
        client.sendMsg(RPL_LIST(nick, ch->getChannelName(), numb.str(), topic));
    }
    client.sendMsg(RPL_ENDLIST(nick));
}
