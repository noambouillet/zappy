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
        print("EXPLORE TICK", agent.tick)
        food_commands = agent.take_food_on_tile()
        if food_commands:
            return food_commands
        if (self.define_leader(agent) == True):
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
        
    def define_leader(self, agent):
        """This function is for see if we can help a leader
        Args:
            agent (class): Agent/IA
        """
        msg_leader = None
        msg_leader_tick = 0
        for msg in agent.mailbox:
            if (msg["action"] == "INCANTATION" and msg["team"] == agent.team_name and msg["level"] == agent.level):
                msg_tick = msg["tick"]
                if (msg_leader is None or msg_tick < msg_leader_tick):
                    msg_leader = msg
                    msg_leader_tick = msg_tick
        if (msg_leader):
            agent.leader_id = msg_leader["sender_id"]
            agent.direction_to_follow = msg_leader["direction"]
            agent.joining_incantation = True
            agent.mailbox.clear()
            print("EXPLORE AGENT LEADER", agent.leader_id)
            print("EXPLORE AGENT DIRECTION", agent.direction_to_follow)
            print("EXPLORE AGENT MAILBOX", agent.mailbox)
            return True
        agent.joining_incantation = False
        return False