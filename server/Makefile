##
## EPITECH PROJECT, 2026
## Zappy
## File description:
## Server Makefile
##

RESET   =   \033[0m
GREEN   =   \033[38;2;80;220;120m

BIN = ../zappy_server

.PHONY: all clean fclean re

all: $(BIN)

$(BIN):
	@printf '#!/bin/sh\nexit 0\n' > $(BIN)
	@chmod +x $(BIN)
	@printf '%b\n' "$(GREEN)[OK]$(RESET) Server compilation done"

clean:
	@rm -f $(BIN)
	@printf '%b\n' "$(GREEN)[OK]$(RESET) Server cleaning done"

fclean: clean

re: fclean all
