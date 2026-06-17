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
        print("[EXPLORE] Shared ressources:", agent.shared_ressources)
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
        shared_info, shared_cost = agent.get_best_shared_ressource(needed_ressources)
        shared_commands = self.get_shared_commands(agent, shared_info)
        broadcast_commands = self.broadcast_ressources(agent, needed_ressources, best_ressource, tile_index)
        print("[EXPLORE] Best ressource:", best_ressource)
        print("[EXPLORE] Tile index:", tile_index)
        print("[EXPLORE] Local commands:", local_commands)
        print("[EXPLORE] Shared info:", shared_info)
        print("[EXPLORE] Shared cost:", shared_cost)
        print("[EXPLORE] Shared commands:", shared_commands)
        chosen_commands = self.choose_best_commands(agent, local_commands, shared_commands)
        if chosen_commands:
            print("[EXPLORE] Chosen commands:", chosen_commands)
            print("=============================\n")
            return broadcast_commands + chosen_commands
        print("[EXPLORE] Nothing useful -> Forward + Look")
        print("=============================\n")
        return broadcast_commands + ["Forward\n", "Look\n"]

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
            return [f"Take {ressource}\n", "Take food\n", "Inventory\n", "Look\n"]
        return agent.go_to(tile) + [f"Take {ressource}\n", "Take food\n", "Inventory\n", "Look\n"]

    def get_shared_commands(self, agent, shared_info):
        if shared_info is None:
            return None
        direction = shared_info["direction"]
        if direction == 0:
            return ["Look\n"]
        return agent.follow_direction(direction) + ["Look\n"]

    def choose_best_commands(self, agent, local_commands, shared_commands):
        if local_commands is None and shared_commands is None:
            return None
        if local_commands is not None and shared_commands is None:
            print("[EXPLORE] Decision: local only")
            return local_commands
        if local_commands is None and shared_commands is not None:
            print("[EXPLORE] Decision: broadcast only")
            return shared_commands
        local_cost = agent.movement_cost(local_commands)
        shared_cost = agent.movement_cost(shared_commands)
        print("[EXPLORE] Local cost:", local_cost)
        print("[EXPLORE] Broadcast cost:", shared_cost)
        if shared_cost < local_cost:
            print("[EXPLORE] Decision: follow broadcast")
            return shared_commands
        print("[EXPLORE] Decision: take local ressource")
        return local_commands

    def broadcast_ressources(self, agent, needed_ressources, chosen_ressource, chosen_tile):
        return []
        commands = []
        already_sent = []
        for index, tile in enumerate(agent.vision):
            if index == 0:
                continue
            for ressource in tile:
                if (ressource == "player") or (ressource == "food") or (ressource not in agent.inventory) or (ressource in already_sent) or (ressource == chosen_ressource and index == chosen_tile):
                    continue
                command = f"Broadcast RES|{ressource}\n"
                print("[EXPLORE] Broadcast:", command.strip())
                commands.append(command)
                already_sent.append(ressource)
        return commands