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
        self.survive = False
        self.list_commands = []
        self.level = 1
        self.direction = Direction.Up
        self.vision = [[]]
        self.unused_slots = 0
        self.elevation = False
        self.behavior = Evolution()
        self.size_map = (0, 0)
        self.team_name = ""
        self.teammate_same_level = 0
        self.sound_direction = -1

    def tile_to_coords(self, tile):
        """Convert tile from look to coordinates.
        y = number of forward to do in the current direction
        x = number of forward to do in right direction
        -x = number of forward to do in left direction

        Args:
            tile (int): direction from which the broadcast originates

        Returns:
            (x, y): direction to follow to go to the tile from the agent tile
        """
        if tile <= 0:
            return 0, 0
        row = 0
        total = -1
        while tile > total:
            total += (2 * row + 1)
            row += 1
        row -= 1
        x = -(total - tile - row)
        y = row
        return x, y

    def go_to(self, tile):
        """Return the list of commands to go to a tile from look
        """
        x, y = self.tile_to_coords(tile)
        commands = []
        for _ in range(y):
            commands.append("Forward\n")
        if x < 0:
            commands.append("Left\n")
            for _ in range(-x):
                commands.append("Forward\n")
        if x > 0:
            commands.append("Right\n")
            for _ in range(x):
                commands.append("Forward\n")
        return commands
    
    def follow_direction(self, direction: int):
        """deplace agent on the map by following the direction
        Args:
            direction (int): direction to go

        Returns:
            tab: all instructions to follow
        """
        commands = []
        up = [1, 2, 3]
        right = [5]
        down = [6, 7, 8]
        left = [4]
        current_direction: Direction = self.direction
        direction_to_follow: Direction = Direction.Undefined
        if (direction in up):
            direction_to_follow = Direction.Up
        if (direction in right):
            direction_to_follow = Direction.Right
        if (direction in down):
            direction_to_follow = Direction.Down
        if (direction in left):
            direction_to_follow = Direction.Left
        rotation = (direction_to_follow.value - current_direction.value) % 4
        if (rotation == 1):
            commands.append("Right\n")
        if (rotation == 2):
            commands.append("Right\n")
            commands.append("Right\n")
        if (rotation == 3):
            commands.append("Left\n")
        commands.append("Forward\n")
        return commands
    
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
        if (self.inventory["food"] < MIN_FOOD or self.survive == True):
            self.survive = True
            self.behavior = Survive()
        elif (self.capable_of_evolving() == False):
            self.behavior = Explore()
        else:
            self.behavior = Evolution()