##
## EPITECH PROJECT, 2026
## zappy-mirror
## File description:
## forward
##

from logger import logger, add_info_streamlit

def do_forward(agent, response_server):
    """This function is to advance the character
    Args:
        agent (class): the agent IA
        response_server (str): the message send by the server for the response
    """
    if (response_server == "ok"):
        add_info_streamlit(agent, "forward")
        logger.info(f"{agent.agent_id}: The wizard decided to follow his destiny and go towards the lands in the direction of {agent.direction.name}")
    else:
        logger.error("The response message from the server is not suitable for this command. response_server != ok (Left)")
    return