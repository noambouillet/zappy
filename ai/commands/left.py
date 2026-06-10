##
## EPITECH PROJECT, 2026
## zappy-mirror
## File description:
## left
##

from parsing import logger
from constant import Direction

def do_left(agent, result_command):
    """This function is to change the direction, left
    Args:
        agent (class): the agent IA
        result_command (str): the message from the serv (the response)
    """
    if (result_command == "ok"):
        agent.direction = Direction((agent.direction.value - 1) % 4)
        logger.info("The Left command was successful (direction: " + agent.direction.name + ")")
        print("The officer turned his position to the left, he was looking in the direction :", agent.direction.name)
    else:
        logger.error("The response message from the server is not suitable for this command. result_command != ok (Left)")
        print("The response message from the server is not suitable for this command. (Left)")
    return