##
## EPITECH PROJECT, 2026
## zappy-mirror
## File description:
## dead
##

from logger import logger, sys

def receive_dead():
    """This function is to say the death of the player"""
    logger.info("The connection has just ended because this client has just lost")
    print("The player is now dead")
    sys.exit(0)
    