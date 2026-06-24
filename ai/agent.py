##
## EPITECH PROJECT, 2026
## zappy-mirror
## File description:
## agent
##

from constant import Direction, MIN_FOOD, requirement_for_progress, FOOD_TO_REACH
from behavior.survive import Survive
from behavior.explore import Explore
from behavior.incantation import Incantation
from behavior.follower import Follower
import os
from logger import logger

class Agent:
    """This class is to define the drone/agent/ia
    """
    def __init__(self):
        self.agent_id = os.getpid()
        self.team_name = ""
        self.list_commands = []
        self.tab_id_teammate = []
        self.mailbox = []
        self.vision = [[]]
        self.inventory = {"food" : 10, "linemate" : 0, "deraumere" : 0, "sibur" : 0, "mendiane" : 0, "phiras": 0, "thystame" : 0}
        self.start_game = True
        self.survive = True
        self.eject_players = False
        self.joining_incantation = False
        self.prepare_incantation = False
        self.is_incantation = False
        self.send_available = False
        self.already_fork = False
        self.direction = Direction.Up
        self.behavior = Survive()
        self.size_map = (0, 0)
        self.unused_slots = 0
        self.tick = 0
        self.last_inventory = 0
        self.last_send_leader = 0
        self.leader_id = 0
        self.direction_to_follow = 0
        self.level = 1
        self.teammate_on_tile = 1

    def display_info(self):
        logger.debug("AGENT INFO:")
        logger.debug("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~")
        logger.debug(f"agent_id {self.agent_id}")
        logger.debug(f"team_name {self.team_name}")
        logger.debug(f"list_commands {self.list_commands}")
        logger.debug(f"mailbox {self.mailbox}")
        logger.debug(f"vision {self.vision}")
        logger.debug(f"inventory {self.inventory}")
        logger.debug(f"eject_players {self.eject_players}")
        logger.debug(f"joining_incantation {self.joining_incantation}")
        logger.debug(f"prepare_incantation {self.prepare_incantation}")
        logger.debug(f"is_incantation {self.is_incantation}")
        logger.debug(f"direction {self.direction}")
        logger.debug(f"behavior {self.behavior}")
        logger.debug(f"size_map {self.size_map}")
        logger.debug(f"unused_slots {self.unused_slots}")
        logger.debug(f"tick {self.tick}")
        logger.debug(f"last_inventory {self.last_inventory}")
        logger.debug(f"last_send_leader {self.last_send_leader}")
        logger.debug(f"leader_id {self.leader_id}")
        logger.debug(f"direction_to_follow {self.direction_to_follow}")
        logger.debug(f"level {self.level}")
        logger.debug(f"teammate_on_tile {self.teammate_on_tile}")
        logger.debug("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~")

    def take_food_on_tile(self):
        """take all food available on current tile

        Returns:
            list: commands list
        """
        commands = []
        if not self.vision or self.vision == [[]]:
            return commands
        if len(self.vision) == 0:
            return commands
        if self.inventory["food"] >= FOOD_TO_REACH + 5:
            return commands
        for elem in self.vision[0]:
            if elem == "food":
                commands.append("Take food\n")
        return commands

    def tile_to_coords(self, tile):
        """Convert tile from look to coordinates.
        y = number of forward to do in the current direction
        x = number of forward to do in right direction
        -x = number of forward to do in left direction

        Args:
            tile (int): direction from which the broadcast originates

        Returns:
            (x, y): direction to follow to go to the tile from the view
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

        Args:
            tile (int): tile index to go

        Returns:
            list: commands list
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
        """follow direction from a broadcast (0 to 8)

        Args:
            direction (int): direction to follow (0 to 8)

        Returns:
            list: command list
        """
        logger.debug(f"[FOLLOW_DIRECTION] Broadcast direction: {direction}")
        if direction == 0:
            return []
        if direction in [2, 1, 8]:
            return ["Forward\n"]
        if direction in [3]:
            return ["Left\n"]
        if direction in [7]:
            return ["Right\n"]
        if direction in [4, 5, 6]:
            return ["Right\n", "Right\n"]
        return []
    
    def movement_cost(self, commands):
        """calculate the cost of a movement command list

        Args:
            commands (list): list of command

        Returns:
            int: cost of movement
        """
        cost = 0
        for command in commands:
            if command in ["Forward\n", "Left\n", "Right\n"]:
                cost += 1
        return cost
    
    def capable_of_incantation(self):
        """This function is to determinate if the player can do incantation with his ressources

        Returns:
            bool: true or false for level up
        """
        level = self.level - 1
        if (level >= 7):
            level = 6
        info_level_up = requirement_for_progress[level]
        can_level_up = True
        for item, valeur in info_level_up.items():
            if (item != "nb_players" and self.inventory[item] < valeur):
                can_level_up = False
                break
        return can_level_up

    def adapt_behavior(self):
        """This function is to adapt behavior during the life of the agent
        """
        if self.is_incantation == True:
            self.behavior = Incantation()
            return
        if (self.inventory["food"] <= MIN_FOOD or self.survive == True) and self.joining_incantation == False:
            self.survive = True
            self.joining_incantation = False
            self.prepare_incantation = False
            self.behavior = Survive()
            return
        if self.joining_incantation == True:
            self.behavior = Follower()
            return
        if self.prepare_incantation == True:
            self.behavior = Incantation()
            return
        if self.capable_of_incantation() == False:
            self.behavior = Explore()
        else:
            self.behavior = Incantation()
    
