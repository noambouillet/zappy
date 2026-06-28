##
## EPITECH PROJECT, 2026
## Zappy
## File description:
## Main Makefile
##

RESET   =   \033[0m
RED     =   \033[38;2;220;70;70m
GREEN   =   \033[38;2;80;220;120m
YELLOW  =   \033[38;2;245;200;80m
BLUE    =   \033[38;2;90;170;255m
PURPLE  =   \033[38;2;190;120;255m
CYAN    =   \033[38;2;90;220;220m

SERVER_DIR = server
GUI_DIR    = gui
AI_DIR     = ai

BIN_SERVER = zappy_server
BIN_GUI    = zappy_gui
BIN_AI     = zappy_ai

all: zappy_server zappy_gui zappy_ai

zappy_server:
	@$(MAKE) --no-print-directory -C $(SERVER_DIR)

zappy_gui:
	@$(MAKE) --no-print-directory -C $(GUI_DIR)

zappy_ai:
	@$(MAKE) --no-print-directory -C $(AI_DIR)

clean:
	@$(MAKE) --no-print-directory -C $(SERVER_DIR) clean
	@$(MAKE) --no-print-directory -C $(GUI_DIR) clean
	@$(MAKE) --no-print-directory -C $(AI_DIR) clean
	@printf '%b\n' "$(GREEN)[OK]$(RESET) Cleaning done"

fclean:
	@$(MAKE) --no-print-directory -C $(SERVER_DIR) fclean
	@$(MAKE) --no-print-directory -C $(GUI_DIR) fclean
	@$(MAKE) --no-print-directory -C $(AI_DIR) fclean
	@find . -type d -name "__pycache__" -prune -exec rm -rf {} +
	@printf '%b\n' "$(GREEN)[OK]$(RESET) Full cleaning done."

re: fclean all

tests_run:
	@$(MAKE) --no-print-directory -C $(SERVER_DIR) tests_run

.PHONY: all zappy_server zappy_gui zappy_ai clean fclean re tests_run
