##
## EPITECH PROJECT, 2026
## zappy-mirror
## File description:
## Follower
##

from constant import AVAILABLE_FREQUENCE, MIN_FOOD, WAIT_MSG
from .class_behavior import Behavior

class Follower(Behavior):
    def execute(self, agent):
        agent.tick += 1
        print("FOLLOWER ", agent.tick)
        if agent.inventory["food"] < MIN_FOOD:
            agent.survive = True
            agent.joining_incantation = False
            agent.leader_id = None
            return []
        msg_leader = None
        msg_leader_tick = 0
        for msg in agent.mailbox:
            if (msg["action"] == "INCANTATION" and msg["sender_id"] == agent.leader_id):
                msg_tick = msg["tick"]
                if (msg_leader is None or msg_tick > msg_leader_tick):
                    msg_leader = msg
                    msg_leader_tick = msg_tick
        if (msg_leader is not None):
            print("FOLLOWER SEND", msg_leader["sender_id"])
            print("FOLLOWER DIRECTION", msg_leader["sender_id"])
            print("FOLLOWER TICK", msg_leader["sender_id"])
            print("FOLLOWER MAILBOX", agent.mailbox)
        agent.mailbox.clear()
        return self.verif_msg_leader(agent, msg_leader)
            
    def verif_msg_leader(self, agent, msg_leader):
        """_summary_

        Args:
            agent (_type_): _description_
        """
        if (msg_leader):
            agent.direction_to_follow = msg_leader["direction"]
            agent.leader_id = msg_leader["sender_id"]
            agent.joining_incantation = True
            if (agent.direction_to_follow != 0):
                return agent.follow_direction(agent.direction_to_follow)
            else:
                return [f"Broadcast AVAILABLE|{agent.level}|{agent.team_name}|{agent.tick}|{agent.agent_id}\n"]
        else:
            if ((agent.tick - agent.last_send_leader) > WAIT_MSG):
                agent.leader_id = None
                agent.last_send_leader = None
                agent.direction_to_follow = None
                agent.joining_incantation = False
                return []
            agent.last_send_leader = agent.tick
            agent.joining_incantation = True
            return agent.follow_direction(agent.direction_to_follow)
            