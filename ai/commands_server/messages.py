##
## EPITECH PROJECT, 2026
## zappy-mirror
## File description:
## messages
##

def receive_message(agent, response_server):
    """This function is for receiving messages from another AI
    
    Expected format from server:
    message K, text

    Args:
        agent (_type_): _description_
        response_server (_type_): _description_
    """
    try:
        header, content = response_server.split(",", 1)
        direction = int(header.split()[1])
        content = content.strip()
    except (ValueError, IndexError):
        return
    parts = content.split("|")
    if parts[0] in ["AVAILABLE", "INCANTATION", "READY", "ALIVE?", "LEAVING"]:
        try:
            action = parts[0]
            level = parts[1]
            team = parts[2]
            tick = parts[3]
            sender_id = parts[4]
            agent.mailbox.append({"action": action, "direction": (int)(direction), "level": (int)(level), "team": team, "tick": (int)(tick), "sender_id": (int)(sender_id)})
        except ValueError:
            return