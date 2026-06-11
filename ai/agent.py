##
## EPITECH PROJECT, 2026
## zappy-mirror
## File description:
## agent
##

from constant import Direction, MIN_FOOD, requirement_for_progress
from behavior.survive import Survive
from behavior.explore import Explore
from behavior.evolution import Evolution
    
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
            print(self.inventory["food"])
            self.behavior = Survive()
        elif (self.capable_of_evolving() == False):
            self.behavior = Explore()
        else:
            self.behavior = Evolution()