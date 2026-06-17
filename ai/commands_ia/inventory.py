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
    if not response_server.startswith("["):
        print("[INVENTORY] Invalid response:", response_server)
        return
    old_inventory = agent.inventory.copy()
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
                print("[INVENTORY] Invalid number:", ressource, number_str)
        index += 2
    agent.adapt_behavior()
    print("The inventory change for the agent from", old_inventory, "to", agent.inventory)
    logger.info("The Inventory command was successful (received and completed).")