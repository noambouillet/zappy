##
## EPITECH PROJECT, 2026
## zappy-mirror
## File description:
## incantation
##

from logger import logger

def remove_incantation_command(agent):
    """remove incantation command from the command list of the agent

    Args:
        agent (Agent): agent
    """
    if "Incantation\n" in agent.list_commands:
        agent.list_commands.remove("Incantation\n")

def do_incantation(agent, response_server):
    """This function is start the incantation
    Args:
        agent (class): the agent IA
        result_command (str): the message send by the server for the response
    """
    if (response_server == "Elevation underway"):
        logger.info("The stars are aligned, the candles are ready, the ritual can begin")
        agent.is_incantation = True
        remove_incantation_command(agent)
    elif (response_server.startswith("Current level:")):
        list_level = response_server.split(':')
        level = int(list_level[1])
        logger.info(f"{agent.agent_id}: The wizard's knowledge has grown; he feels his state evolving from level {agent.level} to level {level}")
        agent.level = level
        agent.is_incantation = False
        agent.prepare_incantation = False
        agent.joining_incantation = False
        agent.tab_id_teammate = []
        agent.mailbox.clear()
        agent.teammate_on_tile = 1
        remove_incantation_command(agent)
    elif (response_server == "ko"):
        logger.info(f"{agent.agent_id}: The ritual failed; the heavens did not grant us their piety.")
        agent.is_incantation = False
        agent.prepare_incantation = False
        agent.joining_incantation = False
        agent.teammate_on_tile = 1
        agent.tab_id_teammate = []
        agent.mailbox.clear()
        remove_incantation_command(agent)
    return   