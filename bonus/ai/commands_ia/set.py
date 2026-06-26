##
## EPITECH PROJECT, 2026
## zappy-mirror
## File description:
## set
##

from logger import logger, add_info_streamlit

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
        logger.info(f"{agent.agent_id}: The sorcerer deposited his offering for the ritual {ressource}")
        add_info_streamlit(agent, "set")
    elif (response_server == "ko"):
        agent.prepare_incantation = False
        agent.is_incantation = False
        add_info_streamlit(agent, "set ko")
        logger.info(f"{agent.agent_id}: The offering was refused by the spirits; the offering was not considered proper enough. {ressource}")       
    else:
        logger.error("The response message from the server is not suitable for this command, result_command != ok or ko. (Set)")
    return
    