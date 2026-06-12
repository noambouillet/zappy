##
## EPITECH PROJECT, 2026
## zappy-mirror
## File description:
## inventory
##

from parsing import logger

def do_inventory(agent, response_server):
    """This function is to change the get and change the inventory for the ia with info from the server
    Args:
        agent (class): agent IA
        response_server (str): Response from the server
    """
    old_inventory = agent.inventory
    info_inventory = response_server.split(' ')
    for i in range(1, len(info_inventory) - 1, 2):
        ressource = info_inventory[i]
        number_str = info_inventory[i + 1].replace(',', '')
        number_str = number_str.strip()
        key_exist = agent.inventory.get(ressource)
        if (key_exist is not None):
            agent.inventory[ressource] = (int)(number_str)
    agent.adapt_behavior()
    print("The inventory change for the agent from", old_inventory, "to", agent.inventory)
    logger.info("The Inventory command was successful (received and completed).")
    return