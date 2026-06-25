##
## EPITECH PROJECT, 2026
## zappy-mirror
## File description:
## logger
##

import logging, sys

logger = logging.getLogger("zappy_ai")

def only_info(record):
    return record.levelno == logging.INFO

def setup_logger():
    """setup the logger to write message in zappy_ai.log file and in stdout
    only the info level message are write in the zappy_ai.log file that is dedicate to the lore
    all other message as debug, warning, error, critical (info included) are write in the stdout

    Returns:
        logging: logger
    """
    if logger.handlers:
        return logger
    logger.setLevel(logging.DEBUG)
    logger.propagate = False
    file_handler = logging.FileHandler("zappy_ai.log", encoding="utf-8")
    file_handler.setLevel(logging.DEBUG)
    file_handler.addFilter(only_info)
    file_handler.setFormatter(logging.Formatter("%(asctime)s - %(levelname)s - %(message)s"))
    console_handler = logging.StreamHandler(sys.stdout)
    console_handler.setLevel(logging.DEBUG)
    console_handler.setFormatter(logging.Formatter("%(message)s"))
    logger.addHandler(file_handler)
    logger.addHandler(console_handler)
    return logger