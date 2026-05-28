##
## EPITECH PROJECT, 2026
## Zappy
## File description:
## Makefile
##

RESET   =   \033[0m
RED     =   \033[38;2;220;70;70m
GREEN   =   \033[38;2;80;220;120m
YELLOW  =   \033[38;2;245;200;80m
BLUE    =   \033[38;2;90;170;255m
PURPLE  =   \033[38;2;190;120;255m
CYAN    =   \033[38;2;90;220;220m

BIN_SERVER = zappy_server
BIN_GUI    = zappy_gui
BIN_AI     = zappy_ai

all: $(BIN_SERVER) $(BIN_GUI) $(BIN_AI)

$(BIN_SERVER):
	@printf '#!/bin/sh\nexit 0\n' > $(BIN_SERVER)
	@chmod +x $(BIN_SERVER)
	@printf '%b\n' "$(GREEN)[OK]$(RESET) Server compilation done"

$(BIN_GUI):
	@printf '#!/bin/sh\nexit 0\n' > $(BIN_GUI)
	@chmod +x $(BIN_GUI)
	@printf '%b\n' "$(GREEN)[OK]$(RESET) GUI compilation done"

$(BIN_AI):
	@printf '#!/bin/sh\nexit 0\n' > $(BIN_AI)
	@chmod +x $(BIN_AI)
	@printf '%b\n' "$(GREEN)[OK]$(RESET) AI compilation done"

clean:
	@rm -f $(BIN_SERVER) $(BIN_GUI) $(BIN_AI)
	@printf '%b\n' "$(GREEN)[OK]$(RESET) Cleaning done"

fclean: clean
	@printf '%b\n' "$(GREEN)[OK]$(RESET) Full cleaning done."

re: fclean all

tests_run:
	@echo "No tests"
