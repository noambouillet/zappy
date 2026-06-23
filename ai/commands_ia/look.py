##
## EPITECH PROJECT, 2026
## zappy-mirror
## File description:
## look
##

from logger import logger

def do_look(agent, response_server):
    """This function is to get and change the vision of the agent with info from the server
    Args:
        agent (class): agent IA
        response_server (str): Response from the serv
    """
    if not response_server.startswith("["):
        print("[LOOK] Invalid Look response:", response_server)
        return  
    old_vision = agent.vision.copy()
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
    print("La vision de l'agent à évoluer passant de", old_vision, "to", agent.vision)
    logger.info("The Look command was successful (received and completed).")