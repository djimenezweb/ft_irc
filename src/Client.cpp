#include "Client.hpp"
#include "definitions.hpp"

Client::Client() :
    _fd(-1),
    _authenticated(false),
    _has_pass(false),
    _registered(false)
{}

Client::Client(int fd, const std::string &hostname) :
    _fd(fd),
    _hostname(hostname),
    _authenticated(false),
    _has_pass(false),
    _registered(false)
{}

Client::Client(const Client& other) :
    _fd(other._fd),
    _nick(other._nick),
    _user(other._user),
    _realname(other._realname),
    _pass(other._pass),
    _hostname(other._hostname),
    _cmd(other._cmd),
    _channels(other._channels),
    _authenticated(other._authenticated),
    _has_pass(other._has_pass),
    _registered(other._registered)
{}

Client& Client::operator=(const Client& other)
{
    if (this != &other)
    {
        this -> _fd = other._fd;
        this -> _nick = other._nick;
        this -> _user = other._user;
        this -> _realname = other._realname;
        this -> _pass = other._pass;
        this -> _hostname = other._hostname;
        this -> _cmd = other._cmd;
        this -> _channels = other._channels;
        this -> _authenticated = other._authenticated;
        this -> _has_pass = other._has_pass;
        this -> _registered = other._registered;
    }
    return (*this);
}

Client::~Client() {}

int Client::getFd() const
{
    return _fd;
}

void Client::setNickname(const std::string& nickname)
{
    this -> _nick = nickname;
}

const std::string &Client::getNickname() const
{
    return _nick;
}

void Client::setUser(const std::string& user)
{
    this -> _user = user;
}

const std::string &Client::getUser() const
{
    return _user;
}

void Client::setRealname(const std::string& realname)
{
    this -> _realname = realname;
}

const std::string &Client::getRealname() const
{
    return _realname;
}

void Client::setAuthenticated(const bool& authenticated)
{
    this -> _authenticated = authenticated;
}

bool Client::getAuthenticated() const
{
    return _authenticated;
}

void Client::setHasPass(const bool& has_pass)
{
    this -> _has_pass = has_pass;
}

bool Client::getHasPass() const
{
    return _has_pass;
}

void Client::setRegistered(const bool& registered)
{
    _registered = registered;
}

bool Client::getRegistered() const
{
    return (_registered);
}

void Client::setHostname(const std::string& hostname)
{
    _hostname = hostname;
}

const std::string &Client::getHostname() const
{
    return _hostname;
}

const std::string &Client::getCliCmd() const
{
    return(_cmd);
}

void Client::setCliCmd(const std::string &cmd)
{
    _cmd = cmd;
}

std::string Client::getNickWithPrefix(const Channel& ch) const
{
    if(ch.isAdmin(_fd))
        return ("@" + _nick);
    return(_nick);
}

void Client::addChannel(const Channel& ch)
{
    for (std::vector<std::string>::iterator it = _channels.begin(); it != _channels.end(); ++it)
    {
        if (*it == ch.getChannelName())
            return;
    }
    _channels.push_back(ch.getChannelName());
}

const std::vector<std::string> &Client::getChannels() const
{
    return (_channels);
}

void Client::removeChannel(const Channel& ch)
{
    for(std::vector<std::string>::iterator it = _channels.begin(); it != _channels.end(); ++it)
    {
        if((*it) == ch.getChannelName())
        {
            _channels.erase(it);
            return ;
        }
    }
}

void Client::sendMsg(std::string msg)
{
    if (msg.length() > 510)
        msg.erase(510);
    std::cout << RED << OUTGOING << RESET << _fd << " " << RED << OUTGOING << RESET << msg << "\n";
    msg += "\r\n";

    ssize_t total = 0;
    ssize_t length = static_cast<ssize_t>(msg.size());
    while (total < length)
    {
        ssize_t n_bytes = send(_fd, msg.c_str() + total, length - total, 0);
        if (n_bytes < 0)
            throw std::runtime_error(strerror(errno));
        total += n_bytes;
    }
}
