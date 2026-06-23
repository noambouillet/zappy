##
## EPITECH PROJECT, 2026
## zappy-mirror
## File description:
## Follower
##

from constant import MIN_FOOD, WAIT_MSG
from .class_behavior import Behavior
from logger import logger

class Follower(Behavior):
            
    def execute(self, agent):
        logger.debug("\n========== FOLLOWER ==========")
        agent.tick += 1
        agent.display_info()
        if agent.inventory["food"] < MIN_FOOD:
            agent.survive = True
            agent.joining_incantation = False
            agent.leader_id = None
            return []
        msg_leader = None
        for msg in reversed(agent.mailbox):
            if (msg["sender_id"] == agent.agent_id):
                continue
            if (msg["action"] == "INCANTATION" and msg["sender_id"] == agent.leader_id):
                msg_leader = msg
                break
        agent.mailbox = []
        return self.verif_msg_leader(agent, msg_leader)
            
    def verif_msg_leader(self, agent, msg_leader):
        """This function is to check if we have a message leader in our mailbox
        Args:
            agent (class): IA
            msg_leader (str): msg
        """
        if (not msg_leader):
            if ((agent.tick - agent.last_send_leader) > WAIT_MSG):
                agent.joining_incantation = False
                agent.leader_id = None
                agent.direction_to_follow = 0
                agent.last_send_leader = agent.tick
                return ["Inventory\n"]
            return ["Inventory\n"]
        else:
            agent.joining_incantation = True
            agent.leader_id = msg_leader["sender_id"]
            agent.direction_to_follow = msg_leader["direction"]
            agent.last_send_leader = agent.tick
            if (agent.direction_to_follow != 0):
                return agent.follow_direction(agent.direction_to_follow)
            else:
                return [f"Broadcast AVAILABLE|{agent.level}|{agent.team_name}|{agent.tick}|{agent.agent_id}\n"]