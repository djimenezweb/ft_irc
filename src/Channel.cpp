#include "Channel.hpp"
#include "Client.hpp"

Channel::Channel()
{}

Channel::Channel(std::string name, std::string key, int user_fd, Server *server)
{
    _name = name;
    _topic = "";
    _modes = "";
    _key = key;
    if(!key.empty())
        _modes += "k";
    _server = server;
    _limit = 0;
    addAdmin(user_fd);
    addMember(user_fd);
}

Channel::Channel(const Channel& copy) :
    _name(copy._name),
    _topic(copy._topic),
    _modes(copy._modes),
    _key(copy._key),
    _server(copy._server),
    _limit(copy._limit),
    _admins_fd(copy._admins_fd),
    _members_fd(copy._members_fd),
    _invited_fd(copy._invited_fd)
{}

Channel& Channel::operator=(const Channel& other)
{
    if (this != &other)
    {
        _name = other._name;
        _topic = other._topic;
        _modes = other._modes;
        _key = other._key;
        _server = other._server;
        _limit = other._limit;
        _admins_fd = other._admins_fd;
        _members_fd = other._members_fd;
        _invited_fd = other._invited_fd;
    }
    return (*this);
}

Channel::~Channel()
{}

const std::string &Channel::getChannelName() const
{
    return(_name);
}

void Channel::setChannelName(const std::string name)
{
    _name = name;
}

const std::string &Channel::getChannelTopic() const
{
    return(_topic);
}

void Channel::setChannelTopic(const std::string topic)
{
    _topic = topic;
}

const std::string &Channel::getChannelModes() const
{
    return(_modes);
}

std::string Channel::getChannelModeArgs() const
{
    std::string args("");

    for (size_t i = 0; i < _modes.size(); ++i)
    {
        if (!args.empty())
            args.append(1, ' ');
        if (_modes[i] == 'l')
        {
            std::ostringstream oss;
            oss << _limit;
            args.append(oss.str());
        }
        else if (_modes[i] == 'k')
            args.append(_key.size(), '*');
    }
    return (args);
}

void Channel::setChannelMode(char modechar)
{
    if (!isModeEnabled(modechar))
        _modes.append(1, modechar);
}

void Channel::unsetChannelMode(char modechar)
{
    std::size_t pos = _modes.find(modechar);
    if (pos != std::string::npos)
    {
        _modes.erase(pos, 1);
        if (modechar == 'l')
            _limit = 0;
        else if (modechar == 'k')
            _key.clear();
    }
}

bool Channel::isModeEnabled(char modechar) const
{
    std::size_t pos = _modes.find(modechar);
    if (pos != std::string::npos)
        return true;
    return false;
}

const int &Channel::getLimit() const
{
    return _limit;
}

void Channel::setLimit(int n)
{
    _limit = n;
}

const std::vector<int> &Channel::getChannelAdmins() const
{
    return(_admins_fd);
}

const std::vector<int> &Channel::getClientsArray() const
{
    return(_members_fd);
}

const std::string& Channel::getChannelKey() const
{
    return(_key);
}

void Channel::setKey(const std::string key)
{
    _key = key;
}

void Channel::addMember(int fd)
{
    for (size_t i = 0; i < _members_fd.size(); i++)
        if (_members_fd[i] == fd)
            return;
    _members_fd.push_back(fd);
}

void Channel::addAdmin(int fd)
{
    for(size_t i = 0; i < _admins_fd.size(); i++)
        if(_admins_fd[i] == fd)
            return;
    _admins_fd.push_back(fd);
}

void Channel::removeAdmin(int fd)
{
    for (std::vector<int>::iterator it = _admins_fd.begin(); it != _admins_fd.end(); ++it)
    {
        if (*it == fd)
        {
            _admins_fd.erase(it);
            break;
        }
    }
    if (_admins_fd.empty())
    {
        for (size_t i = 0; i < _members_fd.size(); ++i)
        {
            if (_members_fd[i] == fd)
                continue;
            if (_server->findClientbyFd(_members_fd[i]))
            {
                Client &op = _server->getClientbyFd(_members_fd[i]);
                addAdmin(op.getFd());
                sendMembers(CMD_MODE(_name, "+o", op.getNickname()));
                break;
            }
        }
    }
}

void Channel::addInvited(int fd)
{
    for(size_t i = 0; i < _invited_fd.size(); i++)
        if(_invited_fd[i] == fd)
            return;
    _invited_fd.push_back(fd);
}

void Channel::removeClient(int fd)
{
    if(isAdmin(fd))
        removeAdmin(fd);
    for (std::vector<int>::iterator it = _members_fd.begin(); it != _members_fd.end(); ++it)
    {
        if (*it == fd)
        {
            _members_fd.erase(it);
            return;
        }
    }
}

void Channel::removeInvit(int fd)
{
    for (std::vector<int>::iterator it = _invited_fd.begin(); it != _invited_fd.end(); ++it)
    {
        if (*it == fd)
        {
            _invited_fd.erase(it);
            return;
        }
    }    
}

bool Channel::hasClient(const Client &client) const
{
    std::vector<int>::const_iterator it;

    for (it = _members_fd.begin() ; it != _members_fd.end(); ++it)
    {
        if (*it == client.getFd())
            return true;
    }
    return false;
}

bool Channel::isAdmin(int fd) const
{
    for(std::vector<int>::const_iterator it = _admins_fd.begin(); it != _admins_fd.end(); it++)
        if((*it) == fd)
            return true;
    return false;
}

bool Channel::isInvited(int fd) const
{
    for(std::vector<int>::const_iterator it = _invited_fd.begin(); it != _invited_fd.end(); it++)
        if((*it) == fd)
            return true;
    return false;   
}

void Channel::sendMembers(std::string msg, int exclude) const
{
    for (size_t i = 0; i < _members_fd.size(); ++i)
    {
        if (exclude && _members_fd[i] == exclude)
            continue;
        if (_server->findClientbyFd(_members_fd[i]))
        {
            Client &dest = _server->getClientbyFd(_members_fd[i]);
            dest.sendMsg(msg);
        }
    }
}

