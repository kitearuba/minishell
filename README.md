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
* [🗕️ Development Status *(Last Updated: July 21, 2025)*](#-development-status-last-updated-july-21-2025)
* [🔜 Upcoming Tasks](#-upcoming-tasks)
* [👥 Contributors](#-contributors)
* [📁 Repository Structure](#-repository-structure)
* [📌 Quickstart](#-quickstart-for-bsamy)
* [📌 Disclaimer](#-disclaimer)

---

## ⚙️ Project Overview

**Minishell** is a Bash-inspired shell written in C, adhering to the 42 School specifications.
It aims to replicate essential shell behavior:

* Execute built-in and external commands
* Handle redirections: `<`, `>`, `>>`, `<<` (heredoc)
* Manage pipes (`|`) with correct input/output propagation
* Expand environment variables: `$VAR`, `$?`
* Provide an interactive prompt with `readline`
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

Located in `minishell.c`, it handles:

* Prompt display
* Input reading
* Cleanup and re-initialization

```c
line = readline("minishell $> ");
tokens = tokenize_input(line);
tokens = expand_env_vars(tokens, bash);
cmds = parse_tokens(tokens);
execute_command(cmds, bash);
```

---

### 🔹 2. `tokenize_input()` — Lexer

**File:** `src/initiation/tokenize.c`

Tokenizes user input into:

* `WORD`, `PIPE`, `REDIRECT`, `ENV_VAR`, `SINGLE_QUOTE`, `DOUBLE_QUOTE`

Example:

```shell
echo $USER | grep world
# → [WORD:echo] → [ENV_VAR:$USER] → [PIPE:|] → [WORD:grep] → [WORD:world]
```

---

### 🔹 3. `expand_env_vars()` — Variable Expansion

**File:** `src/env/expand.c`

Replaces:

* `$VAR` with its value from `bash->env`
* `$?` with last exit status

Resulting `ENV_VAR` tokens are transformed into `WORD`.

---

### 🔹 4. `parse_tokens()` — Syntax Parsing

**File:** `src/initiation/parse_tokens.c`

Builds a linked list of `t_command`, where each node represents a command segment (used in pipes or solo):

```c
t_command:
  argv: {"cat", "<<", "END"}
  next → t_command:
    argv: {"grep", "hello"}
```

---

### 🔹 5. `execute_command()` — Command Dispatcher

**File:** `src/cmd/execute_commands.c`

Dispatches to:

* Built-ins (`cd`, `export`, etc.)
* External commands (via `execve`)
* Pipe handlers (`execute_pipeline()`)
* Redirections (using helpers in `executor/`)

---

### 🔹 6. Redirection & Heredoc

**Files:**

* `src/executor/redirections/` (TBD / partially implemented)
* `src/executor/heredoc.c` (under development)

Handles:

* Input `<` and output redirection `>`/`>>`
* Here-document input (`<<`) using temporary buffer file or pipe with `fork()` + `write()`

Recent progress:
✅ Heredoc tested with pipelines, e.g.:

```shell
cat << EOF | grep hello
hello
world
EOF
```

---

### 🔹 7. Shell State (`t_bash`)

```c
typedef struct s_bash {
    char        **env;
    int         exit_status;
    t_token     *tokens;
    t_command   *commands;
} t_bash;
```

Accessible across all logic layers.

---

### 🔹 8. Utilities

**Files:** `src/utils/`

* `free_tokens()`, `free_commands()`
* `free_2d_array()`, `ft_getenv()`
* `free_all_and_exit()` — safe shutdown

---

## 🗕️ Development Status *(Last Updated: July 21, 2025)*

### ✅ Completed

* [x] Lexer & tokenizer with quote/pipe/env support
* [x] Parser: `t_token → t_command`
* [x] Env expansion for `$VAR` and `$?`
* [x] Custom environment storage (`char **env`)
* [x] Built-in commands (`cd`, `export`, etc.)
* [x] Pipeline execution using `t_command->next`
* [x] Heredoc parsing and partial execution via `cat << END | ...`
* [x] Full memory cleanup on exit

### 🛠️ In Progress

* [ ] Final redirection integration (`<`, `>`, `>>`, `<<`)
* [ ] Pipe + heredoc chaining edge cases
* [ ] Signal handling (Ctrl-C / Ctrl-D)
* [ ] Syntax error detection and feedback
* [ ] Norm & 42 compliance audit (25-line, 80-col, etc.)

---

## 🔜 Upcoming Tasks

* Finalize `<<` into stdin pipe buffer logic
* Refactor executor into modular handlers
* Add error messages for bad syntax and permissions
* Normalize struct access (`bash->env`, etc.)
* Expand test coverage (quotes inside heredoc, nested redir, etc.)

---

## 👥 Contributors

* **chrrodri** — Project structure, lexer/parser, pipe logic, cleanup tools
* **bsamy** — Redirection, heredoc logic, built-in logic, token/command flow

Both are students at **42 Barcelona**.

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

Recommended file walkthrough:

* `minishell.c`
* `tokenize.c` → `expand_env_vars.c`
* `parse_tokens.c`
* `execute_commands.c`
* `executor/pipes/execute_pipeline.c`
* `executor/redirections/`
* `utils/free_all_and_exit.c`

---

## 📌 Disclaimer

This project is created for educational purposes at 42 School.
Not suitable for production use.

📌 *Last updated: July 21, 2025*
