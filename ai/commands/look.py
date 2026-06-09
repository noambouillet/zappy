##
## EPITECH PROJECT, 2026
## zappy-mirror
## File description:
## look
##

from parsing import logger

def do_look(agent, result_command):
    """This function is to get and change the vision of the agent with info from the server
    Args:
        agent (class): agent IA
        result_command (str): Response from the serv
    """
    old_vision = agent.vision
    agent.vision = result_command
    print("La vision de l'agent à évoluer passant de", old_vision, "to", agent.vision)
    logger.info("The Look command was successful (received and completed).")
    return