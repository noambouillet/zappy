##
## EPITECH PROJECT, 2026
## zappy-mirror
## File description:
## constant
##

from enum import Enum

MIN_FOOD = 10
FOOD_FOR_INCANTATION = 15
FOOD_FOR_FOLLOW = 10
INVOCATION_FREQUENCE = 10
FOOD_TO_REACH = 15

class Direction(Enum):
    Up = 0
    Right = 1
    Down = 2
    Left = 3
    Undefined = 4
    
requirement_for_progress = [
    {"nb_players" : 1, "linemate" : 1, "deraumere" : 0, "sibur" : 0, "mendiane" : 0, "phiras" : 0, "thystame" : 0},
    {"nb_players" : 2, "linemate" : 1, "deraumere" : 1, "sibur" : 1, "mendiane" : 0, "phiras" : 0, "thystame" : 0},
    {"nb_players" : 2, "linemate" : 2, "deraumere" : 0, "sibur" : 1, "mendiane" : 0, "phiras" : 2, "thystame" : 0},
    {"nb_players" : 4, "linemate" : 1, "deraumere" : 1, "sibur" : 2, "mendiane" : 0, "phiras" : 1, "thystame" : 0},
    {"nb_players" : 4, "linemate" : 1, "deraumere" : 2, "sibur" : 1, "mendiane" : 3, "phiras" : 0, "thystame" : 0},
    {"nb_players" : 6, "linemate" : 1, "deraumere" : 2, "sibur" : 3, "mendiane" : 0, "phiras" : 1, "thystame" : 0},
    {"nb_players" : 6, "linemate" : 2, "deraumere" : 2, "sibur" : 2, "mendiane" : 2, "phiras" : 2, "thystame" : 1}
]