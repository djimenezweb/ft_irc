#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "Channel.hpp"
# include "Server.hpp"

class Channel;

class Client
{
    private:
        int                         _fd;
        std::string                 _nick;
        std::string                 _user;
        std::string                 _realname;
        std::string                 _pass;
        std::string                 _hostname;
        std::string                 _cmd;
        std::vector<std::string>    _channels;
        bool                        _authenticated;
        bool                        _has_pass;
        bool                        _registered;

    public:
        Client();
        Client(int fd, const std::string &hostname);
        Client(const Client& other);
        Client& operator=(const Client& other);
        ~Client();

        std::string buffer;
        void sendMsg(std::string msg);
        void setNickname(const std::string& nick);
        void setUser(const std::string& user);
        void setRealname(const std::string& realname);
        void setHasPass(const bool& has_pass);
        void setAuthenticated(const bool& autheticated);
        void setRegistered(const bool& registered);
        void setHostname(const std::string& hostname);
        void setCliCmd(const std::string &cmd);
        void addChannel(const Channel& ch);
        void removeChannel(const Channel& ch);
        const std::string &getNickname() const;
        const std::string &getUser() const;
        const std::string &getRealname() const;
        const std::string &getHostname() const;
        const std::string &getCliCmd() const;
        std::string getNickWithPrefix(const Channel& ch) const;
        const std::vector<std::string> &getChannels() const;
        bool getAuthenticated() const;
        bool getHasPass() const;
        bool getRegistered() const;
        int getFd() const;
};

#endif
