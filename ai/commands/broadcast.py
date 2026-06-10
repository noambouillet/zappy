##
## EPITECH PROJECT, 2026
## zappy-mirror
## File description:
## broadcast
##

from parsing import logger

def do_broadcast(agent, result_command):
    """This function is to confirm the Ok for broacast the message from server
    Args:
        agent (class): agent IA
        result_command (str): Response from the server
    """
    if (result_command == "ok"):
        logger.info("The message was successfully sent to the other AI agents.")
        logger.info("The Broadcast command was successful (received and completed).")
    else:
        logger.error("The response message from the server is not suitable for this command, result_command != ok. (Broadcast)")
        print("The response message from the server is not suitable for this command. (Broadcast)")
    return