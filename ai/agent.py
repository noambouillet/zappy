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
from behavior.follower import Follower
import os
    
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
        self.prepare_incantation = False
        self.is_incantation = False
        self.behavior = Explore()
        self.size_map = (0, 0)
        self.team_name = ""
        self.tick = 0
        self.teammate_same_level = 1
        self.mailbox = []
        self.eject_players = False
        self.joining_invocation = False
        self.waiting = False
        self.agent_id = os.getpid()
        self.last_inventory = 0
        self.last_available = 0
        self.last_incantation = 0

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
        for elem in self.vision[0]:
            if elem == "food":
                commands.append("Take food\n")
        if commands:
            commands.append("Inventory\n")
            commands.append("Look\n")
        return commands

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
        print("[FOLLOW_DIRECTION] Broadcast direction:", direction)
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
    
    def broadcast_cost(self, direction):
        """caluclate the cost to follow a broadcast direction

        Args:
            direction (list): list of command

        Returns:
            int: cost of follow direction
        """
        return self.movement_cost(self.follow_direction(direction))
    
    def add_shared_ressource(self, ressource, direction):
        """stock info received from broadcast

        Args:
            ressource (string): ressource type
            direction (int): direction to follow
        """
        if ressource not in self.inventory:
            return
        for info in self.shared_ressources:
            if info["ressource"] == ressource and info["direction"] == direction:
                info["tick"] = self.tick
                print("[BROADCAST MEMORY] Refresh:", info)
                return
        info = {"ressource": ressource, "direction": direction, "tick": self.tick}
        self.shared_ressources.append(info)
        print("[BROADCAST MEMORY] Add:", info)

    def clean_shared_ressources(self, max_time = 5):
        """delete old broadcast information

        Args:
            max_time (int, optional): max tick number before removing info from shared info. Defaults to 5.
        """
        clean_ressources = []
        for info in self.shared_ressources:
            time = self.tick - info["tick"]
            if time <= max_time:
                clean_ressources.append(info)
            else:
                print("[BROADCAST MEMORY] Remove old info:", info)
        self.shared_ressources = clean_ressources

    def get_best_shared_ressource(self, needed_ressources):
        self.clean_shared_ressources()
        best_info = None
        best_cost = None
        for info in self.shared_ressources:
            if info["ressource"] not in needed_ressources:
                continue
            cost = self.broadcast_cost(info["direction"])
            print("[BROADCAST MEMORY] Candidate:", info, "cost:", cost)
            if best_cost is None or cost < best_cost:
                best_cost = cost
                best_info = info
        return best_info, best_cost
    
    def capable_of_evolving(self):
        """This function is to determinate if the player can evolve with his ressources
        Returns:
            bool: true or false for level up
        """
        info_level_up = requirement_for_progress[self.level - 1]
        can_level_up = True
        for item, valeur in info_level_up.items():
            if (item != "nb_players" and self.inventory[item] < valeur):
                can_level_up = False
                break
        return can_level_up
    
    def player_is_call(self):
        """Check if another player is calling an invocation.
        If cannot evolve, join any valid leader.
        If can evolve too, only join the leader with smaller id."""
        for msg in self.mailbox:
            if (msg.get("action") == "INVOCATION" and msg.get("level") == self.level and msg.get("team") == self.team_name):
                sender_id = msg.get("sender_id")
                if sender_id == self.agent_id:
                    continue
                if self.capable_of_evolving() == False:
                    self.joining_invocation = True
                    return True
                if sender_id is not None and sender_id < self.agent_id:
                    self.joining_invocation = True
                    return True
        return False

    def adapt_behavior(self):
        """This function is to adapt behavior during the life of the agent
        """
        if self.is_incantation == True:
            self.behavior = Evolution()
            return
        if self.inventory["food"] <= MIN_FOOD or self.survive == True:
            self.survive = True
            self.joining_invocation = False
            self.waiting = False
            self.prepare_incantation = False
            self.behavior = Survive()
            return
        if self.joining_invocation == True or self.waiting == True or self.player_is_call() == True:
            self.behavior = Follower()
            return
        if self.prepare_incantation == True:
            self.behavior = Evolution()
            return
        if self.capable_of_evolving() == False:
            self.behavior = Explore()
            return
        self.behavior = Evolution()