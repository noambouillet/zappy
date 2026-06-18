##
## EPITECH PROJECT, 2026
## zappy-mirror
## File description:
## follower
##

from .class_behavior import Behavior
from constant import FOOD_FOR_FOLLOW

class Follower(Behavior):
    def execute(self, agent):
        """this function is to execute
        Args:
            agent (class): Agent/IA
        """
        print("[FOLLOWER]", agent.waiting)
        agent.tick += 1
        if agent.inventory["food"] <= FOOD_FOR_FOLLOW:
            agent.joining_invocation = False
            agent.waiting = False
            agent.survive = True
            return ["Look\n"]
        if agent.vision == [[]]:
            return ["Look\n"]
        food_commands = agent.take_food_on_tile()
        if food_commands:
            return food_commands
        invocations_same_team = []
        for index, msg in enumerate(agent.mailbox):
            if (msg.get("action") == "INVOCATION" and msg.get("level") == agent.level and msg.get("team") == agent.team_name):
                invocations_same_team.append((index, msg))
        if not invocations_same_team:
            if (agent.waiting == True):
                return [f"Broadcast AVAILABLE|{agent.level}|{agent.team_name}|{agent.agent_id}\n"] + ["Look\n"]
            agent.joining_invocation = False
            agent.waiting = False
            return ["Look\n"]
        _, recent_msg = min(invocations_same_team, key = lambda item: item[1].get("sender_id", 999999999))
        new_mailbox = []
        for msg in agent.mailbox:
            if (msg.get("action") == "INVOCATION" and msg.get("level") == agent.level and msg.get("team") == agent.team_name):
                continue
            new_mailbox.append(msg)
        agent.mailbox = new_mailbox.copy()
        direction = recent_msg["direction"]
        if (direction == 0):
            agent.joining_invocation = False
            agent.waiting = True
            return [f"Broadcast AVAILABLE|{agent.level}|{agent.team_name}|{agent.agent_id}\n"] + ["Look\n"]
        else:
            return [f"Broadcast AVAILABLE|{agent.level}|{agent.team_name}|{agent.agent_id}\n"] + agent.follow_direction(direction) + ["Look\n"]