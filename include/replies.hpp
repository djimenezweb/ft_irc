#ifndef REPLIES_HPP
# define REPLIES_HPP

# define my_serv_name ":my_serv_irc"

// Standard replies

# define RPL_WELCOME(client)                           (my_serv_name" 001 " + (client) + " :Welcome to the IRC Network, " + (client))
# define RPL_YOURHOST(client)                          (my_serv_name" 002 " + (client) + " :Your host is my_serv_irc, running version 1.0")
# define RPL_CREATED(client)                           (my_serv_name" 003 " + (client) + " :This server was created May 2026")
# define RPL_MYINFO(client)                            (my_serv_name" 004 " + (client) + " my_serv_irc 1.0 o itkl kl")
# define RPL_ISUPPORT(client)                          (my_serv_name" 005 " + (client) + " CASEMAPPING=ascii CHANMODES=,o,kl,it CHANTYPES=# :are supported by this server")
# define RPL_ENDOFWHO(client, mask)                    (my_serv_name" 315 " + (client) + " " + (mask) + " :End of WHO list")
# define RPL_LISTSTART(client)                         (my_serv_name" 321 " + (client) + " Channel :Users Name")
# define RPL_LIST(client, channel, numb, topic)        (my_serv_name" 322 " + (client) + " " + (channel) + " " + (numb) + " :" + (topic))
# define RPL_ENDLIST(client)                           (my_serv_name" 323 " + (client) + " :End of /LIST")
# define RPL_CHANNELMODEIS(client, chan, modstr, args) (my_serv_name" 324 " + (client) + " " + (chan) + " " + (modstr) + " " + (args))
# define RPL_NOTOPIC(nick, alias)                      (my_serv_name" 331 " + (nick) + " " + (alias) + " :No topic is set")
# define RPL_TOPIC(nick, alias, topic)                 (my_serv_name" 332 " + (nick) + " " + (alias) + " :" + (topic))
# define RPL_INVITING(client, nick, channel)           (my_serv_name" 341 " + (client) + " " + (nick) + " " + (channel))
# define RPL_NAMREPLY(nick, alias)                     (my_serv_name" 353 " + (nick) + " = " + (alias) + " :")
# define RPL_ENDOFNAMES(nick, alias)                   (my_serv_name" 366 " + (nick) + " " + (alias) + " :End of /NAMES list")
# define RPL_ENDOFMOTD(client)                         (my_serv_name" 376 " + (client) + " :End of /MOTD command.")

// Standard errors

# define ERR_NOSUCHNICK(clientNick, nick)              (my_serv_name" 401 " + (clientNick) + " " + (nick) + " :No such nick/channel")
# define ERR_NOSUCHCHANNEL(nick, chname)               (my_serv_name" 403 " + (nick) + " " + (chname) + " :No such channel")
# define ERR_CANNOTSENDTOCHAN(nick, channel)           (my_serv_name" 404 " + (nick) + " " + (channel) + " :Cannot send to channel")
# define ERR_NORECIPIENT(client, command)              (my_serv_name" 411 " + (client) + " :No recipient given (" + (command) + ")")
# define ERR_NOTEXTTOSEND(client)                      (my_serv_name" 412 " + (client) + " :No text to send")
# define ERR_UNKNOWNCOMMAND(client, command)           (my_serv_name" 421 " + (client) + " " + (command) + " :Unknown command")
# define ERR_NONICKNAMEGIVEN(client)                   (my_serv_name" 431 " + (client) + " :No nickname given")
# define ERR_ERRONEUSNICKNAME(client, nick)            (my_serv_name" 432 " + (client) + " " + (nick) + " :Erroneus nickname")
# define ERR_NICKNAMEINUSE(client, nick)               (my_serv_name" 433 " + (client) + " " + (nick) + " :Nickname is already in use")
# define ERR_USERNOTINCHANNEL(nick, chname)            (my_serv_name" 441 " + (nick) + " " + (chname) + " :They aren't on that channel")
# define ERR_NOTONCHANNEL(nick, chname)                (my_serv_name" 442 " + (nick) + " " + (chname) + " :You're not on that channel")
# define ERR_USERONCHANNEL(clientNick, nick, channel)  (my_serv_name" 443 " + (clientNick) + " " + (nick) + " " + (channel) + " :is already on channel")
# define ERR_NEEDMOREPARAMS(nick, cmd)                 (my_serv_name" 461 " + (nick) + " " + (cmd) + " :Not enough parameters")
# define ERR_ALREADYREGISTERED(client)                 (my_serv_name" 462 " + (client) + " :You may not reregister")
# define ERR_PASSWDMISMATCH(client)                    (my_serv_name" 464 " + (client) + " :Password incorrect")
# define ERR_CHANNELISFULL(nick, channel)              (my_serv_name" 471 " + (nick) + " " + (channel) + " :Cannot join channel (+l)")
# define ERR_UNKNOWNMODE(client, modechar)             (my_serv_name" 472 " + (client) + " " + (modechar) + " :is unknown mode char to me")
# define ERR_INVITEONLYCHAN(nick, channel)             (my_serv_name" 473 " + (nick) + " " + (channel) + " : Cannot join channel (+i)")
# define ERR_BADCHANNELKEY(nick, chName)               (my_serv_name" 475 " + (nick) + " " + (chName) + " :Cannot join channel")
# define ERR_BADCHANMASK(nick, chName)                 (my_serv_name" 476 " + (nick) + " " + (chName) + " :Bad Channel Mask")
# define ERR_CHANOPRIVSNEEDED(nick, chname)            (my_serv_name" 482 " + (nick) + " " + (chname) + " :You're not channel operator")
# define ERR_UMODEUNKNOWNFLAG(client)                  (my_serv_name" 501 " + (client) + " :Unkwnown MODE flag")

// Custom messages

# define CMD_MODE(channel, modestring, args)             (my_serv_name" MODE " + (channel) + " " + (modestring) + " " + (args))
# define CMD_PONG(token)                                 (my_serv_name" PONG my_serv_irc " + (token))
# define CMD_QUIT(source, client, host, reason)          (":" + (source) + "!" + (client) + "@" + (host) + " QUIT " + (reason))
# define CMD_NICK(source, client, host, newnick)         (":" + (source) + "!" + (client) + "@" + (host) + " NICK " + (newnick))
# define CMD_PRIVMSG(client, user, host, target, text)   (":" + (client) + "!" + (user) + "@" + (host) + " PRIVMSG " + (target) + " " + (text))
# define CMD_INVITE(source, client, host, nick, channel) (":" + (source) + "!" + (client) + "@" + (host) + " INVITE " + (nick) + " :" + (channel))
# define CMD_NOTICE(client, user, host, target, text)    (":" + (client) + "!" + (user) + "@" + (host) + " NOTICE " + (target) + " " + (text))

#endif
