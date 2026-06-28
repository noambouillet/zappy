##
## EPITECH PROJECT, 2026
## zappy-mirror
## File description:
## survive
##

from .class_behavior import Behavior
from constant import Macro
from logger import logger

class Survive(Behavior):
    def execute(self, agent):
        """This function is to execute survive class
        Args:
            agent (class): Agent IA
        Returns:
            str: command for the agent
        """
        logger.debug("\n========== SURVIVE ==========")
        agent.tick += 1
        agent.display_info()
        if (agent.start_game == False):
            if (agent.inventory["food"] >= agent.get_macro_ratio(Macro.FOOD_TO_REACH)):
                agent.survive = False
                agent.adapt_behavior()
                return agent.behavior.execute(agent)
            return self.take_food_vision(agent)
        else:
            if (agent.already_fork == False):
                return ["Fork\n"]
            if (agent.inventory["food"] >= agent.get_macro_ratio(Macro.FOOD_BEFORE_START)):
                agent.survive = False
                agent.start_game = False
                return agent.behavior.execute(agent)
            return self.take_food_vision(agent)

    def take_food_vision(self, agent):
        """This function is to take the food in the vision if is possible
        Args:
            agent (class): Agent
        """
        try:
            food_commands = agent.take_food_on_tile()
            if food_commands != []:
                return food_commands
            for pos, infos in enumerate(agent.vision):
                if "food" in infos:
                    commands = agent.go_to(pos)
                    if agent.inventory["food"] <= 5 and agent.movement_cost(commands) > 2:
                        return ["Forward\n"]
                    return commands + ["Take food\n"]
        except (IndexError, TypeError):
            return ["Forward\n"]
        return ["Forward\n"]