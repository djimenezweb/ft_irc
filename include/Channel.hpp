#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "Server.hpp"
# include "Client.hpp"

class Server;
class Client;

class Channel
{
    private:
        std::string         _name;
        std::string         _topic;
        std::string         _modes;
        std::string         _key;
        Server              *_server;
        int                 _limit;
        std::vector<int>    _admins_fd;
        std::vector<int>    _members_fd;
        std::vector<int>    _invited_fd;
    
    public:
        Channel();
        Channel(std::string name, std::string key, int admin_fd, Server *server);
        Channel(const Channel& copy);
        Channel& operator=(const Channel& other);
        ~Channel();

        const std::vector<int> &getClientsArray() const;
        const std::vector<int> &getChannelAdmins() const;
        const std::string &getChannelTopic() const;
        const std::string &getChannelName() const;
        const std::string &getChannelModes() const;
        std::string getChannelModeArgs() const;
        const std::string& getChannelKey() const;
        const int &getLimit() const;
        void setChannelName(const std::string name);
        void setChannelTopic(const std::string topic);
        void setChannelMode(char modechar);
        void unsetChannelMode(char modechar);
        bool isModeEnabled(char modechar) const;
        void setKey(const std::string key);
        void setLimit(int n);
        bool isAdmin(int fd) const;
        bool isInvited(int fd) const;
        bool hasClient(const Client &client) const;
        void addMember(int fd);
        void addAdmin(int fd);
        void removeAdmin(int fd);
        void addInvited(int fd);
        void removeClient(int fd);
        void removeInvit(int fd);
        void sendMembers(std::string msg, int exclude = 0) const;
};

#endif
