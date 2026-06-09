##
## EPITECH PROJECT, 2026
## zappy-mirror
## File description:
## inventory
##

from parsing import logger

def do_inventory(agent, result_command):
    print("Function Inventory is call")
    info_inventory = result_command.split(' ')
    for i in range(1, len(info_inventory) - 1, 2):
        ressource = info_inventory[i]
        number_str = info_inventory[i + 1].replace(',', '')
        key_exist = agent.inventory.get(ressource)
        if (key_exist is not None):
            agent.inventory[ressource] = (int)(number_str)
    print("The inventory change for the agent:", agent.inventory)
    logger.info("The inventory order has been received and completed.")
    return