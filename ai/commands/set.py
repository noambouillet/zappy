##
## EPITECH PROJECT, 2026
## zappy-mirror
## File description:
## set
##

from parsing import logger

def do_set(agent, result_command):
    """This function is to know if the object is set on a tile
    Args:
        agent (class): the agent IA
        result_command (str): the message send by the server for the response
    """
    if (result_command == "ok"):
        logger.info("The Set command was successful (received and completed).")
        print("The Set command worked correctly, the desired object was able to be placed on the ground.")
    elif (result_command == "ko"):
        logger.info("The Set command did not result in the placement of the object on the ground")
        print("The Set command did not work; there was no noticeable change to the tile.")        
    else:
        logger.error("The response message from the server is not suitable for this command, result_command != ok or ko. (Set)")
        print("The response message from the server is not suitable for this command. (Set)")
    return
    