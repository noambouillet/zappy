##
## EPITECH PROJECT, 2026
## zappy-mirror
## File description:
## forward
##

from logger import logger

def do_forward(agent, response_server):
    """This function is to advance the character
    Args:
        agent (class): the agent IA
        response_server (str): the message send by the server for the response
    """
    if (response_server == "ok"):
        logger.info("The Forward command was successful (direction: " + agent.direction.name + ")")
        print("Our agent managed to advance one space in the direction:", agent.direction.name)
    else:
        logger.error("The response message from the server is not suitable for this command. response_server != ok (Left)")
        print("The response message from the server is not suitable for this command. (Forward)")
    return