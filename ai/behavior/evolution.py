##
## EPITECH PROJECT, 2026
## zappy-mirror
## File description:
## evolution
##

from .class_behavior import Behavior
from constant import requirement_for_progress, INCANTATION_FREQUENCE, FOOD_FOR_INCANTATION

class Evolution(Behavior):
    def execute(self, agent):
        """This function is to execute evolution class
        Args:
            agent (class): Agent IA
        Returns:
            str: command for the agent
        """
        agent.tick += 1
        # food_commands = agent.take_food_on_tile()
        # if food_commands:
        #     return food_commands
        if (agent.inventory["food"] < FOOD_FOR_INCANTATION):
            agent.survive = True
            agent.prepare_incantation = False
            agent.is_incantation = False
            agent.joining_invocation = False
            agent.waiting = False
            return ["Look\n"]
        if (agent.vision == [[]]):
            return ["Look\n"]
        if (agent.eject_players == True):
            agent.eject_players = False
            return ["Eject\n"]
        conditions = self.verif_conditions(agent)
        if (conditions):
            return conditions
        agent.is_incantation = True
        return ["Incantation\n"]
    
    def check_players_on_tile(self, agent):
        """Check allies coming to the invocation tile
        """
        nb_agent_comming = 0
        new_mailbox = []
        for msg in agent.mailbox:
            if (msg.get("action") == "AVAILABLE" and msg.get("level") == agent.level and msg.get("team") == agent.team_name):
                direction = msg.get("direction")
                if direction != 0:
                    nb_agent_comming += 1
            elif (msg.get("action") == "AVAILABLE" and msg.get("team") != agent.team_name and msg.get("direction") == 0):
                agent.eject_players = True
            else:
                new_mailbox.append(msg)
        agent.mailbox = new_mailbox
        return nb_agent_comming
    
    def verif_conditions(self, agent):
        """This function is implementation conditions
        Args:
            agent (class): Agent/IA
        """
        requirement = requirement_for_progress[agent.level - 1]
        nb_players_required = requirement["nb_players"]
        agent.teammate_same_level = agent.vision[0].count("player")
        nb_coming_players = self.check_players_on_tile(agent)
        if (agent.teammate_same_level < nb_players_required):
            if ((agent.teammate_same_level + nb_coming_players) < nb_players_required and agent.unused_slots == 0):
                return ["Fork\n"]
            list_command = []
            if ((agent.tick - agent.last_incantation) >= INCANTATION_FREQUENCE):
                agent.last_incantation = agent.tick
                list_command.append(f"Broadcast INVOCATION|{agent.level}|{agent.team_name}|{agent.agent_id}\n")
            list_command.append("Look\n")
            return list_command
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
            #     for _ in range(nb_resource - requirement[resource]):
            #         list_command += [f"Take {resource}\n"]
                continue
            elif resource != "food" and nb_resource < requirement[resource]:
                missing = requirement[resource] - nb_resource
                for _ in range(missing):
                    if agent.inventory[resource] <= 0:
                        print("[EVOLUTION] Missing resource in inventory:", resource)
                        agent.prepare_incantation = False
                        return ["Look\n"]
                    list_command += [f"Set {resource}\n"]
        list_command += ["Look\n"]
        return list_command
            