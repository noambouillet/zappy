##
## EPITECH PROJECT, 2026
## zappy-mirror
## File description:
## handle_commands
##

from parsing import sys, logger, socket
from commands_ia.forward import do_forward
from commands_ia.left import do_left
from commands_ia.right import do_right
from commands_ia.look import do_look
from commands_ia.inventory import do_inventory
from commands_ia.connect import do_connection
from commands_ia.broadcast import do_broadcast
from commands_ia.fork import do_fork
from commands_ia.eject import do_eject
from commands_ia.take import do_take
from commands_ia.set import do_set
from commands_ia.incantation import do_incantation
from commands_server.dead import receive_dead
from commands_server.ejects import receive_eject
from commands_server.messages import receive_message
from agent import *
    
def launch_commands(agent, command, response_server):
    """This function allows you to determine which command to use to launch them.
    Args:
        agent (class): Agent IA
        command (str): the command ask by the agent
        response_server (str): the response from the server
    """
    dict_commands = {
        "Forward\n" : do_forward,
        "Left\n" : do_left,
        "Right\n" : do_right,
        "Look\n" : do_look,
        "Inventory\n" : do_inventory,
        "Connect_nbr\n" : do_connection,
        "Fork\n" : do_fork,
        "Eject\n" : do_eject,
    }
    if command in dict_commands: 
        dict_commands[command](agent, response_server)
    elif (command == "Incantation\n"):
        do_incantation(agent, response_server)
    elif (command.startswith("Broadcast")):
        do_broadcast(agent, response_server)
    elif (command.startswith("Take")):
        do_take(agent, response_server, command)
    elif (command.startswith("Set")):
        do_set(agent, response_server, command)
    else:
        print(f"Command uknown : {command}")


    
def handle_commands(agent, all_responses_server):
    """This function is to handle commands betwenn commands_ia and commands_server
    Args:
        agent (class): agent IA
        all_responses_server (str): all_responses_server
    Returns:
        str: The remaining orders have not yet been processed.
    """
    while '\n' in all_responses_server:
        response_server, all_responses_server = all_responses_server.split('\n', 1)
        print(f"This is the server's current response : {response_server}")
        if (response_server.startswith("dead")):
            receive_dead()
        elif (response_server.startswith("eject:")):
            receive_eject(agent, response_server)
        elif (response_server.startswith("message")):
            receive_message(agent, response_server)
        else:
            command = agent.list_commands[0]
            if (command == "Incantation\n" and response_server == "Elevation underway"):
                launch_commands(agent, command, response_server)
                continue
            launch_commands(agent, command, response_server)
            agent.list_commands.pop(0)              
    return all_responses_server


def send_recv_command(socket_connection, agent):
    """This function is to juggle between send and receive commands (Ai/Server)
    Args:
        socket_connection (socket): the point of connection between server and ia
        agent (class): Agent IA
    """
    all_responses_server = ""
    while (True):
        try:
            response_server = socket_connection.recv(2048).decode('utf-8')
        except (socket.error, ConnectionError):
            logger.critical("The connection between the server and the AI has been lost because the AI has therefore died in the game.")
            sys.exit(84)
        all_responses_server += response_server
        all_responses_server = handle_commands(agent, all_responses_server)
    return