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
        #return [f"Broadcast INVOCATION|{agent.level}\n"]
        if (agent.vision == [[]]):
            return ["Look\n"]
        requirement = requirement_for_progress[agent.level - 1]
        nb_players = requirement["nb_players"]
        setup = self.check_ressources_tile(agent, requirement)
        if (self.nb_player_in_tile(agent) < nb_players):
            if (agent.unused_slots > 0):
                return [f"Broadcast INVOCATION|{agent.level}"]
            else:
                return ["Fork\n"]
        if (setup != ["Look\n"]):
            return setup
        return ["Incantation\n"]
    
    def nb_player_in_tile(self, agent):
        """This function is for to count the number of player on the same tile
        Args:
            agent (class): Agent/IA
        """
        nb_players = 0
        for elem in agent.vision[0]:
            if (elem == "player"):
                nb_players += 1
        return nb_players

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
        #print(resources_tiles)
        for resource, nb_ressources in resources_tiles.items():
            if (resource != "nb_players"):
                #print(resource, nb_ressources)
                if (resource == "food" and nb_ressources > 0):
                    for _ in range(nb_ressources):
                        list_command += [f"Take food\n"]
                    #print(list_command)
                elif (resource != "food" and nb_ressources > resources_for_upgrade[resource]):
                    for _ in range(nb_ressources - resources_for_upgrade[resource]):
                        list_command += [f"Take {resource}\n"]
                    #print(list_command)
                elif (resource != "food" and nb_ressources < resources_for_upgrade[resource]):
                    for _ in range(resources_for_upgrade[resource] - nb_ressources):
                        list_command += [f"Set {resource}\n"]
                    #print(list_command)
        list_command += ["Look\n"]
        return list_command
                    
                    