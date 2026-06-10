##
## EPITECH PROJECT, 2026
## zappy-mirror
## File description:
## take
##

from parsing import logger

def do_take(agent, result_command):
    """This function is to know if the object is take from the tile
    Args:
        agent (class): the agent IA
        result_command (str): the message send by the server for the response
    """
    if (result_command == "ok"):
        logger.info("The Take command was successful (received and completed).")
        print("The Take command worked correctly, the object could be taken from the tile.")
    elif (result_command == "ko"):
        logger.info("The Take command did not result in retrieving the object from the tile")
        print("The Take command did not work; there was no noticeable change to the tile.")        
    else:
        logger.error("The response message from the server is not suitable for this command, result_command != ok or ko. (Take)")
        print("The response message from the server is not suitable for this command. (Take)")
    return