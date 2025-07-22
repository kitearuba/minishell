# 🐚 Minishell\_bsamy

![Status](https://img.shields.io/badge/status-in%20progress-yellow)
![Language](https://img.shields.io/badge/language-C-blue)
![License](https://img.shields.io/badge/license-42%20School-blueviolet)
![School](https://img.shields.io/badge/42-Barcelona-black)

> A minimalist Bash-like shell built from scratch for the 42 School curriculum.
> Collaborative project by `chrrodri` and `bsamy`.

---

⚠️ **Notice:**
This repository is **a work in progress** and **temporarily public** to allow collaborator access during development.
It is **not a reference implementation** and may be made private once completed. Please do not clone or fork without authorization.

---

## 📚 Table of Contents

* [⚙️ Project Overview](#️-project-overview)
* [🧭 Project Architecture & Flow (for Collaborators)](#-project-architecture--flow-for-collaborators)
* [🗕️ Development Status *(Last Updated: July 22, 2025)*](#-development-status-last-updated-july-22-2025)
* [🔜 Upcoming Tasks](#-upcoming-tasks)
* [👥 Contributors](#-contributors)
* [📁 Repository Structure](#-repository-structure)
* [📌 Quickstart](#-quickstart-for-bsamy)
* [📌 Disclaimer](#-disclaimer)

---

## ⚙️ Project Overview

**Minishell** is a Bash-inspired shell written in C, adhering to the 42 School specifications.
It replicates essential shell behavior:

* Execute built-in and external commands
* Handle redirections: `<`, `>`, `>>`, `<<`
* Manage pipes (`|`) with correct input/output propagation
* Expand environment variables: `$VAR`, `$?`
* Provide an interactive prompt using `readline`
* Maintain and propagate exit statuses

Key concepts covered:

* Process creation and forking
* File descriptor management
* Lexing and parsing
* Environment management
* Signal handling

---

## 🧭 Project Architecture & Flow (for Collaborators)

### 🔹 1. `main()` — The Shell Loop

**File:** `minishell.c`

Handles:

* Prompt display
* Input reading
* Cleanup and re-initialization

```c
line = readline("minishell $> ");
tokens = tokenize_input(line);
tokens = expand_env_vars(tokens, bash);
cmds = parse_tokens(tokens);
execute_command(cmds, bash);
````

---

### 🔹 2. `tokenize_input()` — Lexer

**File:** `src/initiation/tokenize.c`

Tokenizes input into:

* `WORD`, `PIPE`, `REDIRECT`, `ENV_VAR`, `SINGLE_QUOTE`, `DOUBLE_QUOTE`

---

### 🔹 3. `expand_env_vars()` — Environment Expansion

**File:** `src/env/expand.c`

Replaces:

* `$VAR` with value from `bash->env`
* `$?` with last exit status

---

### 🔹 4. `parse_tokens()` — Parser

**File:** `src/initiation/parse_tokens.c`

Builds a linked list of `t_command` from tokens:

```c
t_command:
  argv: {"ls", "-l"}
  next → t_command:
    argv: {"wc", "-l"}
```

---

### 🔹 5. `execute_command()` — Command Dispatcher

**File:** `src/cmd/execute_commands.c`

Routes commands to:

* Built-ins
* External programs (`execve`)
* Pipe executor
* Redirection handler

---

### 🔹 6. Redirection & Heredoc

**Files:**

* `src/executor/redirections/` (under construction)
* `src/executor/heredoc.c`

Handles `<`, `>`, `>>`, `<<`

Heredoc now works with pipes:

```bash
cat << EOF | grep hello
hello
world
EOF
```

---

### 🔹 7. Pipes

**File:** `src/executor/pipes/execute_pipeline.c`

Handles multi-stage pipelines via forked children:

```bash
ls -l | grep txt | wc -l
```

---

### 🔹 8. Shell State (`t_bash`)

```c
typedef struct s_bash {
    char        **env;
    int         exit_status;
    t_token     *tokens;
    t_command   *commands;
} t_bash;
```

---

### 🔹 9. Utilities

**Files:** `src/utils/`

Includes:

* `free_tokens()`, `free_commands()`, `free_2d_array()`
* `ft_getenv()`, `free_all_and_exit()`

---

## 🗕️ Development Status *(Last Updated: July 22, 2025)*

### ✅ Mandatory Part — Completed

* [x] Tokenizer with quote/env/pipe handling
* [x] Parser from token list → command list
* [x] Environment variable expansion
* [x] Custom `char **env` structure
* [x] Built-ins: `cd`, `echo`, `env`, `export`, `pwd`, `unset`, `exit`
* [x] Pipeline execution logic
* [x] Heredoc with pipe support
* [x] Full memory cleanup
* [x] Exit status management

### ☑️ Bonus Part — In Progress

* [x] Wildcard expansion (`*`) for matching files (bonus)
* [x] Heredoc chaining with pipe
* [ ] Logical operators: `&&`, `||` (bonus)
* [ ] Subshells: `(cmd)` using parentheses (bonus)
* [ ] Advanced redirection chaining
* [ ] Signal behavior polishing (Ctrl-C, Ctrl-D)

---

## 🔜 Upcoming Tasks

* Finalize bonus logic for `&&`, `||`, and subshells `(cmd)`
* Redirection edge case handling and chaining
* Final signal behavior consistency
* Apply full 42 Norm compliance:

    * 25-line function limit
    * 80-column width
    * Header format
* Add more test cases (edge quotes, nested redir, wildcard filters)

---

## 👥 Contributors

* **chrrodri** — Architecture, lexer/parser, pipe logic, env system
* **bsamy** — Redirections, built-ins, wildcard, heredoc, expansion

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
│   │   ├── pipes/
│   │   └── redirections/
│   ├── initiation/
│   └── utils/
├── minishell.c
├── Makefile
└── README.md
```

---

## 📌 Quickstart

```bash
make && ./minishell
```

Try:

```bash
echo hello | grep h
cat << EOF | grep test
test
line
EOF
```

---

## 📌 Disclaimer

This project was created for educational use at 42 School.
Not intended for production environments.

📌 *Last updated: July 22, 2025*
