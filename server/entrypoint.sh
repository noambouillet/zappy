#!/bin/sh

exec ./zappy_server \
    -p "$ZAPPY_PORT" \
    -x "$ZAPPY_WIDTH" \
    -y "$ZAPPY_HEIGHT" \
    -n Red Blue Green \
    -c "$ZAPPY_CLIENTS" \
    -f "$ZAPPY_FREQ"
