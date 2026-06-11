##
## EPITECH PROJECT, 2026
## zappy-mirror
## File description:
## explore
##

from .class_behavior import Behavior
from constant import requirement_for_progress

class Explore(Behavior):
    def execute(self, agent):
        """This function is to execute explore class
        Args:
            agent (class): Agent IA
        Returns:
            str: command for the agent
        """ 
        print("Explore")
        info_level_up = requirement_for_progress[agent.level - 1]
        name_stone = ""
        for cle, valeur in info_level_up.items():
            if (cle != "nb_players" and agent.inventory[cle] < valeur):
                name_stone = cle
                break
        print("Stone", name_stone)
        if (name_stone and name_stone in agent.vision[0]):
            return [f"Take {name_stone}\n"]
        elif (name_stone and name_stone not in agent.vision[0]):
            return ["Forward\n", "Look\n"]
        else:
            return ["Forward\n"]