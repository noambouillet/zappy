##
## EPITECH PROJECT, 2026
## zappy-mirror
## File description:
## fork
##

from parsing import logger

def do_fork(agent, response_server):
    """This function is to fork a player
    Args:
        agent (class): the agent IA
        response_server (str): the message send by the server for the response
    """
    if (response_server == "ok"):
        logger.info("The Fork command was successful (received and completed).")
        print("The Fork command was successfully executed, accompanied by the production of an egg.")
    else:
        logger.error("The response message from the server is not suitable for this command. response_server != ok (Fork)")
        print("The response message from the server is not suitable for this command. (Fork)")
    return