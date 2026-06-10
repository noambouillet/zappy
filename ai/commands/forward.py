##
## EPITECH PROJECT, 2026
## zappy-mirror
## File description:
## forward
##

from parsing import logger

def do_forward(agent, result_command):
    """This function is to advance the character
    Args:
        agent (class): the agent IA
        result_command (str): the message send by the server for the response
    """
    if (result_command == "ok"):
        logger.info("The Forward command was successful (direction: " + agent.direction.name + ")")
        print("Our agent managed to advance one space in the direction:", agent.direction.name)
    else:
        logger.error("The response message from the server is not suitable for this command. result_command != ok (Left)")
        print("The response message from the server is not suitable for this command. (Forward)")
    return