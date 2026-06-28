##
## EPITECH PROJECT, 2026
## zappy-mirror
## File description:
## Makefile
##

RESET = \033[0m
GREEN = \033[38;2;80;220;120m

NAME_MAIN = zappy_ai_main
NAME_BINARY = zappy_ai
SOURCE = $(NAME_MAIN)
TARGET = ../$(NAME_BINARY)

.PHONY: all re clean fclean

all: $(TARGET)

$(TARGET): $(SOURCE)
	@printf '%s\n' '#!/bin/sh' > $(TARGET)
	@printf '%s\n' 'exec python3 "$$(dirname "$$0")/ai/zappy_ai_main" "$$@"' >> $(TARGET)
	@chmod +x $(TARGET)
	@printf '%b\n' "$(GREEN)[OK]$(RESET) AI compilation done."

clean:
	@find . -type d -name "__pycache__" -prune -exec rm -rf {} +
	@find . -type f \( -name "*.txt" -o -name "*.log" \) -delete
	@printf '%b\n' "$(GREEN)[OK]$(RESET) AI cleaning done."

fclean: clean
	@rm -f $(TARGET)

re: fclean all