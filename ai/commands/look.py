##
## EPITECH PROJECT, 2026
## zappy-mirror
## File description:
## look
##

from parsing import logger

def do_look(agent, result_command):
    print("Function Look is call")
    agent.vision = result_command
    print("Vision de l'agent:", agent.vision)
    logger.info("The look order has been received and completed")
    return