##
## EPITECH PROJECT, 2026
## zappy-mirror
## File description:
## agent
##

from constant import Direction, MIN_FOOD, requirement_for_progress
from abc import ABC, abstractmethod

class Behavior(ABC):
    """Parent class for all AI behaviors."""
    @abstractmethod
    def execute(self, agent):
        pass

class Survive(Behavior):
    def execute(self, agent):
        """This function is to execute survive class
        Args:
            agent (class): Agent IA
        Returns:
            str: command for the agent
        """
        try:
            if "food" in agent.vision[0]:
                return ["Take food\n"]
            for index, tab_elems in enumerate(agent.vision):
                print("Index ", index, "Tab_elems", tab_elems)
                if "food" in tab_elems:
                    return ["Forward\n"]
            return ["Forward\n"]
        except (TypeError, IndexError):
            return ["Forward\n"]
    
class Explore(Behavior):
    def execute(self, agent):
        """This function is to execute explore class
        Args:
            agent (class): Agent IA
        Returns:
            str: command for the agent
        """ 
        info_level_up = requirement_for_progress[agent.level - 1]
        name_stone = ""
        for cle, valeur in info_level_up.items():
            if (cle != "nb_players" and agent.inventory[cle] < valeur):
                name_stone = cle
                break
        if (name_stone and name_stone in agent.vision[0]):
            return [f"Take {name_stone}\n"]
        elif (name_stone and name_stone not in agent.vision[0]):
            return ["Forward\n", "Look\n"]
        else:
            return ["Forward\n"]
        
        
class Evolution(Behavior):
    def execute(self, agent):
        """This function is to execute evolution class
        Args:
            agent (class): Agent IA
        Returns:
            str: command for the agent
        """
        return ["Incantation\n"]

class Agent:
    """This class is to define the drone/agent/ia
    """
    def __init__(self):
        self.alive = True
        self.inventory = {"food" : 10, "linemate" : 0, "deraumere" : 0, "sibur" : 0, "mendiane" : 0, "phiras": 0, "thystame" : 0}
        self.list_commands = []
        self.level = 1
        self.direction = Direction.North
        self.vision = [[]]
        self.unused_slots = 0
        self.elevation = False
        self.behavior = Survive()
    
    def capable_of_evolving(self):
        """This function is to determinate if the player can evolve with his ressources
        Returns:
            bool: true or false for level up
        """
        info_level_up = requirement_for_progress[self.level - 1]
        can_level_up = True
        for cle, valeur in info_level_up.items():
            if (cle != "nb_players" and self.inventory[cle] < valeur):
                can_level_up = False
                break
        return can_level_up
        
    def adapt_behavior(self):
        """This function is to adapt behavior during the life of the agent
        """
        if (self.inventory["food"] < MIN_FOOD):
            self.status = Survive()
        elif (self.capable_of_evolving() == False):
            self.status = Explore()
        else:
            self.status = Evolution()