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
    # print("[MESSAGE] Raw:", response_server)
    # try:
    #     header, content = response_server.split(",", 1)
    #     direction = int(header.split()[1])
    #     content = content.strip()
    # except (ValueError, IndexError):
    #     print("[MESSAGE] Invalid message format")
    #     return
    # if not content.startswith("RES|"):
    #     print("[MESSAGE] Ignored message:", content)
    #     return
    # parts = content.split("|")
    # if len(parts) != 2:
    #     print("[MESSAGE] Invalid RES message:", content)
    #     return
    # ressource = parts[1]
    # print("[MESSAGE] Ressource info received:", ressource)
    # print("[MESSAGE] Direction:", direction)
    # agent.add_shared_ressource(ressource, direction)

    response_clean = response_server.strip()
    response_split = response_clean.split(' ')
    direction = (int)(response_split[1].replace(',', ''))
    if (len(response_split) > 2):
        info_message = response_split[2].split('|')
        agent.mailbox.append({"action" : info_message[0], "direction" : direction, "level" : (int)(info_message[1]), "team" : info_message[2]})
    return
