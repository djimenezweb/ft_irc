NAME        := ircserv
NAME_BONUS  := ircserv_bonus

SRC_DIR     := src
BONUS_DIR   := bonus
OBJ_DIR     := obj
INC_DIR     := include
DEP_DIR     := deps

SRCS        := main.cpp Client.cpp CommandParse.cpp Server.cpp Channel.cpp \
                utils.cpp commands/cap.cpp commands/join.cpp commands/nick.cpp \
                commands/pass.cpp commands/topic.cpp commands/user.cpp \
                commands/privmsg.cpp commands/kick.cpp commands/part.cpp \
                commands/names.cpp commands/who.cpp commands/quit.cpp \
                commands/list.cpp commands/invite.cpp commands/ping.cpp \
                commands/mode.cpp commands/notice.cpp

BONUS_SRCS  := bot_bonus.cpp

SRCS        := $(addprefix $(SRC_DIR)/, $(SRCS))
BONUS_SRCS  := $(addprefix $(BONUS_DIR)/, $(BONUS_SRCS))

OBJS        := $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
DEPS        := $(SRCS:$(SRC_DIR)/%.cpp=$(DEP_DIR)/%.d)
OBJS_BONUS  := $(BONUS_SRCS:$(BONUS_DIR)/%.cpp=$(OBJ_DIR)/bonus/%.o)
DEPS_BONUS  := $(BONUS_SRCS:$(BONUS_DIR)/%.cpp=$(DEP_DIR)/bonus/%.d)

CXX         := c++
CXXFLAGS    := -Wall -Wextra -Werror -std=c++98 -I$(INC_DIR)

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(OBJS) -o $@

bonus: $(NAME_BONUS)

$(NAME_BONUS): $(OBJS_BONUS)
	$(CXX) $(CXXFLAGS) $(OBJS_BONUS) -lcurl -o $@ 

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR) $(DEP_DIR)
	mkdir -p $(dir $@) $(DEP_DIR)/$(dir $*)
	$(CXX) $(CXXFLAGS) -MMD -MF $(DEP_DIR)/$*.d -c $< -o $@

$(OBJ_DIR)/bonus/%.o: $(BONUS_DIR)/%.cpp | $(OBJ_DIR) $(DEP_DIR)
	mkdir -p $(dir $@) $(DEP_DIR)/bonus
	$(CXX) $(CXXFLAGS) -MMD -MF $(DEP_DIR)/bonus/$*.d -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(DEP_DIR):
	mkdir -p $(DEP_DIR)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -rf $(DEP_DIR)
	rm -f $(NAME) $(NAME_BONUS)

re: fclean all

run: all
	./$(NAME) 6667 1234

-include $(DEPS) $(DEPS_BONUS)

.PHONY: all bonus clean fclean re run
