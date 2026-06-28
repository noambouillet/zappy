##
## EPITECH PROJECT, 2026
## zappy-mirror
## File description:
## take
##

from logger import logger, add_info_streamlit

def do_take(agent, response_server, command):
    """This function is to know if the object is take from the tile
    Args:
        agent (class): the agent IA
        response_server (str): the message send by the server for the response
        command: the command send by the ia
    """
    split_command = command.split()
    ressource = split_command[1].strip()
    if (response_server == "ok"):
        key = agent.inventory.get(ressource)
        if (key is not None):
            agent.inventory[ressource] += 1
        if (agent.vision != [[]] and ressource in agent.vision[0]):
            agent.vision[0].remove(ressource)
        logger.info(f"{agent.agent_id}: The mage managed to seize the coveted component {ressource}, and his grimoire grew richer.")
        add_info_streamlit(agent, "take")
    elif (response_server == "ko"):
        if (agent.vision != [[]] and ressource in agent.vision[0]):
            agent.vision[0].remove(ressource)
        add_info_streamlit(agent, "take ko")
        logger.info(f"{agent.agent_id}: The ground yielded no treasure this time {ressource}")      
    else:
        logger.error("The response message from the server is not suitable for this command, result_command != ok or ko. (Take)")
    return