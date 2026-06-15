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
    print(response_server.strip())
    response_split = response_server.split(' ')
    if (len(response_split) > 2):
        info_message = response_split[2].split('|')
        if (info_message[0] == "INVOCATION"):
            agent.elevation = True
            if (agent.level == info_message[1]):
                agent.info_invocation = True
    return
