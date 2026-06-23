##
## EPITECH PROJECT, 2026
## zappy-mirror
## File description:
## connection
##

import sys, socket
from logger import logger

def connect_to_server(port, machine_name):
    """ This function is to start the connection with the server and recover the socket
    Args:
        port (_type_): the port for the server
        machine_name (_type_): the machine name for the connection
    Returns:
        socket: the socket of the connection to receive and send informations
    """
    socket_connect = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    try:
        socket_connect.connect((machine_name, port))
        logger.debug(f"Connect to the server at the {port} port and with the machine {machine_name}")
        welcome_msg = socket_connect.recv(1024).decode('utf-8')
        if (welcome_msg != "WELCOME\n"):
            logger.critical(f"The first protocol message received by the client is not WELCOME, here it is {welcome_msg}")
            sys.exit(84)
    except (socket.error, ConnectionRefusedError):
        logger.critical("The connection to the server failed. The problem may be due to a faulty socket or incorrectly configured server settings.")
        sys.exit(84)
    logger.debug("The connection was successful with the departure protocol respected.")
    return socket_connect

def communication_server_client(socket_connection, team_name):
    """This function is to do the communication between the serv and the client, for recover slot, or world's dimensions
    Args:
        socket_connection (socket): The socket with the connection
        team_name (str): The name to send for the server
    Returns:
        dict: a dictionnary with the values of the slot and dimensions
    """
    socket_connection.sendall((team_name + "\n").encode('utf-8'))
    logger.debug(f"Send the team name information for the server, {team_name}")
    msg = ""
    while msg.count("\n") < 2:
        msg += socket_connection.recv(2048).decode('utf-8')
        if (msg == "ko\n"):
            logger.critical("The number of slots available on the server is invalid (more space to accommodate a new customer or the team_name does not exist)")
            sys.exit(84)
    tab = msg.split('\n')
    if (len(tab) < 2):
        logger.critical("Sentence incomplete missing the number of slot or the value for the map (Server info incorrect)")
        sys.exit(84)
    client_num = tab[0]
    X = tab[1].split()[0]
    Y = tab[1].split()[1]
    logger.debug(f"Here is the information about the client/AI that just connected TEAM_NAME : {team_name}, CLIENT_NUM : {client_num}, MAP_SIZE : {X}/{Y}")
    info_client = {"team_name" : team_name, "client_num" : client_num, 'X' : X, 'Y' : Y}
    return info_client
