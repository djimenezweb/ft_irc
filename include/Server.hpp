#ifndef SERVER_HPP
# define SERVER_HPP

# include <string>
# include <cstring>
# include <iostream>
# include <sstream>
# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>
# include <poll.h>
# include <unistd.h>
# include <vector>
# include <map>
# include <exception>
# include <signal.h>
# include <iterator>
# include <errno.h>
# include <arpa/inet.h>
# include <algorithm>

# include "Channel.hpp"
# include "Client.hpp"
# include "commands.hpp"
# include "replies.hpp"

class Channel;
class Client;

class Server
{
    typedef std::vector<Channel *>::iterator channel_iterator;

    private:
        const std::string                   _port;
        std::string                         _password;
        std::string                         _hostname;
        int                                 _serv_socket;
        struct addrinfo                     *_addr_lst;
        std::vector<struct pollfd>          _pfd_arr;
        std::vector<int>                    _accepted_sockets;
        std::vector<int>                    _disconnected_sockets;
        std::map<std::string, Channel *>    _channels;
        std::map<int, std::string>          _accepted_ips;
        std::map<int, Client>               _clients;
        std::map<std::string, int>          _clientsByNick;
        void                                accept_socket();
        void                                client_event(int fd);
        void                                add_clients();
        void                                disconnect_sockets();
        void                                broadcastQuit(int fd);

    public:
        Server();
        Server(const Server &other);
        Server &operator=(const Server& other);
        Server(char *port, char *password);
        ~Server();
        void init();
        void run();
        std::map<std::string, Channel *> &getChannels();
        std::map<int, Client> &getClients();
        const std::string &getHostname() const;
        std::map<std::string, int> &getClientsByNick();
        std::vector<int> &getDisconnectedSockets();
        bool findChannelbyName(std::string name);
        Channel *getChannelbyName(std::string name);
        bool findClientbyNick(std::string name);
        bool findClientbyFd(int fd);
        void eraseChannel(Channel* channel);
        Client *getClientbyNickname(std::string name);
        Client &getClientbyFd(int fd);
};

#endif
