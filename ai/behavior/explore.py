##
## EPITECH PROJECT, 2026
## zappy-mirror
## File description:
## explore
##

from .class_behavior import Behavior
from constant import requirement_for_progress, FOOD_TO_REACH
import math

class Explore(Behavior):
    def execute(self, agent):
        print("\n========== EXPLORE ==========")
        agent.tick += 1
        agent.display_info()
        if (self.define_leader(agent) == True):
            agent.adapt_behavior()
            return agent.behavior.execute(agent)
        best_ressource, tile_index = self.find_first_ressource(agent)
        command_list = self.get_command(agent, best_ressource, tile_index)
        if command_list:
            return command_list
        return ["Forward\n"]

    def find_first_ressource(self, agent):
        valid = {"food" : FOOD_TO_REACH, "linemate" : 9, "deraumere" : 8, "sibur" : 10, "mendiane" : 5, "phiras" : 6, "thystame" : 1}
        for index, tile in enumerate(agent.vision):
            for ressource in tile:
                if ressource in valid and agent.inventory[ressource] < valid[ressource]:
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
        msg_leader_id = math.inf
        for msg in agent.mailbox:
            if msg["sender_id"] == agent.agent_id:
                continue
            if (msg["action"] == "INCANTATION" and msg["level"] == agent.level and msg["team"] == agent.team_name):
                agent_sender_id = msg["sender_id"]
                if (msg_leader is None or agent_sender_id < msg_leader_id):
                    msg_leader = msg
                    msg_leader_id = agent_sender_id
        if (msg_leader):
            agent.leader_id = msg_leader["sender_id"]
            agent.direction_to_follow = msg_leader["direction"]
            agent.joining_incantation = True
            return True
        agent.joining_incantation = False
        return False