##
## EPITECH PROJECT, 2026
## zappy-mirror
## File description:
## handle_commands
##

from parsing import sys, logger, socket
from commands.forward import do_forward
from commands.left import do_left
from commands.right import do_right
from commands.look import do_look
from commands.inventory import do_inventory
from commands.connect import do_connection
from commands.broadcast import do_broadcast
from commands.fork import do_fork
from commands.eject import do_eject
from commands.take import do_take
from commands.set import do_set
from commands.incantation import do_incantation
    
def handle_recv_basic(agent, command, result_command):
    """_summary_

    Args:
        agent (_type_): _description_
        command (_type_): _description_
        result_command (_type_): _description_
    """
    dict_commands = {
        "Forward\n" : do_forward,
        "Left\n" : do_left,
        "Right\n" : do_right,
        "Look\n" : do_look,
        "Inventory\n" : do_inventory,
        "Connect_nbr\n" : do_connection,
        "Fork\n" : do_fork,
        "Eject" : do_eject,
    }
    if command in dict_commands: 
        dict_commands[command](agent, result_command)
    elif (command == "Incantation\n"):
        do_incantation(agent, result_command)
    elif (command.startswith("Broadcast")):
        do_broadcast(agent, result_command)
    elif (command.startswith("Take")):
        do_take(agent, result_command)
    elif (command.startswith("Set")):
        do_set(agent, result_command)
    else:
        print("Command uknown :", command,"\n")


    
def handle_commands(agent, str_list_command):
    """_summary_
    Args:
        agent (_type_): _description_
        str_list_command (_type_): _description_

    Returns:
        _type_: _description_
    """
    while '\n' in str_list_command:
        result_command, str_list_command = str_list_command.split('\n', 1)
        print("Result command receive by the server ", result_command)
        if (len(agent.list_commands) > 0):
            command = agent.list_commands[0]
        #print(command, result_command)
        if (result_command.startswith("dead")):
            print("Player is dead")
            sys.exit(0)
        elif (result_command.startswith("eject:")):
            #receive_eject(agent, result_command)
            continue
        elif (result_command.startswith("message")):
            #receive_message(agent, result_command)
            continue
        else:
            if (command == "Incantation\n" and result_command == "Elevation underway"):
                handle_recv_basic(agent, command, result_command)
                continue
            handle_recv_basic(agent, command, result_command)
            agent.list_commands.pop(0)
        #print(agent.list_commands)                
    return str_list_command


def send_recv_command(socket_connection, info_client, agent):
    """_summary_
    Args:
        socket_connection (_type_): _description_
        info_client (_type_): _description_
    """
    #socket_connection.sendall(("Look\n").encode('utf-8'))
    socket_connection.sendall(("Look\n").encode('utf-8'))
    socket_connection.sendall(("Forward\n").encode('utf-8'))
    socket_connection.sendall(("Connect_nbr\n").encode('utf-8'))
    #agent.list_commands.append("Look\n")
    agent.list_commands.append("Look\n")
    agent.list_commands.append("Forward\n")
    agent.list_commands.append("Connect_nbr\n")
    str_list_command = ""
    print("Les commandes envoyées par l'agent:", agent.list_commands)
    while (True):
        try:
            msg = socket_connection.recv(2048).decode('utf-8')
        except (socket.error, ConnectionError):
            logger.critical("The connection between the server and the AI has been lost because the AI has therefore died in the game.")
            sys.exit(84)
        str_list_command += msg
        str_list_command = handle_commands(agent, str_list_command)
    return