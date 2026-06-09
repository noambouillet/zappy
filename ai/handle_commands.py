##
## EPITECH PROJECT, 2026
## zappy-mirror
## File description:
## handle_commands
##

from parsing import sys, logger, socket


def do_inventory(agent, result_command):
    print("Function Inventory is call")
    #print(result_command)
    info_inventory = result_command.split(' ')
    #print(info_inventory)
    for i in range(1, len(info_inventory) - 1, 2):
        ressource = info_inventory[i]
        number_str = info_inventory[i + 1].replace(',', '')
        #print(f"The ressource {ressource} with the value {number_str}")
        key_exist = agent.inventory.get(ressource)
        if (key_exist is not None):
            agent.inventory[ressource] = (int)(number_str)
    print("The inventory change for the agent:", agent.inventory)
    logger.info("The inventory order has been received and completed.")
    if (len(agent.list_commands) > 0):
        agent.list_commands.pop(0)
    return

def do_look(agent, result_command):
    print("Function Look is call")
    agent.vision = result_command
    print("Vision de l'agent:", agent.vision)
    logger.info("The look order has been received and completed")
    if (len(agent.list_commands) > 0):
        agent.list_commands.pop(0)
    return

def do_basic_commands(agent):
    agent.list_commands(0)
    
def sort_command(agent, str_list_command):
    """_summary_

    Args:
        agent (_type_): _description_
        str_list_command (_type_): _description_ [ food 9, ...]\nOK\n
    """
    print("Toutes les commandes en attente :", str_list_command)
    while '\n' in str_list_command:
        result_command, str_list_command = str_list_command.split('\n', 1) #Ici, en gros on découpe la chaine une seule fois, c'est a dire on recupere la commande qu'on veut check mais aussi le reste de la commande sans cette element
        print("Resultat du serveur :", result_command)
        split_command = result_command.split(' ')
        # print(split_command)
        if (split_command[0] == "dead"):
            print("The player is dead")
            logger.info("The connection between the server and the AI ​​has been lost because the AI ​​has therefore died in the game.")
            sys.exit(0)
        elif (split_command[0] == "ok"):
            #On check dans la liste des commandes l'ordre, fonction spécifique.
            continue
        elif (split_command[0] == "["  and split_command[1] == "food"):
            do_inventory(agent, result_command)
        elif (split_command[0] == "[" and split_command[1] == "player"):
            do_look(agent, result_command)
        elif (split_command[0] == "ko"):
            agent.list_commands.pop(0)
            print("The command result with ko")
    return str_list_command


def send_recv_command(socket_connection, info_client, agent):
    """_summary_
    Args:
        socket_connection (_type_): _description_
        info_client (_type_): _description_
    """
    socket_connection.sendall(("Look\n").encode('utf-8'))
    socket_connection.sendall(("Inventory\n").encode('utf-8'))
    agent.list_commands.append("Look\n")
    agent.list_commands.append("Inventory\n")
    str_list_command = ""
    print("Les commandes envoyées par l'agent:", agent.list_commands)
    while (True):
        try:
            msg = socket_connection.recv(2048).decode('utf-8')
        except (socket.error, ConnectionError):
            logger.critical("The connection between the server and the AI ​​has been lost because the AI ​​has therefore died in the game.")
            sys.exit(84)
        str_list_command += msg
        str_list_command = sort_command(agent, str_list_command)
    return