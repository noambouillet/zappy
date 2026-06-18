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
        print("\n========== EXPLORE ==========")
        agent.tick += 1
        print("[EXPLORE] Tick:", agent.tick)
        print("[EXPLORE] Level:", agent.level)
        print("[EXPLORE] Inventory:", agent.inventory)
        print("[EXPLORE] Vision:", agent.vision)
        call = agent.player_is_call()
        if (call is True):
            return ["Look\n"]
        if not agent.vision or agent.vision == [[]]:
            print("=============================\n")
            return ["Look\n"]
        food_commands = agent.take_food_on_tile()
        if food_commands:
            return food_commands
        needed_ressources = self.get_needed_ressources(agent)
        print("[EXPLORE] Needed ressources:", needed_ressources)
        best_ressource, tile_index = self.find_best_ressource(agent, needed_ressources)
        local_commands = self.get_local_commands(agent, best_ressource, tile_index)
        print("[EXPLORE] Best ressource:", best_ressource)
        print("[EXPLORE] Tile index:", tile_index)
        if local_commands:
            print("[EXPLORE] Chosen commands:", local_commands)
            print("=============================\n")
            return local_commands
        print("[EXPLORE] Nothing useful -> Forward + Look")
        print("=============================\n")
        return ["Forward\n", "Look\n"]

    def get_needed_ressources(self, agent):
        requirements = requirement_for_progress[agent.level - 1]
        needed_ressources = []
        print("[EXPLORE] Requirements:", requirements)
        for ressource, required_quantity in requirements.items():
            if ressource == "nb_players":
                continue
            current_quantity = agent.inventory[ressource]
            print("[EXPLORE] Check", ressource, "have:", current_quantity, "need:", required_quantity)
            if current_quantity < required_quantity:
                needed_ressources.append(ressource)
        return needed_ressources

    def find_best_ressource(self, agent, needed_ressources):
        for index, tile in enumerate(agent.vision):
            if agent.inventory["food"] <= 12 and index > 3:
                continue
            print("[EXPLORE] Tile", index, "contains:", tile)
            for ressource in tile:
                if ressource in needed_ressources:
                    print("[EXPLORE] Useful visible ressource:", ressource, "at tile:", index)
                    return ressource, index
        return None, None

    def get_local_commands(self, agent, ressource, tile):
        if ressource is None or tile is None:
            return None
        if tile == 0:
            return [f"Take {ressource}\n", "Take food\n", "Look\n"]
        return agent.go_to(tile) + [f"Take {ressource}\n", "Take food\n", "Look\n"]