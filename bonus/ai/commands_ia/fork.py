##
## EPITECH PROJECT, 2026
## zappy-mirror
## File description:
## fork
##

from logger import logger, add_info_streamlit

def do_fork(agent, response_server):
    """This function is to fork a player
    Args:
        agent (class): the agent IA
        response_server (str): the message send by the server for the response
    """
    if (response_server == "ok"):
        agent.unused_slots += 1
        add_info_streamlit(agent, "fork")
        logger.info(f"{agent.agent_id}: A new egg of power has been laid; the mage's lineage is expanding.")
    else:
        logger.error("The response message from the server is not suitable for this command. response_server != ok (Fork)")
    return