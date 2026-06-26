##
## EPITECH PROJECT, 2026
## zappy-mirror
## File description:
## connect
##

from logger import logger, add_info_streamlit

def do_connection(agent, response_server):
    """This function is to get the number of slots available in the Team (From the server)
    Args:
        agent (class): agent IA
        response_server (str): Response from the server
    """
    if (response_server.isdigit() == False):
        logger.error("The response message from the server is not suitable for this command. response_server != int (Connect_nbr)")
        return
    else:
        value = (int)(response_server)
        agent.unused_slots = value
        logger.info(f"{agent.agent_id}: The wizard always takes care of his family; looking at his family tree, they are now {value}")
        add_info_streamlit(agent, "connect_nbr")
        