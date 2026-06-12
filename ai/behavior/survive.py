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
        if (agent.vision == [[]]):
            return ["Look\n"]
        try:
            if ("food" in agent.vision[0]):
                return ["Take food\n", "Look\n"]
            for pos, infos in enumerate(agent.vision):
                if ("food" in infos):
                    return agent.go_to(pos) + ["Take food\n", "Look\n"]
        except (IndexError, TypeError):
            return ["Forward\n"]
        return ["Forward\n", "Look\n"]