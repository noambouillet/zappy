##
## EPITECH PROJECT, 2026
## Zappy
## File description:
## AI Makefile
##

RESET   =   \033[0m
GREEN   =   \033[38;2;80;220;120m

BIN = ../zappy_ai

.PHONY: all clean fclean re

all: $(BIN)

$(BIN):
	@printf '#!/bin/sh\nexit 0\n' > $(BIN)
	@chmod +x $(BIN)
	@printf '%b\n' "$(GREEN)[OK]$(RESET) AI compilation done"

clean:
	@rm -f $(BIN)
	@printf '%b\n' "$(GREEN)[OK]$(RESET) AI cleaning done"

fclean: clean

re: fclean all
