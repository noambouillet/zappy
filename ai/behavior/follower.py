##
## EPITECH PROJECT, 2026
## zappy-mirror
## File description:
## follower
##

from .class_behavior import Behavior
from constant import requirement_for_progress

class Follower(Behavior):
    def execute(self, agent):
        """this function is to execute
        Args:
            agent (class): Agent/IA
        """
        print("Follower")
        print(agent.waiting)
        if (agent.waiting == True):
            return [f"Broadcast AVAILABLE|{agent.level}|{agent.team_name}\n"]
        invocations_same_team = []
        for index, msg in enumerate(agent.mailbox):
            if (msg["action"] == "INVOCATION" and msg["level"] == agent.level and msg["team"] == agent.team_name):
                invocations_same_team.append((index, msg))
        if not invocations_same_team:
            agent.joining_invocation = False
            return ["Look\n"]
        _, recent_msg = max(invocations_same_team, key = lambda tuple : tuple[1]["tick"])
        new_mailbox = []
        for msg in agent.mailbox:
            if (msg["action"] != "INVOCATION"):
                new_mailbox.append(msg)
        agent.mailbox = new_mailbox.copy()
        direction = recent_msg["direction"]
        if (direction == 0):
            agent.joining_invocation = False
            agent.waiting = True
            return [f"Broadcast AVAILABLE|{agent.level}|{agent.team_name}\n"] + ["Look\n"]
        else:
            return [f"Broadcast AVAILABLE|{agent.level}|{agent.team_name}\n"] + agent.follow_direction(direction) + ["Look\n"]