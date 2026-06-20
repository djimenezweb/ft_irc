#include "Server.hpp"
#include "Channel.hpp"
#include "Client.hpp"

void appendModesetModechar(std::string &modestring, char modeset, char modechar)
{
    char active = 0;
    size_t pos = modestring.find_last_of("+-");
    if (pos != std::string::npos)
        active = modestring[pos];

    if (modeset != active)
        modestring.append(1, modeset);
    modestring.append(1, modechar);
}

// MUST always have a parameter
void modeTypeB(Server &s, Channel &channel, Client &c, char modechar, char modeset, std::istringstream &args, std::vector<std::string> &result)
{
    std::string modearg;
    args >> modearg;
    if (modearg.empty())
        return;
    if (modechar == 'o')
    {
        if (!s.findClientbyNick(modearg))
            return c.sendMsg(ERR_NOSUCHNICK(c.getNickname(), modearg));
        Client &target = *s.getClientbyNickname(modearg);
        if (!channel.hasClient(target))
            return c.sendMsg(ERR_USERNOTINCHANNEL(c.getNickname(), channel.getChannelName()));
        bool isAdmin = channel.isAdmin(target.getFd());
        if ((modeset == '-' && !isAdmin) || (modeset == '+' && isAdmin))
            return;
        if (modeset == '-')
            channel.removeAdmin(target.getFd());
        else if (modeset == '+')
            channel.addAdmin(target.getFd());
        result.push_back(modearg);
        appendModesetModechar(result[0], modeset, modechar);
    }
}

// MUST have a parameter when being set, and MUST NOT have a parameter when being unset
void modeTypeC(Channel &channel, char modechar, char modeset, std::istringstream &args, std::vector<std::string> &result)
{
    if (modeset == '-')
    {
        if (!channel.isModeEnabled(modechar))
            return;
        channel.unsetChannelMode(modechar);
    }
    else if (modeset == '+')
    {
        std::string modearg;
        args >> modearg;
        if (modearg.empty())
            return;
        if (modechar == 'l')
        {
            int value;
            std::istringstream iss(modearg);
            iss >> value;
            if (iss.fail() || !iss.eof())
                return;
            if (value <= 0 || channel.getLimit() == value)
                return;
            channel.setChannelMode('l');
            channel.setLimit(value);
        }
        else if (modechar == 'k')
        {
            if (channel.getChannelKey() == modearg)
                return;
            channel.setChannelMode('k');
            channel.setKey(modearg);
        }
        result.push_back(modearg);
    }
    appendModesetModechar(result[0], modeset, modechar);
}

// MUST NOT have a parameter
void modeTypeD(Channel &channel, char modechar, char modeset, std::vector<std::string> &result)
{
    if (modeset == '-')
    {
        if (!channel.isModeEnabled(modechar))
            return;
        channel.unsetChannelMode(modechar);
    }
    else if (modeset == '+')
    {
        if (channel.isModeEnabled(modechar))
            return;
        channel.setChannelMode(modechar);
    }
    appendModesetModechar(result[0], modeset, modechar);
}

std::string computeModestring(std::string modestring)
{
    char modeset = '+';
    std::string str("");
    for (size_t i = 0; i < modestring.length(); ++i)
    {
        if (modestring[i] == '+' || modestring[i] == '-')
            modeset = modestring[i];
        else
        {
            size_t pos = str.find(modestring[i]);
            if (pos == std::string::npos)
            {
                str.append(1, modeset);
                str.append(1, modestring[i]);
            }
            else
                str[pos - 1] = modeset;
        }
    }
    return (str);
}

void cmdMode(Server &s, Client &c, std::string &line)
{
    std::istringstream  iss(line);
    std::string         target;
    std::string         modestring;

    iss >> target >> modestring;

    if (target[0] != '#' && !modestring.empty())
        return c.sendMsg(ERR_UMODEUNKNOWNFLAG(c.getNickname()));

    if (!s.findChannelbyName(target))
        return c.sendMsg(ERR_NOSUCHCHANNEL(c.getNickname(), target));

    Channel &channel = *(s.getChannelbyName(target));

    if (modestring.empty())
        return c.sendMsg(RPL_CHANNELMODEIS(c.getNickname(), target, channel.getChannelModes(), channel.getChannelModeArgs()));

    if (!channel.isAdmin(c.getFd()))
        return c.sendMsg(ERR_CHANOPRIVSNEEDED(c.getNickname(), target));

    char modeset = '+';
    std::vector<std::string> result;
    result.push_back("");

    std::string temp = computeModestring(modestring);
    modestring = temp;

    for (size_t i = 0; i < modestring.length(); ++i)
    {
        if (modestring[i] == '+' || modestring[i] == '-')
            modeset = modestring[i];
        else if (modestring[i] == 'o')
            modeTypeB(s, channel, c, modestring[i], modeset, iss, result);
        else if (modestring[i] == 'k' || modestring[i] == 'l')
            modeTypeC(channel, modestring[i], modeset, iss, result);
        else if (modestring[i] == 'i' || modestring[i] == 't')
            modeTypeD(channel, modestring[i], modeset, result);
        else
            c.sendMsg(ERR_UNKNOWNMODE(c.getNickname(), modestring[i]));
    }

    std::string appliedArgs;
    for (size_t i = 1; i < result.size(); ++i)
    {
        appliedArgs.append(result[i]);
        if (i != result.size() - 1)
            appliedArgs.append(" ");
    }
    if (!result[0].empty())
        channel.sendMembers(CMD_MODE(target, result[0], appliedArgs));
}
