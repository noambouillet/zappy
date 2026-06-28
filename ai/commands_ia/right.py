##
## EPITECH PROJECT, 2026
## zappy-mirror
## File description:
## right
##

from logger import logger
from constant import Direction

def do_right(agent, response_server):
    """This function is to change the direction, right
    Args:
        agent (class): the agent IA
        response_server (str): the message from the serv (the response)
    """
    if (response_server == "ok"):
        agent.direction = Direction((agent.direction.value + 1) % 4)
        logger.info(f"{agent.agent_id}: The sorcerer heard the spirits leading him to the {agent.direction.name}")
    else:
        logger.error("The response message from the server is not suitable for this command. response_server != ok (Left)")
    return