##
## EPITECH PROJECT, 2026
## zappy-mirror
## File description:
## incantation
##

from logger import logger

def remove_incantation_command(agent):
    if "Incantation\n" in agent.list_commands:
        agent.list_commands.remove("Incantation\n")

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
        remove_incantation_command(agent)
    elif (response_server.startswith("Current level:")):
        list_level = response_server.split(':')
        level = int(list_level[1])
        logger.info(f"The Incantation command was successful. Player level {agent.level} to {level}")
        print("The Incantation command was successful; the character has done incantation to the following level: ", level)
        agent.level = level
        agent.is_incantation = False
        agent.prepare_incantation = False
        agent.joining_incantation = False
        agent.tab_id_teammate = []
        agent.mailbox.clear()
        agent.teammate_on_tile = 1
        remove_incantation_command(agent)
    elif (response_server == "ko"):
        logger.info("The Incantation command did not result in the elevation of the character")
        print("The Incantation command did not work; there was no noticeable change for the character.")
        agent.is_incantation = False
        agent.prepare_incantation = False
        agent.joining_incantation = False
        agent.teammate_on_tile = 1
        agent.tab_id_teammate = []
        agent.mailbox.clear()
        remove_incantation_command(agent)
    return   