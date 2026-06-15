##
## EPITECH PROJECT, 2026
## zappy-mirror
## File description:
## set
##

from parsing import logger

def do_set(agent, response_server, command):
    """This function is to know if the object is set on a tile
    Args:
        agent (class): the agent IA
        response_server (str): the message send by the server for the response
        command: the command send by the ia
    """
    if (response_server == "ok"):
        split_command = command.split()
        ressource = split_command[1].strip()
        key = agent.inventory.get(ressource)
        if (key is not None):
            agent.inventory[ressource] -= 1
        logger.info("The Set command was successful (received and completed).")
        print(f"The Set command worked correctly, the desired object was able to be placed on the ground. ({ressource})")
    elif (response_server == "ko"):
        agent.prepare_incantation = False
        agent.is_incantation = False
        logger.info("The Set command did not result in the placement of the object on the ground")
        print("The Set command did not work; there was no noticeable change to the tile.")        
    else:
        logger.error("The response message from the server is not suitable for this command, result_command != ok or ko. (Set)")
        print("The response message from the server is not suitable for this command. (Set)")
    return
    