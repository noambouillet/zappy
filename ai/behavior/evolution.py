##
## EPITECH PROJECT, 2026
## zappy-mirror
## File description:
## evolution
##

from .class_behavior import Behavior
from constant import requirement_for_progress

class Evolution(Behavior):
    def execute(self, agent):
        """This function is to execute evolution class
        Args:
            agent (class): Agent IA
        Returns:
            str: command for the agent
        """
        print("\n========== EVOLUTION ==========")
        agent.tick += 1
        if (agent.vision == [[]]):
            print("vision empty -> Look")
            print("=============================\n")
            return ["Look\n"]
        join_commands = self.check_invocation_call(agent)
        if join_commands is not None:
            print("[EVOLUTION] Stop own invocation -> join teammate")
            print("=============================\n")
            return join_commands
        if (agent.eject_players == True):
            agent.eject_players = False
            return ["Eject\n"]
        condition = self.implementation_of_conditions(agent)
        if (condition != []):
            return condition
        agent.is_incantation = True
        print("Launch Incantation")
        print("=============================\n")
        return ["Incantation\n"]
    
    def check_invocation_call(self, agent):
        for mail in agent.mailbox:
            print("[EVOLUTION][MAIL] Read:", mail)
            if (mail["action"] != "INVOCATION") or (mail["level"] != agent.level) or (mail["team"] != agent.team_name):
                continue
            direction = mail["direction"]
            print("[EVOLUTION][MAIL] Same level invocation detected")
            print("[EVOLUTION][MAIL] Direction:", direction)
            if direction == 0:
                print("[EVOLUTION][MAIL] Already on invocation tile -> AVAILABLE + Look")
                agent.joining_invocation = False
                return [f"Broadcast AVAILABLE|{agent.level}|{agent.team_name}\n", "Look\n"]
            print("[EVOLUTION][MAIL] Join existing invocation instead of broadcasting mine")
            agent.joining_invocation = True
            return [f"Broadcast AVAILABLE|{agent.level}|{agent.team_name}\n"] + agent.follow_direction(direction) + ["Look\n"]
        return None
    
    def implementation_of_conditions(self, agent):
        """This function is implementation conditions
        Args:
            agent (class): Agent/IA
        """
        requirement = requirement_for_progress[agent.level - 1]
        nb_players = requirement["nb_players"]
        agent.teammate_same_level = 1
        nb_agent_comming = self.check_players_tile(agent)
        print(f"nb_players requis: {nb_players}")
        print(f"nb_players sur la case: {agent.teammate_same_level}")
        print(f"nb_players en route: {nb_agent_comming}")
        if (agent.teammate_same_level < nb_players):
            if (agent.unused_slots == 0 and (agent.teammate_same_level + nb_agent_comming) < nb_players):
                print("Fork")
                print("=============================\n")
                return ["Fork\n"]
            else:
                print("Broadcast INVOCATION")
                print("=============================\n")
                return [f"Broadcast INVOCATION|{agent.level}|{agent.team_name}\n"]
        setup = self.check_ressources_tile(agent, requirement)
        if (setup != ["Look\n"]):
            agent.prepare_incantation = True
            print("setup != Look")
            print("=============================\n")
            return setup
        agent.prepare_incantation = False
        return []
    
    def check_players_tile(self, agent):
        """This function is to check the allies or the ennemies on the tile of the Invocation
        Args:
            agent (class): Agent/IA
        """
        nb_agent_comming = 0
        for msg in agent.mailbox:
            if (msg["action"] == "AVAILABLE" and msg["level"] == agent.level and msg["team"] == agent.team_name and msg["direction"] == 0):
                agent.teammate_same_level += 1
                print("[EVOLUTION][MAIL] Teammate available on tile")
            elif (msg["action"] == "AVAILABLE" and msg["team"] != agent.team_name and msg["direction"] == 0):
                agent.eject_players = True
                print("[EVOLUTION][MAIL] Enemy detected on tile")
            elif (msg["action"] == "AVAILABLE" and msg["level"] == agent.level and msg["team"] == agent.team_name and msg["direction"] != 0):
                nb_agent_comming += 1
        agent.mailbox.clear()
        return nb_agent_comming
    
    def check_ressources_tile(self, agent, ressources_for_upgrade):
        """This function is to determinate, the ressources on a tile are necessary/useless 
        Args:
            agent (class): Agent/IA
        """
        list_command = []
        print("vision:", agent.vision)
        ressources_tiles = {"food" : 0, "linemate" : 0, "deraumere" : 0, "sibur" : 0, "mendiane" : 0, "phiras" : 0, "thystame" : 0}
        for element in agent.vision[0]:
            if (element != "player"):
                ressources_tiles[element] += 1
        print("ressources tiles", ressources_tiles)
        for ressource, nb_ressources in ressources_tiles.items():
            if (ressource != "nb_players"):
                print("number ressource", ressource, nb_ressources)
                if (ressource == "food" and nb_ressources > 0):
                    for _ in range(nb_ressources):
                        list_command += [f"Take food\n"]
                    print("list command", list_command)
                elif (ressource != "food" and nb_ressources > ressources_for_upgrade[ressource]):
                    for _ in range(nb_ressources - ressources_for_upgrade[ressource]):
                        list_command += [f"Take {ressource}\n"]
                    print("list command", list_command)
                elif (ressource != "food" and nb_ressources < ressources_for_upgrade[ressource]):
                    for _ in range(ressources_for_upgrade[ressource] - nb_ressources):
                        list_command += [f"Set {ressource}\n"]
                    print("list command", list_command)
        list_command += ["Look\n"]
        return list_command