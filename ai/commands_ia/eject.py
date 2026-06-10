##
## EPITECH PROJECT, 2026
## zappy-mirror
## File description:
## eject
##

from parsing import logger

def do_eject(agent, response_server):
    """This function is to know if an egg or a player was eject from the tile 
    Args:
        agent (class): the agent IA
        response_server (str): the message send by the server for the response
    """
    if (response_server == "ok"):
        logger.info("The Eject command was successful (received and completed).")
        print("The eject command worked correctly, expelling everything that was on the box.")
    elif (response_server == "ko"):
        logger.info("The Eject command did not result in the expulsion of anything from the square.")
        print("The eject command did not work; there was no noticeable change to the tile.")        
    else:
        logger.error("The response message from the server is not suitable for this command, result_command != ok or ko. (Eject)")
        print("The response message from the server is not suitable for this command. (Eject)")
    return