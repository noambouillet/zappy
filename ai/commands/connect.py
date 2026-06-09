##
## EPITECH PROJECT, 2026
## zappy-mirror
## File description:
## connect
##

from parsing import logger

def do_connection(agent, result_command):
    """This function is to get the number of slots available in the Team (From the server)
    Args:
        agent (class): agent IA
        result_command (str): Response from the server
    """
    if (result_command.isdigit() == False):
        logger.error("The response message from the server is not suitable for this command, not a number. (Connect_nbr)")
        print("The value of the reponse from the server is not a number")
        return
    else:
        value = (int)(result_command)
        agent.unusued_slots = value
        logger.info("The Connect_nbr command was successful (received and completed).")
        print("Here is the number of places still available in the team:", value)
        