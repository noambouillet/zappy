##
## EPITECH PROJECT, 2026
## zappy-mirror
## File description:
## ejects
##

from logger import logger

def receive_eject(agent, response_server):
    """This function is to receive the message that you have been ejected from the tile
    Args:
        agent (_type_): _description_
        response_server (_type_): _description_
    """
    split_response = response_server.split(':')
    logger.info(f"{agent.agent_id}: You were the victim of an Eject spell, so you found yourself on a new land..{split_response[1]}")
    agent.vision = [[]]
    if agent.is_incantation:
        return
    agent.mailbox.clear()
    agent.tab_id_teammate.clear()
    agent.teammate_on_tile = 1
    agent.joining_incantation = False
    agent.prepare_incantation = False
    agent.leader_id = 0
    agent.direction_to_follow = 0
    agent.send_available = False
    agent.last_send_leader = agent.tick
    return
