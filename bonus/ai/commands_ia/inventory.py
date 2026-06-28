##
## EPITECH PROJECT, 2026
## zappy-mirror
## File description:
## inventory
##

from logger import logger, add_info_streamlit

def do_inventory(agent, response_server):
    """This function is to change the get and change the inventory for the ia with info from the server
    Args:
        agent (class): agent IA
        response_server (str): Response from the server
    """
    if not response_server.startswith("["):
        return
    clean_response = response_server.replace("[", "")
    clean_response = clean_response.replace("]", "")
    clean_response = clean_response.replace(",", "")
    parts = clean_response.split()
    index = 0
    while index < len(parts) - 1:
        ressource = parts[index]
        number_str = parts[index + 1]
        if ressource in agent.inventory:
            try:
                agent.inventory[ressource] = int(number_str)
            except ValueError:
                return
        index += 2
    agent.adapt_behavior()
    logger.info(f"{agent.agent_id}: The wizard consulted his satchel; he now knows what it contains {agent.inventory}")
    add_info_streamlit(agent, "inventory")