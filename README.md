# 🐚 Minishell_bsamy

![Status](https://img.shields.io/badge/status-in%20progress-yellow)
![Language](https://img.shields.io/badge/language-C-blue)
![License](https://img.shields.io/badge/license-42%20School-blueviolet)
![School](https://img.shields.io/badge/42-Barcelona-black)

> A minimalist Bash-like shell built from scratch for the 42 School curriculum.  
> Project in collaboration with `chrrodri` and `bsamy`.

---

## 📚 Table of Contents

- [⚙️ Project Overview](#️-project-overview)
- [🧭 Project Architecture & Flow (for Collaborators)](#-project-architecture--flow-for-collaborators)
- [📅 Development Status *(Last Updated: June 2025)*](#-development-status-last-updated-june-2025)
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
````

---

### 🔹 2. `tokenize_input()` — Lexer

* File: `src/initiation/tokenize.c`
* Splits the user input into tokens (words, pipes, etc.)
* Outputs a `t_token *` linked list

Example:

```
echo hello | grep world
→ [WORD: "echo"] → [WORD: "hello"] → [PIPE: "|"] → [WORD: "grep"] → [WORD: "world"]
```

---

### 🔹 3. `parse_tokens()` — Parser

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

### 🔹 4. `execute_command()` — Command Dispatcher

* File: `src/cmd/execute_commands.c`
* Routes commands to:

  * Built-ins: `cd`, `exit`, etc.
  * External: calls `execve()` via `run_external_cmd.c`

---

### 🔹 5. Shell State (`t_bash`)

* Central struct passed to all modules
* Stores:

  * `char **env` (custom environment)
  * `int exit_status` (last command result)

---

### 🔹 6. Utilities

* Located in `src/utils`
* `free_tokens()` and `free_2d_array()` for memory management
* `ft_getenv()` replaces `getenv()` for controlled env access

---

## 📅 Development Status *(Last Updated: June 2025)*

### ✅ Completed

* [x] Makefile and main shell structure
* [x] Custom shell state with `t_bash`
* [x] Tokenizer for basic commands + pipes
* [x] Command parser (`t_token → t_command`)
* [x] Built-in dispatcher: `is_builtin`, `run_builtin`
* [x] Custom environment manager (`ft_getenv`)
* [x] Debug utilities: `print_tokens()`
* [x] Memory cleanup: `free_tokens()`

### 🛠️ In Progress

* [ ] Full parser integration with redirections
* [ ] Executor that handles pipes (`|`)
* [ ] Expand tokenizer to support:

  * Quotes: `'`, `"`
  * Redirections: `>`, `>>`, `<`, `<<`
  * Environment variables: `$VAR`, `$?`
* [ ] Signal handling (Ctrl-C, Ctrl-D)
* [ ] Redirection output logic (via `dup2` and file descriptors)

---

## 🔜 Upcoming Tasks

* Implement `free_commands()` to avoid leaks after parsing
* Add heredoc (`<<`) parsing
* Handle escaped characters
* Improve error messaging and Bash-like feedback
* Final cleanup for Deepthought and 42 Norm compliance

---

## 👥 Contributors

* **chrrodri** — Original author, project structure & lexer/parser logic
* **bsamy** — Partner from June 2025 onward, contributing to parsing, redirection, and exec logic

Both students at **42 School Barcelona**.

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
│   ├── env/
│   ├── executor/
│   ├── initiation/
│   └── utils/
├── minishell.c
├── Makefile
└── README.md
```

---

## 📌 Quickstart for Bsamy

> 🆕 Partner onboarding guide

1. **Run it:**
   `make && ./minishell`

2. **Try commands:**
   `echo hello | grep h`

3. **Read in this order:**

   * `minishell.c`
   * `tokenize.c` (`tokenize_input`, `print_tokens`)
   * `parser.c` (`parse_tokens`, `t_command`)
   * `execute_commands.c`

4. Ping `chrrodri` for:

   * Task division
   * File ownership
   * Help understanding environment/exec parts

---

## 📌 Disclaimer

This project is for educational purposes as part of the 42 curriculum.
It does not aim to replace production shells and should not be copied without understanding its internal design and learning goals.
