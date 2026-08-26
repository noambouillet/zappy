# Zappy

**Zappy** is a project from the module ***(G4-Year-End-Project-Zappy)***. The goal of this project is to create a network game where several teams confront each other on a tile map
containing resources. The winning team is the first one where at least 6 players reach the maximum elevation.

## Content

- [Summary](#summary)
- [Installation](#installation)
- [CommitNorm](#commit-norm)
- [Branching](#branching)
- [Project structure](#project-structure)
- [Authors](#authors)

---

## Summary

Welcome to this README for the Zappy project. In this repository, you'll discover that our project has been divided into three main parts.

Therefore, we've decided to provide READMEs for each of these parts of the project, so you can easily find the section you're interested in. Happy reading!

### **GUI** (C++)
#### - [Graphical](gui/README.md)

### **SERVER** (C++)
#### - [Communication](server/README.md)

### **AI** (PYTHON)
#### - [Algorithm](ai/README.md)

### Features :


---

## Installation

### 1. Git clone the repository.

```bash
git clone git@github.com:EpitechPGE2-2025/G-YEP-400-LIL-4-1-zappy-6.git
cd G-YEP-400-LIL-4-1-zappy-6
cd ai
```

### 2. Install the dependencies

We invite you to go and see the installation section of each README associated with the parts in order to be able to install and understand how to launch each part.

If you want to see where the README files are:
- [Summary](#summary)

### 3. Example

Once everything is installed and you understand the principle of each part, here is an example of how to launch all the elements:

For this example, we decided to set the server port to 1025 and to use the team name RED, BLUE.

- Server
```bash
./zappy_server -p 1025 -n RED BLUE
```

- Gui
```bash
./zappy_gui -p 1025
```

- AI
```bash
./zappy_ai -p 1025 -n RED
```

---

## Branching

### Branch Naming

Each feature or subfeature is developed using a simple naming convention.

Use one of these formats:

```plaintext
feature-name
feature-name#subfeature-name
```

**Examples:**

- `network`
- `network#poll-loop`
- `gui`

This keeps the branch history easy to read without relying on GitHub issues.

### Merge Flow

All work is pushed to `dev` before `main`.

Recommended flow:

- create a branch from `main`
- develop and push the branch
- merge into `dev`
- validate on `dev`
- merge `dev` into `main`

---

## Commit Norm

We use a clear and consistent commit norm inspired by [Conventional Commits](https://www.conventionalcommits.org/en/v1.0.0/) :

```plaintext
<type>: <description>
```

### Types include:

- feat: A new feature
- fix: A bug fix
- docs: Documentation only changes
- refactor: Code changes that neither fix a bug nor add a feature
- build: Changes that affect the build system or external dependencies

Example:
```plaintext
feat: added a new feature
```

---

## Project structure

```bash
zappy
.
├── ai
│   ├── agent.py
│   ├── behavior
│   │   ├── class_behavior.py
│   │   ├── explore.py
│   │   ├── follower.py
│   │   ├── incantation.py
│   │   └── survive.py
│   ├── commands_ia
│   │   ├── broadcast.py
│   │   ├── connect.py
│   │   ├── eject.py
│   │   ├── fork.py
│   │   ├── forward.py
│   │   ├── incantation.py
│   │   ├── inventory.py
│   │   ├── left.py
│   │   ├── look.py
│   │   ├── right.py
│   │   ├── set.py
│   │   └── take.py
│   ├── commands_server
│   │   ├── dead.py
│   │   ├── ejects.py
│   │   └── messages.py
│   ├── connection.py
│   ├── constant.py
│   ├── handle_commands.py
│   ├── logger.py
│   ├── Makefile
│   ├── parsing.py
│   ├── README.md
│   ├── zappy_ai
│   └── zappy_ai.log
├── gui
│   ├── assets
│   │   └── images
│   │       ├── background.png
│   │       ├── benoit.png
│   │       ├── egg.png
│   │       ├── food.png
│   │       └── ground.png
│   ├── Makefile
│   └── zappy_gui
├── Makefile
├── README.md
├── server
│   └── Makefile
└── zappy_ref-v3.0.1
    ├── linux
    │   ├── log6.txt
    │   ├── zappy_gui.AppImage
    │   └── zappy_server
    ├── macos
    │   └── zappy_server
    ├── README.md
    └── zappy_server

12 directories, 46 files
```

---

## Authors

Thanks to everyone who contributed:

- [Paul Ammeloot](https://github.com/PaulAmmeloot)
- [Louis Hector](https://github.com/MinFlag)
- [Noam Bouillet](https://github.com/nomaisthere)
- [Keryan Pollet-druelle](https://github.com/Keryan-pollet)
- [Logan Capoen](https://github.com/logan-capoen)
