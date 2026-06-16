##
## EPITECH PROJECT, 2026
## Zappy
## File description:
## Server Makefile
##

CC      = clang++
CFLAGS  = -Wall -Wextra -Werror -I src/ -I src/Parsing -I src/Encapsulations/ -I src/GameLoop/ -I src/Commands/

RESET   =   \033[0m
BLUE    =   \033[38;2;80;160;255m
GREEN   =   \033[38;2;80;220;120m

SRC     = main.cpp \
          src/Logger.cpp \
          src/Parsing/ServerData.cpp \
          src/Encapsulations/Poll.cpp \
          src/Encapsulations/Socket.cpp \
          src/GameLoop/Server.cpp \
          src/GameLoop/Client.cpp \
          src/Parsing/InputParser.cpp \
          src/GameLoop/PlayerData.cpp \
          src/GameLoop/Map.cpp \
          src/Commands/AiCommands.cpp \
          src/Commands/GuiCommands.cpp

OBJ     = $(SRC:.cpp=.o)

NAME    = ../zappy_server

all: $(NAME)

$(NAME): $(OBJ)
	@printf '%b\n' "$(BLUE)[LD]$(RESET) $@"
	@$(CC) $(CFLAGS) $(OBJ) -o $@

%.o:            %.cpp
	@printf '%b\n' "$(BLUE)[CC]$(RESET) $< -> $@"
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -f $(OBJ)
	@rm -f $(NAME)
	@printf '%b\n' "$(GREEN)[OK]$(RESET) Server cleaning done"

fclean: clean

re: fclean all

.PHONY: all clean fclean re
