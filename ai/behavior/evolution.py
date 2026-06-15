##
## EPITECH PROJECT, 2026
## zappy-mirror
## File description:
## evolution
##

from .class_behavior import Behavior
from constant import requirement_for_progress

class Evolution(Behavior):
    def execute(self, agent):
        """This function is to execute evolution class
        Args:
            agent (class): Agent IA
        Returns:
            str: command for the agent
        """
        if (agent.vision == [[]]):
            return ["Look\n"]
        if (agent.eject_players == True):
            agent.eject_players = False
            return ["Eject\n"]
        condition = self.implementation_of_conditions(agent)
        if (condition != []):
            return condition
        agent.is_incantation = True
        return ["Incantation\n"]
    
    def implementation_of_conditions(self, agent):
        """This function is implementation conditions
        Args:
            agent (class): Agent/IA
        """
        requirement = requirement_for_progress[agent.level - 1]
        nb_players = requirement["nb_players"]
        agent.teammate_same_level = 1
        self.check_players_tile(agent)
        if (agent.teammate_same_level < nb_players):
            if (agent.unused_slots == 0):
                return ["Fork\n"]
            else:
                return [f"Broadcast INVOCATION|{agent.level}|{agent.team_name}"]
        setup = self.check_ressources_tile(agent, requirement)
        if (setup != ["Look\n"]):
            agent.prepare_incantation = True
            return setup
        agent.prepare_incantation = False
        return []
    
    def check_players_tile(self, agent):
        """This function is to check the allies or the ennemies on the tile of the Invocation
        Args:
            agent (class): Agent/IA
        """
        for msg in agent.mailbox:
            if (msg.action == "AVAILABLE" and msg.level == agent.level and msg.team_name == agent.team_name and msg.direction == 0):
                agent.teammate_same_level += 1
            elif (msg.action == "AVAILABLE" and msg.team_name != agent.team_name and msg.direction == 0):
                agent.eject_players = True
        agent.mailbox.clear()
        return
    
    def check_ressources_tile(self, agent, resources_for_upgrade):
        """This function is to determinate, the ressources on a tile are necessary/useless 
        Args:
            agent (class): Agent/IA
        """
        list_command = []
        print(agent.vision)
        resources_tiles = {"food" : 0, "linemate" : 0, "deraumere" : 0, "sibur" : 0, "mendiane" : 0, "phiras" : 0, "thystame" : 0}
        for element in agent.vision[0]:
            if (element != "player"):
                resources_tiles[element] += 1
        for resource, nb_ressources in resources_tiles.items():
            if (resource != "nb_players"):
                if (resource == "food" and nb_ressources > 0):
                    for _ in range(nb_ressources):
                        list_command += [f"Take food\n"]
                elif (resource != "food" and nb_ressources > resources_for_upgrade[resource]):
                    for _ in range(nb_ressources - resources_for_upgrade[resource]):
                        list_command += [f"Take {resource}\n"]
                elif (resource != "food" and nb_ressources < resources_for_upgrade[resource]):
                    for _ in range(resources_for_upgrade[resource] - nb_ressources):
                        list_command += [f"Set {resource}\n"]
        list_command += ["Look\n"]
        return list_command
                    
                    