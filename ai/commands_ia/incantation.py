##
## EPITECH PROJECT, 2026
## zappy-mirror
## File description:
## incantation
##

from parsing import logger

def do_incantation(agent, response_server):
    """This function is start the incantation
    Args:
        agent (class): the agent IA
        result_command (str): the message send by the server for the response
    """
    if (response_server == "Elevation underway"):
        logger.info("The Incantation command is in progress")
        print("The elevation of this character is in progress")
        agent.is_incantation = True
    elif (response_server.startswith("Current level:")):
        list_level = response_server.split((':'))
        level = (int)(list_level[1])
        logger.info("The Incantation command was successful (received and completed). Player level ", agent.level, "to", level)
        print("The Incantation command was successful; the character has evolved to the following level: ", level)
        agent.level = level
        agent.is_incantation = False
    elif (response_server == "ko"):
        logger.info("The Incantation command did not result in the elevation of the character")
        print("The Incantation command did not work; there was no noticeable change for the character.") 
        agent.is_incantation = False
    return    