# 🐚 Minishell — Tiny Bash (42 Project)

**Minishell • C • Unix • Signals • Readline**

A compact, Bash-like shell in **C** for the 42 core curriculum. It supports builtins, pipelines, redirections (incl. heredoc), quote-aware expansion, and correct signal/exit-status semantics — implemented **strictly within the subject** (not a full Bash).

- ✅ **Mandatory**: complete, defense-ready
- ✨ **Bonus (started)**: minimal `*` wildcard expansion (ungraded in our defense)
- 🚫 Not implemented by design: `&&`, `||`, subshells `( … )`, command substitution, assignment words like `X=1 cmd`

> This repository contains the final, evaluated version. The code follows **42 Norm** (≤ 25 lines per function, ≤ 5 functions per file, ≤ 4 params per function, formatting rules).

---

## 📑 Table of Contents

1. [Introduction](#-introduction)
2. [Features & Scope](#-features--scope)
3. [Build & Dependencies](#-build--dependencies)
4. [Quick Start](#-quick-start)
5. [Architecture Overview](#-architecture-overview)
   - [Data Structures](#data-structures)
   - [Control Flow](#control-flow)
   - [Execution Rules](#execution-rules)
6. [Lexing, Parsing & Expansion](#-lexing-parsing--expansion)
7. [Redirections & Heredoc](#-redirections--heredoc)
8. [Signals & Exit Status](#-signals--exit-status)
9. [Environment Management](#-environment-management)
10. [Wildcards (Bonus — started)](#-wildcards-bonus--started)
11. [Testing & Example Sessions](#-testing--example-sessions)
12. [Defense Notes](#-defense-notes)
13. [Known Limitations](#-known-limitations)
14. [Changelog (Recent Fixes)](#-changelog-recent-fixes)
15. [Project Structure](#-project-structure)
16. [Build Options & Make Targets](#-build-options--make-targets)
17. [Evaluator Quick Checklist](#-evaluator-quick-checklist)
18. [Authors](#-authors)

---

## 🧠 Introduction

**Minishell** re-implements a constrained, **Bash-compatible subset** focusing on:

- Correct **builtins** in the **parent** process (when not piped)
- Robust **redirections** (`<`, `>`, `>>`, `<<`) with right-most precedence
- **Pipelines** `|`
- **Expansion** of `$VAR` and `$?` honoring quote rules & token adjacency
- **Signals** for prompt / children / heredoc
- Accurate **exit statuses** (`126`, `127`, `130`, `131`)

Design goal: **clean, Norm-compliant, defense-ready** code that behaves like Bash in the parts covered by the subject.

---

## ✨ Features & Scope

- **Builtins** (run in parent if not piped):
  - `echo` (with `-n`)
  - `cd`
  - `pwd`
  - `export`
  - `unset`
  - `env`
  - `exit`
- **External commands** via `fork` + `execve`
- **Pipelines**: arbitrary length (`cmd1 | cmd2 | ...`)
- **Redirections**:
  - Input `<`
  - Output `>`
  - Append `>>`
  - Heredoc `<<` (quoted/unquoted delimiter semantics)
- **Expansion**:
  - `$VAR`, `$?`
  - Single quotes block expansion
  - Double quotes allow expansion
  - Adjacent tokens preserved (e.g., `"a"'b'c` → `abc`)
- **Signals**
  - Prompt: `Ctrl-C` clears line + sets `$?=130`; `Ctrl-\` ignored
  - Children: `Ctrl-C` → 130, `Ctrl-\` → 131
  - Heredoc: `Ctrl-C` aborts immediately (status 130), skips command
- **Environment management**
  - Deep-copied `char **env`
  - `export` sorted printing, strict identifier validation
  - `unset` safe removal
- **Bonus (started)**:
  - Minimal `*` wildcard expansion (no hidden files unless pattern starts with `.`)

**Out of scope** per subject:
- Logical operators `&&`, `||`
- Subshells `( … )`
- Command substitution `` `cmd` `` / `$(cmd)`
- Assignment words (`X=1 cmd`)

---

## 🔧 Build & Dependencies

### Dependencies
- **readline** library
- 42 **libft** (included as submodule or folder)

On **Linux (Debian/Ubuntu)**:
```bash
sudo apt-get update
sudo apt-get install -y libreadline-dev
make
````

On **macOS** (Homebrew):

```bash
brew install readline
# The Makefile already links with -lreadline and includes -I as needed
make
```

### Build

```bash
make         # build minishell
make clean   # remove object files
make fclean  # remove objects + binary
make re      # full rebuild
```

---

## 🚀 Quick Start

```bash
./minishell

minishell $> echo hello | grep h
minishell $> export A=42; echo "$A"
minishell $> cat << EOF | grep hi
> hi
> EOF
minishell $> cd -
```

---

## 🏗 Architecture Overview

### Data Structures

```c
typedef struct s_redirection {
    int                  type;       // enum: <, >, >>, << 
    char                *filename;   // for << store delimiter
    struct s_redirection *next;
} t_redirection;

typedef struct s_command {
    char             **argv;         // null-terminated
    t_redirection    *redirection;   // linked list
    struct s_command *next;          // pipeline chain
} t_command;

typedef struct s_bash {
    char      **env;           // deep-copied environment
    int        exit_status;
    t_token   *tokens;         // last tokens (for cleanup)
    t_command *commands;       // last parsed pipeline (for cleanup)
    char     **export_noval;   // names declared via 'export NAME' (no value)
} t_bash;
```

### Control Flow

```
readline → tokenize → expand → parse → execute
```

* **Tokenizer**: words, quotes, `$`, redirects, pipe
* **Expander**: `$VAR`, `$?` under quote rules; concatenates adjacent pieces
* **Parser**: builds `t_command` linked list; attaches redirections; permits
  redirections at command start (e.g., `> out cmd`)
* **Executor**:

  * **No pipe**: builtin runs in **parent**, else `fork` + `execve`
  * **Pipeline**: each stage forks; redirs apply in child; parent reaps and sets status from **rightmost** child

### Execution Rules

* Right-most redirection wins (`echo hi > a > b` writes to `b`)
* **Builtins in parent** (when not piped) so that `PWD`, `env`, `$?` update in-shell
* In pipelines, builtins run in the **child** of their stage (Bash semantics)
* `cd -`:

  * uses `$OLDPWD` and **prints** the new cwd
  * updates `PWD`/`OLDPWD`

---

## 🧩 Lexing, Parsing & Expansion

* **Single quotes `'...'`**: literal text, no expansion
* **Double quotes `"..."`**: expand `$VAR`, `$?`
* **Bare `$`** inside double quotes remains `$` if no valid name follows
* **Adjacency** preserved:

  * `"a"'b'c` → `abc`
  * filenames for redirections can be built from multiple adjacent pieces without spaces; we **concatenate during parse**
* `$?` expands to last exit status

---

## 📥 Redirections & Heredoc

* `<` — open read-only, dup to stdin
* `>` — create/truncate, dup to stdout
* `>>` — create/append, dup to stdout
* `<<` (heredoc)

  * **Quoted delimiter** → no expansion in heredoc body
  * **Unquoted delimiter** → expand `$VAR` and `$?`
  * `Ctrl-C` during heredoc cancels immediately (status **130**), command is **skipped**
* Redirections are applied **inside children** (including pipeline stages), so each stage has its own view
* For a chain of `>`/`>>`, the **last** one takes effect (right-most precedence)

---

## 🪓 Signals & Exit Status

* **Prompt**

  * `SIGINT` (`Ctrl-C`): clear line + newline, set `$?=130`
  * `SIGQUIT` (`Ctrl-\`): **ignored**
* **Children**

  * `SIGINT` → exit code 130
  * `SIGQUIT` → exit code 131
* **Exit codes of interest**

  * `127` — command not found / `PATH` unset
  * `126` — found but **not executable** or is a **directory**
  * `130` — terminated by `SIGINT`
  * `131` — terminated by `SIGQUIT`
* For pipelines, the shell’s `$?` is the **rightmost** stage’s status (with the signal mapping above)

---

## 🌱 Environment Management

* We deep-copy the host environment (`char **env`) at startup
* **`export` rules** (Bash-compatible within scope)

  * `export` (no args): print a **sorted** merged view of actual `env` and names stored in `export_noval`

    * Format: `declare -x KEY="VAL"` (if value) or `declare -x KEY` (if no value)
  * `export NAME` (no `=`):

    * Validate identifier (see below)
    * If `NAME` is **not** already in `env`, store it in `export_noval` (name only)
  * `export NAME=VAL`:

    * Add/replace in `env`
    * Remove `NAME` from `export_noval` if present
  * Invalid identifiers print to `stderr` and set status **1**
* **Identifier validation**:

  * `[A-Za-z_][A-Za-z0-9_]*`
  * Rejects: `A-`, `HELLO-=123`, `=`, `123`, etc.
* **`unset NAME`** removes from `env` and also from `export_noval` if present

---

## 🔹 Wildcards (Bonus — started)

* Minimal `*` expansion implemented (not graded in defense)
* Uses `opendir/readdir`
* Expands only **unquoted** tokens containing `*`
* Hidden files are **ignored** unless the pattern starts with `.`

---

## 🧪 Testing & Example Sessions

We validated behavior with the public **LucasKuhn/minishell_tester** plus extra manual tests.

### Handy Manual Checks

```bash
# cd -
pwd; cd ..; cd -; echo "$?"             # prints new cwd; exit 0

# export (name only vs key=val)
unset A; export A; export | grep '^declare -x A$'
export A=1; export | grep '^declare -x A="1"$'
unset A

# invalid export (should set status 1 and print error)
export A-; echo $?
export HELLO-=123; echo $?
export =; echo $?
export 123; echo $?

# pipe + env mutation (env change must not leak back to parent through a pipe)
export GHOST=123 | env | grep GHOST     # parent remains unchanged

# mixed quoting / adjacency
echo "a"'b'c                            # -> abc
echo "$"                                # -> $

# heredoc cancel (Ctrl-C)
cat << EOF
^C                                      # heredoc canceled, $?=130

# signal exit statuses from children
sleep 5 & kill -QUIT %1                 # observe status 131
```

### Pipelines

```bash
echo a | cat -e
cat << EOF | grep hi | wc -l
hi
EOF
```

### Redirections

```bash
rm -f a b
echo hi > a > b; wc -c a b
# 0 a
# 3 b
# 3 total

grep hi < ./test_files/infile
echo hi > out.txt
echo again >> out.txt
```

---

## 🛡 Defense Notes

* We implemented **only** what the subject requires, not full Bash.
* **Builtins in parent** (when not piped) so `PWD`, `env`, `$?` mutate the shell state.
* **Pipelines** spawn children per stage; redirs are applied in children; parent reaps and sets `$?` from the **rightmost** stage.
* **`cd -`** uses `$OLDPWD`, **prints** the new cwd, and updates `PWD`/`OLDPWD`.
* **`export`**:

  * Print format: `declare -x KEY="VAL"` or `declare -x KEY` (no quotes if no value)
  * Accept `export NAME` as a declaration (recorded in `export_noval` unless already in `env`)
  * Invalid identifiers → `stderr`, exit 1
* **126 vs 127**: distinguished correctly (not executable vs not found).
* **Heredoc**:

  * Quoted delimiter → no expansion
  * Unquoted → expands `$VAR`/`$?`
  * `Ctrl-C` cancels heredoc and sets `$?=130` (command skipped)

---

## ⚠️ Known Limitations

* No logical operators (`&&`, `||`), no subshells `( … )`
* No command substitution or assignment words
* Minimal wildcard only (bonus, not graded)
* Message strings like “Broken pipe” may differ from Bash; **not required by subject**. Exit codes match.

---

## 📝 Changelog (Recent Fixes)

* **`cd -` parity with Bash**

  * Fetch `$OLDPWD`, change directory, **print** new cwd
  * Update `PWD`/`OLDPWD`
  * If `OLDPWD` missing → `minishell: cd: OLDPWD not set` (exit 1)

* **`export` compliance**

  * Added **name-only** declarations with `bash->export_noval`
  * `export` (no args) prints **sorted** merged view of env + noval
  * Correct quoting: `declare -x KEY="VAL"` or `declare -x KEY`
  * **Strict identifier validation** (reject `A-`, `HELLO-=123`, `=`, `123`, …)
  * On `NAME=VAL`, update env and **remove** from noval
  * On `unset NAME`, also remove from noval

* **Pipelines**

  * Builtins inside pipeline stages run in the **child** of that stage
  * Redirections applied **per child**
  * Parent computes final `$?` from the **rightmost** stage

* **Norm refactors**

  * Split long functions (e.g., pipeline execution into `child_process`, `pipe_child`, `wait_for_pipeline`)
  * Respected ≤ 25 lines/function, ≤ 5 functions/file, ≤ 4 params/function, line widths, spacing, includes

* **Printing & errors**

  * Bash-like formatting for `export` lines
  * Consistent error messages for `cd`, invalid identifiers, and heredoc cancel

---

## 📂 Project Structure

```
.
├── include/
│   ├── minishell.h
│   ├── builtin.h
│   └── struct.h
├── libft/                      # 42 libft
├── minishell.c                 # entry (delegates to core)
├── src/
│   ├── core/
│   │   ├── init_minishell.c
│   │   └── execute_minishell.c
│   ├── lexer/
│   │   ├── tokenize.c
│   │   ├── tokenizer_utils.c
│   │   └── tokenizer_redirects.c
│   ├── parser/
│   │   ├── parse_tokens.c
│   │   ├── parse_utils.c
│   │   ├── parser_cmd.c
│   │   ├── parser_cmd_utils.c
│   │   ├── parser_checks.c
│   │   ├── handle_parse_redirection.c
│   │   └── last_token_is_pipe.c
│   ├── expand/
│   │   ├── expand.c
│   │   ├── expand_utils.c
│   │   ├── expand_wildcard.c
│   │   ├── wildcard_utils.c
│   │   └── wildcard_utils_2.c
│   ├── execution/
│   │   ├── execve.c
│   │   ├── get_cmd_path.c
│   │   ├── redirection.c
│   │   ├── heredoc.c
│   │   ├── heredoc_utils.c
│   │   ├── run_external_cmd.c
│   │   ├── run_external_cmd_utils.c
│   │   └── pipes/
│   │       ├── create_pipe.c
│   │       └── execute_pipeline.c
│   ├── builtin/
│   │   ├── ft_echo.c
│   │   ├── ft_cd.c
│   │   ├── ft_pwd.c
│   │   ├── ft_env.c
│   │   ├── ft_export.c
│   │   ├── ft_export_utils.c
│   │   ├── ft_unset.c
│   │   └── ft_exit.c
│   ├── env/                    # ft_getenv, env_set, etc.
│   ├── signal/                 # prompt + child signal handling
│   └── utils/                  # frees, strings, small helpers
├── Makefile
└── README.md
```

> Headers use `#include "minishell.h"` with `-Iinclude` in the Makefile.

---

## 🛠 Build Options & Make Targets

* `make` — build `minishell`
* `make clean` — remove objects
* `make fclean` — remove objects and binary
* `make re` — full rebuild

Compile flags (typical 42):

```
-Wall -Wextra -Werror
```

The Makefile links `-lreadline` and includes the correct headers (adjust include paths for your platform if needed).

---

## ✅ Evaluator Quick Checklist

* **Prompt** shows and accepts input until EOF (`Ctrl-D`)
* **Signals**

  * Prompt: `Ctrl-C` clears line + sets `$?=130`
  * `Ctrl-\` ignored at prompt
  * Children: `SIGINT`→130, `SIGQUIT`→131
* **Builtins**

  * `echo`, `cd`, `pwd`, `export`, `unset`, `env`, `exit`
  * Builtins run in parent **unless piped**
  * `cd -` prints new cwd and updates `PWD`/`OLDPWD`
  * `export` without args prints sorted `declare -x ...`
  * Invalid `export` identifiers → error + status 1
  * `unset` removes keys (and from `export_noval`)
* **Expansion**

  * `$VAR`, `$?` with quote rules
  * Adjacent tokens concatenate properly
* **Redirections**

  * `<`, `>`, `>>`, `<<` working
  * Heredoc quoted delimiter → no expansion
  * Heredoc `Ctrl-C` cancels (status 130)
* **Pipelines**

  * N-stage pipelines, rightmost exit status applies
  * Redirs applied in the children
* **Path resolution**

  * `127` not found; `126` not executable / is directory
* **Norm**

  * ≤ 25 lines/function, ≤ 5 functions/file, ≤ 4 parameters/function
  * Indentation, headers, includes, forbidden functions: respected

---

## 👤 Authors

* **Christian (chrrodri)** — 42 intra: `chrrodri` — `chrrodri@student.42barcelona.com` — GitHub: **@kitearuba**
* **bsamy** — 42 intra: `bsamy`

> Educational project for 42 School (Barcelona). Feedback and suggestions are welcome!

```
