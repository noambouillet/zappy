# Project Name

## Description

Brief description of the project (en anglais svp).

---

## Folder Structure

```plaintext
project-name
├── src/
│   └── ...                      # Source files
├── .github/
│   └── workflows/
│       └── safety-checks.yml    # Pipeline to check build and tests before merge
└── README.md
```

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

### Issues and Labels

GitHub issues are not used in this workflow.

GitHub labels are not used in this workflow.

## Setup and Running

### Install the dependencies

```sh
echo 'Our dependencies'
```

### Running

```sh
echo 'How to run it'
```

## Collaborators

Thanks to everyone who contributed:

[Collaborator's Name](https://github.com/name)  
[Collaborator's Name](https://github.com/name)  
[Collaborator's Name](https://github.com/name)  
