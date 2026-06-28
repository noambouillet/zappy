##
## EPITECH PROJECT, 2026
## zappy-mirror
## File description:
## dead
##

from logger import logger, sys

def receive_dead(agent, response_server):
    """This function is to say the death of the player
    Args:
        agent (class): Agent
        response_server (str): response from the server
    """
    logger.info(f"{agent.agent_id}: The sorcerer felt his last breath approaching; he returns to rest in peace with the spirits.")
    sys.exit(0)
    