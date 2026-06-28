##
## EPITECH PROJECT, 2026
## zappy-mirror
## File description:
## connect
##

from logger import logger

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
        agent.last_slots_check = agent.tick
        if (agent.unused_slots == 0):
            agent.need_fork = True
        else:
            agent.need_fork = False
        logger.info(f"{agent.agent_id}: The wizard always takes care of his family; looking at his family tree, they are now {value}")
        