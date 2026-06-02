##
## EPITECH PROJECT, 2026
## Zappy
## File description:
## Server Makefile
##

CC      = clang++
CFLAGS  = -Wall -Wextra -Werror -I src

RESET   =   \033[0m
BLUE    =   \033[38;2;80;160;255m
GREEN   =   \033[38;2;80;220;120m

SRC     = main.cpp \
          src/ServerData.cpp
OBJ     = $(SRC:.cpp=.o)

NAME    = zappy_server

all: $(NAME)

zappy_server: $(OBJ)
	@printf '%b\n' "$(BLUE)[LD]$(RESET) $@"
	@$(CC) $(CFLAGS) $(OBJ) -o $@

%.o:            %.cpp
	@printf '%b\n' "$(BLUE)[CC]$(RESET) $< -> $@"
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -f $(OBJ)
	@printf '%b\n' "$(GREEN)[OK]$(RESET) Server cleaning done"

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
