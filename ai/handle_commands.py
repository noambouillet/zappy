##
## EPITECH PROJECT, 2026
## zappy-mirror
## File description:
## handle_commands
##

from parsing import sys, socket
from constant import INVENTORY_FREQUENCE
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
from agent import Agent
import select
from logger import logger


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
        logger.debug(f"Command unknown : {command}")
    
def handle_commands(agent : Agent, all_responses_server):
    """This function is to handle commands betwenn commands_ia and commands_server
    Args:
        agent (class): agent IA
        all_responses_server (str): all_responses_server
    Returns:
        str: The remaining orders have not yet been processed.
    """
    while '\n' in all_responses_server:
        response_server, all_responses_server = all_responses_server.split('\n', 1)
        if (len(agent.list_commands) > 0):
            command = agent.list_commands[0]
        else:
            command = None
        logger.debug(f"This is the server's current response : {response_server}")
        if (response_server.startswith("dead")):
            receive_dead()
        elif (response_server.startswith("eject:")):
            receive_eject(agent, response_server)
        elif (response_server.startswith("message")):
            receive_message(agent, response_server)
        elif (response_server == "Elevation underway"):
            do_incantation(agent, response_server)
        elif (response_server.startswith("Current level:")):
            do_incantation(agent, response_server)
        elif (response_server == "ko"):
            if agent.is_incantation:
                do_incantation(agent, response_server)
            elif command is not None:
                launch_commands(agent, command, response_server)
                if (len(agent.list_commands) > 0):
                    agent.list_commands.pop(0)
        else:
            if (command is None):
                continue
            launch_commands(agent, command, response_server)
            if (len(agent.list_commands) > 0):
                agent.list_commands.pop(0)
        logger.debug(f"List commands: {agent.list_commands}")
    return all_responses_server

def check_inventory_regularly(agent, tab_commands):
    """_summary_

    Args:
        agent (_type_): _description_
    """
    if ((agent.tick - agent.last_inventory) >= INVENTORY_FREQUENCE and agent.is_incantation != True):
        agent.last_inventory = agent.tick
        tab_commands.append("Inventory\n")
    return tab_commands

def send_commands(socket_connection : socket.socket, agent : Agent):
    """Send the commands to server from the execute behavior (class)

    Args:
        socket_connection (socket.socket): the socket connection between ia and server
        agent (Agent): Agent IA
    """
    need_look = ["Forward\n", "Left\n", "Right\n", "Eject\n"]
    if agent.is_incantation == True:
        return
    if (len(agent.list_commands) == 0):
        agent.adapt_behavior()
        tab_commands = []
        if agent.vision == [[]]:
            tab_commands.append("Look\n")
        tab_commands += agent.behavior.execute(agent)
        if not tab_commands:
            return
        for command in tab_commands:
            if (command in need_look or command.startswith("Set")) and agent.joining_incantation == False:
                tab_commands.append("Look\n")
                break
        tab_commands = check_inventory_regularly(agent, tab_commands)
        for command in tab_commands:
            if (len(agent.list_commands) < 10):
                agent.list_commands.append(command)
                try:
                    socket_connection.sendall((command).encode('utf-8'))
                except OSError:
                    logger.critical("The connection between the server and the AI has been lost because the AI has therefore died in the game.")
                    socket_connection.close()
                    sys.exit(84)
        logger.debug(f"New list of commands send by the client : {agent.list_commands}")

def send_recv_command(socket_connection : socket.socket, agent : Agent):
        """This function is to juggle between send and receive commands (Ai/Server)
        Args:
            socket_connection (socket): the point of connection between server and ia
            agent (class): Agent IA
        """
        all_responses_server = ""
        socket_connection.setblocking(False)
        while (True):
            try:
                result, _, exception = select.select([socket_connection], [], [socket_connection], 0.1)
            except (select.error, OSError):
                logger.critical("The connection between the server and the AI has been lost because the AI has therefore died in the game.")
                sys.exit(84)
            if exception:
                logger.critical("The connection between the server and the AI has been lost because the AI has therefore died in the game.")
                sys.exit(84)
            if result:
                try:
                    response_server = socket_connection.recv(2048).decode('utf-8')
                    if (not response_server):
                        logger.critical("The connection between the server and the AI has been lost because the AI has therefore died in the game.")
                        sys.exit(0)
                    all_responses_server += response_server
                except (BlockingIOError):
                    pass
                except (socket.error, ConnectionError):
                    logger.critical("The connection between the server and the AI has been lost because the AI has therefore died in the game.")
                    sys.exit(84)
            all_responses_server = handle_commands(agent, all_responses_server)
            send_commands(socket_connection, agent)
        return
