##
## EPITECH PROJECT, 2026
## zappy-mirror
## File description:
## take
##

from parsing import logger

def do_take(agent, response_server, command):
    """This function is to know if the object is take from the tile
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
            agent.inventory[ressource] += 1
        logger.info("The Take command was successful (received and completed).")
        print(f"The Take command worked correctly, the object could be taken from the tile. ({ressource}) 🗿​​")
    elif (response_server == "ko"):
        logger.info("The Take command did not result in retrieving the object from the tile")
        print("The Take command did not work; there was no noticeable change to the tile.")        
    else:
        logger.error("The response message from the server is not suitable for this command, result_command != ok or ko. (Take)")
        print("The response message from the server is not suitable for this command. (Take)")
    return