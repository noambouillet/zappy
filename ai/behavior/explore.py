##
## EPITECH PROJECT, 2026
## zappy-mirror
## File description:
## explore
##

from .class_behavior import Behavior
from constant import requirement_for_progress

class Explore(Behavior):
    def execute(self, agent):
        agent.tick += 1
        food_commands = agent.take_food_on_tile()
        if food_commands:
            return food_commands
        if (self.which_leader(agent) == True):
            return []
        needed_ressources = self.get_needed_ressources(agent)
        best_ressource, tile_index = self.find_best_ressource(agent, needed_ressources)
        command_list = self.get_command(agent, best_ressource, tile_index)
        if command_list:
            return command_list
        return ["Forward\n"]

    def get_needed_ressources(self, agent):
        requirements = requirement_for_progress[agent.level - 1]
        inventory = agent.inventory
        needed_ressources = {"linemate" : 0, "deraumere" : 0, "sibur" : 0, "mendiane" : 0, "phiras" : 0, "thystame" : 0}
        for ressource, current_quantity in inventory.items():
            if ressource == "food":
                continue
            missing = requirements[ressource] - current_quantity
            if missing > 0:
                needed_ressources[ressource] = missing
            else:
                needed_ressources[ressource] = 0
        return needed_ressources

    def find_best_ressource(self, agent, needed_ressources):
        for index, tile in enumerate(agent.vision):
            for ressource in tile:
                if ressource in needed_ressources and needed_ressources[ressource] > 0:
                    return ressource, index
        return None, None

    def get_command(self, agent, ressource, tile_index):
        if ressource is None or tile_index is None:
            return None
        if tile_index == 0:
            return [f"Take {ressource}\n"]
        return agent.go_to(tile_index) + [f"Take {ressource}\n"]

    def which_leader(self, agent):
        """This function is for see if we can help a leader
        Args:
            agent (class): Agent/IA
        """
        tick_leader = 0
        for msg in agent.mailbox:
            if (msg["action"] == "INCANTATION" and msg["level"] == agent.level and msg["team"] == agent.team_name):
                direction = msg["direction"]
                sender_agent_id = msg["sender_id"]
                tick_msg = msg["tick"]
                if (tick_msg > tick_leader):
                    agent.leader_id = sender_agent_id
                    agent.direction_to_follow = direction
                    agent.joining_incantation = True
                    tick_leader = tick_msg 
        if (agent.joining_incantation == True):
            return True
        return False