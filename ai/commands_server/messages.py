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

    Example:
    message 4, RES|linemate

    Args:
        agent (_type_): _description_
        response_server (_type_): _description_
    """
    print("[MESSAGE] Raw:", response_server)
    try:
        header, content = response_server.split(",", 1)
        direction = int(header.split()[1])
        content = content.strip()
    except (ValueError, IndexError):
        print("[MESSAGE] Invalid message format")
        return
    parts = content.split("|")
    if len(parts) < 2:
        print("[MESSAGE] Invalid content:", content)
        return
    if parts[0] in ["AVAILABLE", "INCANTATION"]:
        if len(parts) < 3:
            print("[MESSAGE] Invalid incantation message:", content)
            return
        try:
            level = int(parts[1])
        except ValueError:
            print("[MESSAGE] Invalid level:", parts[1])
            return
        sender_id = None
        if len(parts) >= 4:
            try:
                sender_id = int(parts[3])
            except ValueError:
                sender_id = None
        agent.mailbox.append({"action": parts[0], "direction": direction, "level": level, "team": parts[2], "tick": agent.tick, "sender_id": sender_id})
        return
    print("[MESSAGE] Ignored message:", content)