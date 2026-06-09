##
## EPITECH PROJECT, 2026
## zappy-mirror
## File description:
## parsing
##

import sys, argparse, socket, logging

logger = logging.getLogger(__name__)

def print_help(value_exit):
    """This function helps the user to use the settings correctly
    Args:
        value_exit (_type_): value for exit the code
    """
    print("USAGE: ./zappy_ai -p port -n name -h machine")
    if (value_exit == 84):
        logger.critical("\nIncorrect number of arguments:\nYou need at least: -p num_port -n name_team\nAnd at most: -p num_port -n name_team -h name_machine")
    sys.exit(value_exit)

def parse_args():
    """This function parse the arguments as the port, the name, and the name of the machine
    Returns:
        _type_: port(int), name(str), machine(str)
    """
    logging.basicConfig(filename='zappy_ai.log', level=logging.DEBUG, format='%(asctime)s - %(levelname)s - %(message)s')
    logger.info("Launch of the program")
    if (len(sys.argv) == 2 and sys.argv[1] == "--help"):
        print_help(0)
    if (len(sys.argv) < 5 or len(sys.argv) > 7 or len(sys.argv) % 2 == 0):
        print_help(84)
    parser = argparse.ArgumentParser(add_help=False)
    parser.add_argument("-p", type=int, required=True, dest="port", help="The port of the machine")
    parser.add_argument("-n", type=str, required=True, dest="name_team", help="The name of the team")
    parser.add_argument("-h", type=str, required=False, dest="name_machine", help="The name of the machine; localhost by default")
    try:
        args = parser.parse_args()
    except (ValueError, SystemExit, SystemError):
        sys.exit(84)
    if (args.name_machine is None):
        args.name_machine = socket.gethostname()
    if (args.port is None or args.name_team is None):
        logger.critical("The port and the name of the machine are mandatory")
        sys.exit(84)
    if (args.port < 1024 or args.port > 65535):
        logger.critical("The machine port cannot be less than 1 or greater than 65535, the value now :", args.port)
        sys.exit(84)
    return args.port, args.name_team, args.name_machine

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
        logger.info(f"Connect to the server at the {port} port and with the machine {machine_name}")
        welcome_msg = socket_connect.recv(1024).decode('utf-8')
        if (welcome_msg != "WELCOME\n"):
            logger.critical("The first protocol message received by the client is not WELCOME, here it is", welcome_msg)
            sys.exit(84)
    except (socket.error, ConnectionRefusedError):
        logger.critical("The connection to the server failed. The problem may be due to a faulty socket or incorrectly configured server settings.")
        sys.exit(84)
    logger.info("The connection was successful with the departure protocol respected.")
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
    logger.info(f"Send the team name information for the server, {team_name}")
    msg = ""
    while msg.count("\n") < 2:
        msg += socket_connection.recv(2048).decode('utf-8')
    tab = msg.split('\n')
    if (len(tab) < 2):
        logger.critical("Sentence incomplete missing the number of slot or the value for the map (Server info incorrect)")
        sys.exit(84)
    client_num = tab[0]
    if (client_num == 'ko'):
        logger.critical("The number of slots available on the server is invalid (more space to accommodate a new customer or the team_name does not exist)")
        sys.exit(84)
    X = tab[1].split()[0]
    Y = tab[1].split()[1]
    logger.info(f"Here is the information about the client/AI that just connected TEAM_NAME : {team_name}, CLIENT_NUM : {client_num}, MAP_SIZE : {X}/{Y}")
    info_client = {"team_name" : team_name, "client_num" : client_num, 'X' : X, 'Y' : Y}
    return info_client
