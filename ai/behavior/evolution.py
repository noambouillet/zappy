##
## EPITECH PROJECT, 2026
## zappy-mirror
## File description:
## evolution
##

from .class_behavior import Behavior
from constant import requirement_for_progress, INVOCATION_FREQUENCE

class Evolution(Behavior):
    def execute(self, agent):
        """This function is to execute evolution class
        Args:
            agent (class): Agent IA
        Returns:
            str: command for the agent
        """
        agent.tick += 1
        if (agent.vision == [[]]):
            return ["Look\n"]
        if (agent.inventory["food"] < 10):
            agent.survive = True
            return ["Look\n", "Inventory\n"]
        call = self.call_for_another_incantation(agent)
        if (call):
            return call
        if (agent.eject_players == True):
            agent.eject_players = False
            return ["Eject\n"]
        conditions = self.verif_conditions(agent)
        if (conditions):
            return conditions
        agent.is_incantation = True
        return ["Incantation\n"]
        
    def call_for_another_incantation(self, agent):
        """This function is call and join another incantation
        Args:
            agent (class): Agent/IA
        """
        for i, msg in enumerate(agent.mailbox):
            if (msg["action"] == "INVOCATION" and msg["level"] == agent.level and msg["team"] == agent.team_name):
                direction = msg["direction"]
                agent.mailbox.pop(i)
                if (direction != 0):
                    return [f"Broadcast AVAILABLE|{agent.level}|{agent.team_name}\n"] + agent.follow_direction(direction) + ["Look\n"]
                else:
                    return [f"Broadcast AVAILABLE|{agent.level}|{agent.team_name}\n"] + ["Look\n"]
        return []
    
    def check_players_on_tile(self, agent):
        """This function is to check the allies or the ennemies on the tile of the Invocation
        Args:
            agent (class): Agent/IA
        """
        nb_agent_comming = 0
        for i, msg in enumerate(agent.mailbox):
            if (msg["action"] == "AVAILABLE" and msg["level"] == agent.level and msg["team"] == agent.team_name):
                direction = msg["direction"]
                if (direction == 0):
                    agent.teammate_same_level += 1
                else:
                    nb_agent_comming += 1
            elif (msg["action"] == "AVAILABLE" and msg["team"] != agent.team_name and msg["direction"] == 0):
                agent.eject_players = True
            agent.mailbox.pop(i)
        agent.mailbox.clear()
        return nb_agent_comming
    
    def verif_conditions(self, agent):
        """This function is implementation conditions
        Args:
            agent (class): Agent/IA
        """
        requirement = requirement_for_progress[agent.level - 1]
        nb_players_required = requirement["nb_players"]
        agent.teammate_same_level = 1
        nb_coming_players = self.check_players_on_tile(agent)
        if (agent.teammate_same_level < nb_players_required):
            if ((agent.teammate_same_level + nb_coming_players) < nb_players_required and agent.unused_slots == 0):
                return ["Fork\n"]
            if (agent.tick % 15 == 0):
                return [f"Broadcast INVOCATION|{agent.level}|{agent.team_name}\n"]
            return ["Look\n"]
        setup = self.prepare_resources_tile(agent, requirement)
        if (setup != ["Look\n"]):
            agent.prepare_incantation = True
            return setup
        agent.prepare_incantation = False
        return []
            
    def prepare_resources_tile(self, agent, requirement):
        """This function is to determinate, the ressources on a tile are necessary/useless 
        Args:
            agent (class): Agent/IA
        """
        list_command = []
        resources_tiles = {"food" : 0, "linemate" : 0, "deraumere" : 0, "sibur" : 0, "mendiane" : 0, "phiras" : 0, "thystame" : 0}
        for elem in agent.vision[0]:
            if (elem != "player"):
                resources_tiles[elem] += 1
        for resource, nb_resource in resources_tiles.items():
            if (resource == "player"):
                continue
            if (resource == "food" and nb_resource > 0):
                for _ in range(nb_resource):
                    list_command += [f"Take {resource}\n"]
            elif (resource != "food" and nb_resource > requirement[resource]):
                for _ in range(nb_resource - requirement[resource]):
                    list_command += [f"Take {resource}\n"]
            elif (resource != "food" and nb_resource < requirement[resource]):
                for _ in range(requirement[resource] - nb_resource):
                    list_command += [f"Set {resource}\n"]
        list_command += ["Look\n"]
        return list_command
            