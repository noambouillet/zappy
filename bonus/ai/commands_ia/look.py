##
## EPITECH PROJECT, 2026
## zappy-mirror
## File description:
## look
##

from logger import logger, add_info_streamlit

def do_look(agent, response_server):
    """This function is to get and change the vision of the agent with info from the server
    Args:
        agent (class): agent IA
        response_server (str): Response from the serv
    """
    if not response_server.startswith("["):
        return  
    remove_bracket = response_server.strip()[1:-1]
    vision_split = remove_bracket.split(',')
    tab_vision = []
    for i in vision_split:
        elem_tab = i.strip()
        if (elem_tab):
            tab_vision.append(elem_tab.split())
        else:
            tab_vision.append([])
    agent.vision = tab_vision
    logger.info(f"{agent.agent_id}: Thanks to a night vision spell, the wizard's vision is now as clear as crystal {agent.vision}")
    add_info_streamlit(agent, "look")