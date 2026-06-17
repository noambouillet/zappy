##
## EPITECH PROJECT, 2026
## zappy-mirror
## File description:
## follower
##

from .class_behavior import Behavior

class Follower(Behavior):
    def execute(self, agent):
        """this function is to execute
        Args:
            agent (class): Agent/IA
        """
        print("[FOLLOWER]", agent.waiting)
        agent.tick += 1
        if agent.vision == [[]]:
            return ["Look\n"]
        food_commands = agent.take_food_on_tile()
        if food_commands:
            return food_commands
        if (agent.waiting == True):
            return [f"Broadcast AVAILABLE|{agent.level}|{agent.team_name}\n"] + ["Inventory\n", "Look\n"]
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
            if (msg.get("action") == "INVOCATION" and msg.get("level") == agent.level and msg.get("team") == agent.team_name):
                continue
            new_mailbox.append(msg)
        agent.mailbox = new_mailbox.copy()
        direction = recent_msg["direction"]
        if (direction == 0):
            agent.joining_invocation = False
            agent.waiting = True
            return [f"Broadcast AVAILABLE|{agent.level}|{agent.team_name}\n"] + ["Inventory\n", "Look\n"]
        else:
            return [f"Broadcast AVAILABLE|{agent.level}|{agent.team_name}\n"] + agent.follow_direction(direction) + ["Inventory\n", "Look\n"]