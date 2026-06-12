##
## EPITECH PROJECT, 2026
## Zappy
## File description:
## Server Makefile
##

CC      = clang++
CFLAGS  = -Wall -Wextra -I src/ -I src/Parsing -I src/SFML -I src/Data -I src/Network #-Werror

RESET   =   \033[0m
BLUE    =   \033[38;2;80;160;255m
GREEN   =   \033[38;2;80;220;120m

SRC     = main.cpp \
		  src/Parsing/Parsing_gui.cpp \
		  src/SFML/Sfml.cpp \
		  src/Parsing/CommandHandler.cpp \
		  src/Data/World.cpp \
		  src/SFML/EventHandler.cpp \
		  src/Network/NetworkHandler.cpp

OBJ     = $(SRC:.cpp=.o)

NAME    = ../zappy_gui

SFML_FLAGS  =   -lsfml-graphics -lsfml-window -lsfml-system -lsfml-network

all: $(NAME)

$(NAME): $(OBJ)
	@printf '%b\n' "$(BLUE)[LD]$(RESET) $@"
	@$(CC) $(CFLAGS) $(OBJ) -o $@ $(SFML_FLAGS)

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