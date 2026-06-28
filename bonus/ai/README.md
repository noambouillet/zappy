# Zappy (AI)

**Zappy** is a project from the module ***(G4-Year-End-Project-Zappy)***. The goal of this project is to create a network game where several teams confront each other on a tile map
containing resources. The winning team is the first one where at least 6 players reach the maximum elevation.

---

## Content

- [Summary](#summary)
- [Installation](#installation)
- [Project structure](#project-structure)
- [Authors](#authors)

---

## Summary

This README describes the artificial intelligence implemented in our game. It's based on a set of distinct behaviors:

- **Survive**: handles the player's basic survival needs, mainly managing food levels to avoid starvation.
- **Explore**: handles movement and discovery of the map, locating resources and other players.
- **Follower**: makes a player follow a leader's directives in order to regroup for an incantation.
- **Incantation**: handles the elevation ritual once enough players and resources are gathered on a tile.

Each of these behaviors has specific goals to make our AI completely autonomous and as efficient as possible in reaching level 8 as quickly as possible.

---

## Installation

### 1. Git clone the repository.

```bash
git clone git@github.com:EpitechPGE2-2025/G-YEP-400-LIL-4-1-zappy-6.git
cd G-YEP-400-LIL-4-1-zappy-6
cd ai
```

### 2. Launch the AI:

Several points:
- Replace the port number after -p with your server's default port number (4242). This argument is not mandatory.
- Replace the server's team name after -n with the server's team name. This argument is mandatory.
- Replace the server's hostname after -h with the server's hostname (localhost). This argument is not mandatory.

```bash
./zappy_ai_main [-p port] -n name [-h machine]
```

### 3.Bonus

#### 3.1 Create an environnment
```bash
python -m venv .venv
```
```bash
 source .venv/bin/activate 
```


#### 3.2 Install dependencies
```bash
pip install -r requirements.txt
```

#### 3.3 Launch streamlit
```bash
streamlit run streamlit.py
```

---

## Project structure

```bash  
.
├── agent.py
├── behavior
│   ├── class_behavior.py
│   ├── explore.py
│   ├── follower.py
│   ├── incantation.py
│   └── survive.py
├── commands_ia
│   ├── broadcast.py
│   ├── connect.py
│   ├── eject.py
│   ├── fork.py
│   ├── forward.py
│   ├── incantation.py
│   ├── inventory.py
│   ├── left.py
│   ├── look.py
│   ├── right.py
│   ├── set.py
│   └── take.py
├── commands_server
│   ├── dead.py
│   ├── ejects.py
│   └── messages.py
├── connection.py
├── constant.py
├── handle_commands.py
├── logger.py
├── Makefile
├── parsing.py
├── README.md
├── zappy_ai
└── zappy_ai.log

4 directories, 30 files
```

---

## Authors

- [Paul Ammeloot](https://github.com/PaulAmmeloot)
- [Louis Hector](https://github.com/MinFlag)