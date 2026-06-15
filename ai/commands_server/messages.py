##
## EPITECH PROJECT, 2026
## zappy-mirror
## File description:
## messages
##

def receive_message(agent, response_server):
    """This function is for receiving messages from another AI"

    Args:
        agent (_type_): _description_
        response_server (_type_): _description_
    """
    response_clean = response_server.strip()
    response_split = response_clean.split(' ')
    direction = (int)(response_split[1].replace(',', ''))
    if (len(response_split) > 2):
        info_message = response_split[2].split('|')
        agent.mailbox.append({"action" : info_message[0], "direction" : direction, "level" : (int)(info_message[1]), "team" : info_message[2]})
    return
