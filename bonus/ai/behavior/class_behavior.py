##
## EPITECH PROJECT, 2026
## zappy-mirror
## File description:
## behavior
##

from abc import ABC, abstractmethod

class Behavior(ABC):
    """Parent class for all AI behaviors."""
    @abstractmethod
    def execute(self, agent):
        pass