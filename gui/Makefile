##
## EPITECH PROJECT, 2026
## Zappy
## File description:
## Gui Makefile
##

CC      = clang++
CFLAGS  = -Wall -Wextra -I src/ -I src/Parsing -I src/SFML -I src/Data -I src/Network -I src/Encapsulations -I src/Raylib -I src/Raylib/Encapsulations #-Werror

RESET   =   \033[0m
BLUE    =   \033[38;2;80;160;255m
GREEN   =   \033[38;2;80;220;120m

GUI_FLAGS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -lsfml-graphics -lsfml-window -lsfml-system -lsfml-network

GUI_SRC = src/Raylib/RaylibGui.cpp \
		  src/Raylib/Encapsulations/RayWindow.cpp \
		  src/Raylib/Encapsulations/RayCamera.cpp \
		  src/Raylib/Encapsulations/RayTexture.cpp \
		  src/Raylib/Encapsulations/RayModel.cpp \
		  src/SFML/Sfml.cpp \
		  src/SFML/EventHandler.cpp \
		  src/SFML/RenderMap.cpp \
		  src/SFML/HandleTrantorians.cpp \
      src/SFML/UIRender.cpp

SRC     = main.cpp \
		  src/Parsing/Parsing_gui.cpp \
		  src/Parsing/CommandHandler.cpp \
		  src/Data/World.cpp \
		  src/Network/NetworkHandler.cpp \
		  src/Encapsulations/Socket.cpp \
		  src/Encapsulations/Poll.cpp \
		  src/Data/TextureManager.cpp \
		  $(GUI_SRC)

OBJ     = $(SRC:.cpp=.o)

NAME    = ../zappy_gui

all: $(NAME)

$(NAME): $(OBJ)
	@printf '%b\n' "$(BLUE)[LD]$(RESET) $@"
	@$(CC) $(CFLAGS) $(OBJ) -o $@ $(GUI_FLAGS)

%.o:            %.cpp
	@printf '%b\n' "$(BLUE)[CC]$(RESET) $< -> $@"
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -f $(OBJ)
	@printf '%b\n' "$(GREEN)[OK]$(RESET) Gui cleaning done"

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re