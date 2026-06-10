##
## EPITECH PROJECT, 2026
## zappy-mirror
## File description:
## agent
##

from constant import Direction, MIN_FOOD, requirement_for_progress
from abc import ABC, abstractmethod

class Behavior(ABC):
    """Classe mère pour tous les comportements de l'IA."""
    @abstractmethod
    def execute(self, agent):
        pass

class Survive(Behavior):
    """This class is to force IA for survive with search food
    Args:
        Behavior (class): Mother class
    """
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
                return "Take food\n"
            for index, tab_elems in enumerate(agent.vision):
                print("Index ", index, "Tab_elems", tab_elems)
                if "food" in tab_elems:
                    return "Forward\n"
            return "Forward\n"
        except (TypeError, IndexError):
            return "Forward\n"
    
class Explore(Behavior):
    def execute(self, agent):
        """This function is to execute explore class
        Args:
            agent (class): Agent IA
        Returns:
            str: command for the agent
        """ 
        info_level_up = requirement_for_progress[agent.level - 1]
        print(info_level_up)
        name_stone = ""
        for cle, valeur in requirement_for_progress.items():
            if (cle != "nb_players" and agent.inventory[cle] < valeur):
                name_stone = cle
                break
        if (name_stone and name_stone in agent.vision[0]):
            return f"Take {name_stone}\n"
        elif (name_stone and name_stone not in agent.vision[0]):
            return ["Forward\n", "Look\n"]
        
        
class Evolution(Behavior):
    def execute(self, agent):
        """This function is to execute evolution class
        Args:
            agent (class): Agent IA
        Returns:
            str: command for the agent
        """
        return "Incantation\n"

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
        self.status = Survive()
    
    def check_all_ressources(self):
        """_summary_

        Returns:
            _type_: _description_
        """
        info_level_up = requirement_for_progress[self.level - 1]
        level_up_ok = True
        for cle, valeur in info_level_up.items():
            if (cle != "nb_players" and self.inventory[cle] < valeur):
                level_up_ok = False
                break
        if (level_up_ok == False):
            return False
        else:
            return True
    
    def change_status_client(self):
        """_summary_
        """
        if (self.inventory["food"] < MIN_FOOD):
            self.status = Survive()
        elif (self.check_all_ressources() == False):
            self.status = Explore()
        else:
            self.status = Evolution()