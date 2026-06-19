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
        if agent.inventory["food"] < MIN_FOOD:
            agent.survive = True
            agent.joining_incantation = False
            agent.leader_id = None
            return []
        mail_incantation_same_team = []
        for index, msg in enumerate(agent.mailbox):
            if (msg["action"] == "INCANTATION" and msg["level"] == agent.level and msg["team"] == agent.team_name):
                mail_incantation_same_team.append((index, msg))
        if (not mail_incantation_same_team):
            if ((agent.tick - agent.last_send_leader) > WAIT_MSG):
                agent.last_send_leader = agent.tick
                agent.leader_id = None
                agent.joining_incantation = False
                return []
            else:
                return agent.follow_direction(agent.direction_to_follow)
        for _, msg in mail_incantation_same_team:
            if (msg["sender_id"] == agent.leader_id):
                direction = msg["direction"]
                agent.last_send_leader = agent.tick
                agent.direction_to_follow = direction
                if (direction == 0):
                    return [f"Broadcast AVAILABLE|{agent.level}|{agent.team_name}|{agent.tick}|{agent.agent_id}\n"]
                else:
                    return agent.follow_direction(agent.direction_to_follow)
        new_mailbox = []
        for msg in agent.mailbox:
            if (msg["action"] != "INCANTATION" and msg["action"] != "AVAILABLE"):
                new_mailbox.append(msg)
        agent.mailbox = new_mailbox.copy()
        if ((agent.tick - agent.last_send_leader) > WAIT_MSG):
            agent.last_send_leader = agent.tick
            agent.leader_id = None
            agent.joining_incantation = False
            return []
        return []