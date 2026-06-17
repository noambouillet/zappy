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
        if (agent.vision == [[]]):
            return ["Look\n"]
        try:
            if ("food" in agent.vision[0]):
                return ["Take food\n", "Look\n", "Inventory\n"]
            for pos, infos in enumerate(agent.vision):
                if ("food" in infos):
                    return agent.go_to(pos) + ["Take food\n", "Look\n", "Inventory\n"]
        except (IndexError, TypeError):
            return ["Forward\n"]
        return ["Forward\n", "Look\n"]