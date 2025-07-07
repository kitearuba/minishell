# 🐚 Minishell_bsamy

![Status](https://img.shields.io/badge/status-in%20progress-yellow)
![Language](https://img.shields.io/badge/language-C-blue)
![License](https://img.shields.io/badge/license-42%20School-blueviolet)
![School](https://img.shields.io/badge/42-Barcelona-black)

> A minimalist Bash-like shell built from scratch for the 42 School curriculum.  
> Collaborative project by `chrrodri` and `bsamy`.

---

⚠️ **Notice:**  
This repository is **a work in progress** and **temporarily public** **only** to allow collaborator access during development.  
It is **not a reference implementation** and will be cleaned or made private once completed. Please do not clone or fork this code unless authorized.

---

## 📚 Table of Contents

- [⚙️ Project Overview](#️-project-overview)
- [🧭 Project Architecture & Flow (for Collaborators)](#-project-architecture--flow-for-collaborators)
- [📅 Development Status *(Last Updated: July 2025)*](#-development-status-last-updated-july-2025)
- [🔜 Upcoming Tasks](#-upcoming-tasks)
- [👥 Contributors](#-contributors)
- [📁 Repository Structure](#-repository-structure)
- [📌 Quickstart for Bsamy](#-quickstart-for-bsamy)
- [📌 Disclaimer](#-disclaimer)

---

## ⚙️ Project Overview

**Minishell** is a simple UNIX shell clone built in C as part of the 42 School curriculum. It replicates basic Bash behavior:

- Execute built-in and external commands
- Handle redirection: `<`, `>`, `>>`, `<<`
- Manage pipes (`|`)
- Expand environment variables (`$VAR`, `$?`)
- Interactive shell prompt using `readline`
- Maintain and propagate exit status codes

Key learning outcomes:
- **Process forking**
- **Signal handling**
- **File descriptor redirection**
- **Lexical and syntax parsing**
- **Memory and environment management**

---

## 🧭 Project Architecture & Flow (for Collaborators)

This section explains how the Minishell is structured and how the components interact. This helps onboard new contributors quickly.

### 🔹 1. `main()` — The Shell Loop

- File: `minishell.c`
- Infinite loop with `readline()`
- Handles cleanup, user input, and shell exit

```c
line = readline("minishell $> ");
tokens = tokenize_input(line);
tokens = expand_env_vars(tokens, bash);
cmds = parse_tokens(tokens);
````

---

### 🔹 2. `tokenize_input()` — Lexer

* File: `src/initiation/tokenize.c`
* Splits the user input into tokens (words, pipes, etc.)
* Outputs a `t_token *` linked list

Supports token types:

* `WORD`, `PIPE`, `REDIRECT`, `SINGLE_QUOTE`, `DOUBLE_QUOTE`, `ENV_VAR`

Example:

```
echo $USER | grep world
→ [WORD: "echo"] → [ENV_VAR: "$USER"] → [PIPE: "|"] → [WORD: "grep"] → [WORD: "world"]
```

---

### 🔹 3. `expand_env_vars()` — Environment Expansion

* File: `src/env/expand.c`

* Replaces tokens of type `ENV_VAR` with `WORD` after expanding:

  * `$VAR` → value from `bash->env`
  * `$?`   → value of `bash->exit_status`

* Called after tokenization and before parsing.

---

### 🔹 4. `parse_tokens()` — Parser

* File: `src/initiation/parser.c`
* Builds `t_command` linked list from tokens
* Supports command pipelining via `next`

```c
t_command:
  argv: {"ls", "-la"}
  next → t_command:
    argv: {"grep", "txt"}
```

---

### 🔹 5. `execute_command()` — Command Dispatcher

* File: `src/cmd/execute_commands.c`
* Routes commands to:

  * Built-ins: `cd`, `exit`, etc.
  * External: calls `execve()` via `run_external_cmd.c`

---

### 🔹 6. Shell State (`t_bash`)

* Central struct passed to all modules
* Stores:

  * `char **env` (custom environment)
  * `int exit_status` (last command result)

---

### 🔹 7. Utilities

* Located in `src/utils`
* `free_tokens()` and `free_2d_array()` for memory management
* `ft_getenv()` replaces `getenv()` for controlled env access

---

## 📅 Development Status *(Last Updated: July 2025)*

### ✅ Completed

* [x] Makefile and main shell structure
* [x] Custom shell state with `t_bash`
* [x] Tokenizer for basic commands + pipes + quotes
* [x] Parser (`t_token → t_command`)
* [x] Built-in dispatcher: `is_builtin`, `run_builtin`
* [x] Custom environment manager (`ft_getenv`)
* [x] Environment variable expansion: `$VAR`, `$?`
* [x] Debug utilities: `print_tokens()`
* [x] Memory cleanup: `free_tokens()`, `free_commands()`

### 🛠️ In Progress

* [ ] Final integration of redirection logic (`<`, `>`, `>>`, `<<`)
* [ ] Executor that handles pipes (`|`)
* [ ] Signal handling (Ctrl-C, Ctrl-D)
* [ ] Redirection output logic (via `dup2` and file descriptors)

---

## 🔜 Upcoming Tasks

* Finalize heredoc (`<<`) parsing
* Handle escaped characters in quotes
* Improve syntax validation and Bash-like error feedback
* Norm check: Ensure all `.c` files meet 42 formatting and line limits
* Add more execution test cases and error behaviors

---

## 👥 Contributors

* **chrrodri** — Project structure & lexer/parser logic
* **bsamy** — Partner from June 2025 onward, contributing to lexer, parser, env expansion, and redirection logic

Both students at **42 School Barcelona**

---

## 📁 Repository Structure

```
.
├── include/
│   ├── minishell.h
│   ├── builtin.h
│   └── struct.h
├── libft/
├── src/
│   ├── builtin/
│   ├── cmd/
│   ├── env/                   ← 🌱 NEW: contains `expand_env_vars.c`
│   ├── executor/
│   ├── initiation/
│   └── utils/
├── minishell.c
├── Makefile
└── README.md
```

---

## 📌 Quickstart

> 🆕 Current onboarding guide

1. **Run it:**
   `make && ./minishell`

2. **Try commands:**
   `echo hello | grep h`
   `echo $USER > out.txt`

3. **Read in this order:**

   * `minishell.c`
   * `tokenize.c` (`tokenize_input`, `print_tokens`)
   * `expand_env_vars.c`
   * `parser.c` (`parse_tokens`, `t_command`)
   * `execute_commands.c`

---

## 📌 Disclaimer

This project is for educational purposes as part of the 42 curriculum.
It does not aim to replace production shells and should not be copied without understanding its internal design and learning goals.

📌 *Last updated: July 7, 2025*
