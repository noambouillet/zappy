##
## EPITECH PROJECT, 2026
## zappy-mirror
## File description:
## broadcast
##

from logger import logger

def do_broadcast(agent, response_server):
    """This function is to confirm the Ok for broacast the message from server
    Args:
        agent (class): agent IA
        response_server (str): Response from the server
    """
    if (response_server == "ok"):
        logger.info(f"{agent.agent_id}: The wizard used his telekinesis spell to warn everyone.")
    else:
        logger.error("The response message from the server is not suitable for this command, result_command != ok. (Broadcast)")
    return