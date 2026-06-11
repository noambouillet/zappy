##
## EPITECH PROJECT, 2026
## zappy-mirror
## File description:
## survive
##

from .class_behavior import Behavior

class Survive(Behavior):
    def execute(self, agent):
        """This function is to execute survive class
        Args:
            agent (class): Agent IA
        Returns:
            str: command for the agent
        """
        print("Survive")
        try:
            if "food" in agent.vision[0]:
                return ["Take food\n"]
            for index, tab_elems in enumerate(agent.vision):
                if "food" in tab_elems:
                    return ["Forward\n"]
            return ["Forward\n"]
        except (TypeError, IndexError):
            return ["Forward\n"]