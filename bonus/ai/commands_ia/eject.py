##
## EPITECH PROJECT, 2026
## zappy-mirror
## File description:
## eject
##

from logger import logger, add_info_streamlit

def do_eject(agent, response_server):
    """This function is to know if an egg or a player was eject from the tile 
    Args:
        agent (class): the agent IA
        response_server (str): the message send by the server for the response
    """
    if (response_server == "ok"):
        add_info_streamlit(agent, "eject")
        logger.info(f"{agent.agent_id}: The Eject spell struck the square, banishing anyone who was there")
    elif (response_server == "ko"):
        add_info_streamlit(agent, "eject ko")
        logger.info(f"{agent.agent_id}: The Eject spell did not work; the person was driven from their lands.")  
    else:
        logger.error("The response message from the server is not suitable for this command, result_command != ok or ko. (Eject)")
    return