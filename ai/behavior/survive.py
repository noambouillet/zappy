##
## EPITECH PROJECT, 2026
## zappy-mirror
## File description:
## survive
##

from .class_behavior import Behavior
from constant import FOOD_TO_REACH

class Survive(Behavior):
    def execute(self, agent):
        """This function is to execute survive class
        Args:
            agent (class): Agent IA
        Returns:
            str: command for the agent
        """
        print("\n========== SURVIVE ==========")
        agent.tick += 1
        if (agent.inventory["food"] >= FOOD_TO_REACH):
            agent.survive = False
            return []
        try:
            food_commands = agent.take_food_on_tile()   
            if food_commands:
                return food_commands
            for pos, infos in enumerate(agent.vision):
                if "food" in infos:
                    commands = agent.go_to(pos)
                    if agent.inventory["food"] <= 5 and agent.movement_cost(commands) > 2:
                        return ["Forward\n"]
                    return commands + ["Take food\n"]
        except (IndexError, TypeError):
            return ["Forward\n"]
        return ["Forward\n"]