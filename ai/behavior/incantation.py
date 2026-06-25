##
## EPITECH PROJECT, 2026
## zappy-mirror
## File description:
## Incantation
##

from .class_behavior import Behavior
from constant import requirement_for_progress, Macro, NB_PLAYERS_REQUIRED
from logger import logger

class Incantation(Behavior):
    def execute(self, agent):
        logger.debug("\n========== INCANTATION ==========")
        agent.tick += 1
        agent.display_info()
        if (agent.inventory["food"] < agent.get_macro_ratio(Macro.MIN_FOOD)):
            agent.survive = True
            agent.prepare_incantation = False
            agent.joining_incantation = False
            return []
        (is_leader, leader_id) = self.can_become_leader(agent)
        if (not is_leader):
            agent.leader_id = leader_id
            agent.joining_incantation = True
            agent.prepare_incantation = False
            return []
        agent.leader_id = agent.agent_id 
        is_eject = self.count_follower(agent)
        if (is_eject == True):
            return ["Eject\n"]
        (is_prepare, list_command) = self.verif_incantation(agent)
        if (is_prepare == False):
            return list_command
        agent.is_incantation = True
        return ["Incantation\n"]
        
    def can_become_leader(self, agent):
        """return if the agent can become a leader

        Args:
            agent (Agent): agent

        Returns:
            tuple(bool, int): can become leader and id of the new leader
        """
        for msg in agent.mailbox:
            if (msg["action"] == "INCANTATION" and msg["level"] == agent.level and msg["team"] == agent.team_name):
                other_agent_id = msg["sender_id"]
                if (other_agent_id < agent.agent_id):
                    agent.joining_incantation = True
                    return (False, other_agent_id)
        return (True, agent.agent_id)
    
    def count_follower(self, agent):
        """count the number of agent that follow the incantation to ensure they are all from the same team

        Args:
            agent (Agent): agent

        Returns:
            bool: bool to decide if the agent that start the incantation need to eject enemies
        """
        eject_players = False
        mailbox_message = agent.mailbox.copy()
        for msg in mailbox_message:
            if (msg["sender_id"] == agent.agent_id):
                continue
            if (msg["action"] == "AVAILABLE" and msg["level"] == agent.level and msg["team"] == agent.team_name and msg["sender_id"] not in agent.tab_id_teammate):
                direction = msg["direction"]
                if (direction == 0):
                    agent.tab_id_teammate.append(msg["sender_id"])
                    agent.teammate_on_tile += 1
            elif (msg["action"] == "LEAVING" and msg["level"] == agent.level and msg["team"] == agent.team_name and msg["sender_id"] in agent.tab_id_teammate):
                agent.tab_id_teammate.remove(msg["sender_id"])
                agent.teammate_on_tile -= 1
            elif (msg["team"] != agent.team_name and msg["direction"] == 0):
                eject_players = True
                agent.tab_id_teammate = []
                agent.teammate_on_tile = 1
        agent.mailbox = [msg for msg in agent.mailbox if msg.get("action") == "INCANTATION"]
        return eject_players
    
    def verif_incantation(self, agent):
        """check if all the condition for an incantation are OK before starting

        Args:
            agent (_type_): _description_

        Returns:
            tuple(bool, list): bool that indicate if the agent can start the incantation and list of command to execute
        """
        if NB_PLAYERS_REQUIRED > agent.teammate_on_tile:
            return False, [f"Broadcast INCANTATION|{agent.level}|{agent.team_name}|{agent.tick}|{agent.agent_id}\n", "Look\n"]
        nb_players_in_vision = agent.vision[0].count("player")
        if (NB_PLAYERS_REQUIRED > nb_players_in_vision):
            return False, ["Look\n"]
        commands, can_start = self.prepare_tile_resources(agent)
        if commands:
            agent.prepare_incantation = True
            return False, commands
        if not can_start:
            agent.prepare_incantation = False
            return False, []
        agent.prepare_incantation = False
        return True, []
        
    def prepare_tile_resources(self, agent):
        """set all required ressources for the incantation on the current tile

        Args:
            agent (Agent): agent

        Returns:
            tuple(list, bool): list of command to execute and bool that indicate if the ressources have been correctly set
        """
        commands = []
        level = agent.level - 1
        if (level >= 7):
            level = 6
        required = requirement_for_progress[level]
        resources_on_tile = {"linemate" : 0, "deraumere" : 0, "sibur" :  0, "mendiane" : 0, "phiras" : 0, "thystame" : 0}
        for resource in agent.vision[0]:
            if (resources_on_tile.get(resource) is not None):
                resources_on_tile[resource] += 1
        for resource in resources_on_tile:
            missing = required[resource] - resources_on_tile[resource]
            if missing <= 0:
                continue
            available = agent.inventory.get(resource, 0)
            if available < missing:
                return [], False
            for _ in range(missing):
                commands.append(f"Set {resource}\n")
        if commands:
            return commands, False
        return [], True