# Zappy Server

This is the Server component for the Zappy project. It handles all the game logic, client connections (AI and GUI), and time-based events.

## Architecture & Code Structure

The server uses a modular architecture, split logically into different responsibility areas:

- `src/GameLoop/`: Contains the core server loop (`Server.cpp`), client representations (`Client.cpp`, `PlayerData.cpp`), map management (`Map.cpp`, `Tile.hpp`), and egg handling.
- `src/Commands/`: Contains the command dispatchers and logic. `AiCommands.cpp` handles actions triggered by the AI players (moving, taking, incantation). `GuiCommands.cpp` handles actions triggered by the graphical interface (querying map state, receiving events).
- `src/Encapsulations/`: RAII-compliant wrappers over low-level C network calls, ensuring safety and preventing resource leaks. Contains `Socket.cpp` and `Poll.cpp`.
- `src/Parsing/`: Handles command-line argument parsing (`ServerData.cpp`) and the internal server shell / stdin commands (`InputParser.cpp`).
- `src/Logger.cpp`: A custom singleton logger implementation responsible for unified output to `stdout` and a local `.log` file.

### Design Patterns Implemented

- **Singleton:** The `Logger` is implemented as a global singleton (`extern Logger logger;`), meaning there is exactly one unified logging instance across the whole server runtime.
- **Encapsulation / RAII:** Network resources (Sockets and Poll structures) are encapsulated inside `Socket` and `Poll` classes. In the event of a failure, their destructors safely release file descriptors, and custom major/minor exceptions cleanly unwind the stack.
- **Command Dispatch Pattern (Strategy):** Commands (both AI and GUI) are dispatched using `std::unordered_map` linking command string names to member function handlers (`AiDispatch` map). This ensures fast `O(1)` dispatching without heavy `if/else` ladders.

## How to build and start the server

You can build the server from the root of the project using the main Makefile:

```bash
make zappy_server
```

Or by running `make` directly in the `server/` directory.

### Usage

```bash
./zappy_server -p <port> -x <width> -y <height> -n <team1> [<team2> ...] -c <clientsNb> -f <freq> [OPTIONS]
```

- `-p, --port` : Server port (1024-65535)
- `-x, --width` : World width (10-42)
- `-y, --height` : World height (10-42)
- `-n, --teams` : Team names (at least one required)
- `-c, --clientnbr` : Max clients per team at start (1-200)
- `-f, --freq` : Time unit frequency (1-10000)

**Bonus Options:**
- `-s, --seed <seed>` : Provide a strict positive integer to seed the internal random generation algorithm (Map generation, random item placements, spawning locations).
- `-v, --verbose` : Enables verbose logging (displays `[INFO]` and `[WARN]` logs). If not provided, the server remains quiet on these channels.

## Commands

### AI Commands

The AI interacts with the server through text commands sent over TCP.
- `Forward`, `Right`, `Left`: Movement commands.
- `Look`: See tiles in front of the player up to their vision range.
- `Inventory`: List held resources.
- `Take <object>`, `Set <object>`: Interact with resources on the tile.
- `Connect_nbr`: See how many slots are left on the team.
- `Fork`: Lay an egg to open a new team slot.
- `Eject`: Push all players on the tile in the direction the pusher is facing.
- `Broadcast <text>`: Shout a message across the map (heard with sound source direction).
- `Incantation`: Start a ritual to level up. Requires specific resources and player counts on the same tile.

### GUI Commands

The GUI uses a specific binary/text protocol to stay synchronized:
- `msz`: Map size
- `bct <X> <Y>`: Content of a tile
- `mct`: Content of all tiles
- `tna`: Team names
- `ppo <n>`, `plv <n>`, `pin <n>`: Player position, level, and inventory
- Various server-side broadcast events like `pnw` (new player), `pex` (player ejected), `pbc` (broadcast), `pic` (incantation start), etc.

### Stdin / Server Shell Commands (Bonus)

The server listens to `stdin` to allow real-time control by the administrator:
- `/help`: Lists all available shell commands.
- `/stop`: Properly closes the server, safely disconnecting all clients.
- `/pause`: Pauses the game loop (ticks are not computed).
- `/resume`: Resumes the game loop.
- `/setFreq <freq>`: Dynamically changes the game frequency on the fly.
- `/clients`: Prints detailed information about all connected users (FD, Team, Position, Level).
- `/map`: Prints a text representation of the map and resource counts.
- `/kill <fd>`: Murders a specific AI client by forcing its disconnection.

## Win Condition

The server evaluates the win condition at the end of every successful `Incantation`.
The winning team is the first one where **at least 6 players reach the maximum elevation (level 8)**.
When this happens, the server broadcasts a `seg <team_name>` to the GUI, logs the victory, and gracefully halts the simulation.

## Bonus Features
1. **Dynamic Interactive Shell:** A fully integrated `stdin` shell to monitor the server, change frequency mid-game, or pause execution.
2. **Deterministic Seed (`--seed`):** Fully reproducible map and spawn generation for testing purposes.
3. **Verbose Logging (`-v` / `--verbose`):** A custom built Logger singleton that captures time-stamped logs, routes them dynamically based on the verbose flag, and outputs safely to both standard output and a `server.log` file.
4. **Resilient Exception Handling:** Separation of `MinorServerException` and `MajorServerException` ensuring the server survives unexpected client errors (like disconnected sockets) without crashing.
