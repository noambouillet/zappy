##
## EPITECH PROJECT, 2026
## zappy-mirror
## File description:
## parsing
##

import sys, argparse, socket
from logger import logger

def display_help(value_exit):
    """This function helps the user to use the settings correctly

    Args:
        value_exit (int): value for exit the code
    """
    logger.warning("USAGE: ./zappy_ai -p port -n name -h machine")
    if (value_exit == 84):
        logger.error("\nIncorrect number of arguments:\nYou need at least: -p num_port -n name_team\nAnd at most: -p num_port -n name_team -h name_machine")
    sys.exit(value_exit)

def parse_args():
    """this function parse the arguments as the port, the name, and the name of the machine

    Returns:
        struct{int, str, str}: port, name, machine
    """
    logger.debug("Launch of the program")
    if (len(sys.argv) == 2 and sys.argv[1] == "--help"):
        display_help(0)
    if (len(sys.argv) < 3 or len(sys.argv) > 7 or len(sys.argv) % 2 == 0):
        display_help(84)
    parser = argparse.ArgumentParser(add_help=False)
    parser.add_argument("-p", type=int, required=False, dest="port", help="The port of the machine")
    parser.add_argument("-n", type=str, required=True, dest="name_team", help="The name of the team")
    parser.add_argument("-h", type=str, required=False, dest="name_machine", help="The name of the machine; localhost by default")
    try:
        args = parser.parse_args()
    except (ValueError, SystemExit, SystemError):
        sys.exit(84)
    if (args.port is None):
        args.port = 4242
    if (args.name_machine is None):
        args.name_machine = socket.gethostname()
    if (args.port is None or args.name_team is None):
        logger.error("The port and the name of the machine are mandatory")
        sys.exit(84)
    if (args.port < 1024 or args.port > 65535):
        logger.error(f"The machine port cannot be less than 1 or greater than 65535, the value now : {args.port}")
        sys.exit(84)
    return args.port, args.name_team, args.name_machine