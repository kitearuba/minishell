---

# 🐚 Minishell — Tiny Bash (42 Project)

Minishell C Unix Signals Readline

This project implements a compact, Bash-like shell in **C** for the 42 core curriculum. It supports built-ins, pipelines, redirections (including heredoc), environment expansion, and correct signal/exit-status semantics.

✅ This repo implements the **mandatory** Minishell behavior (with a safe `*` wildcard bonus).
❌ No logical operators (`&&`, `||`), subshells `( … )`, or assignment words (`X=1 cmd`) — by design.

---

## 📑 Table of Contents

* [🧠 Introduction](#-introduction)
* [✨ Features](#-features)
* [📁 Project Structure](#-project-structure)
* [🛠 Compilation](#-compilation)
* [🚀 Usage](#-usage)
* [🔍 How It Works](#-how-it-works)
* [⚙️ Design Highlights](#️-design-highlights)
* [🔄 Improvements Over Early Versions](#-improvements-over-early-versions)
* [🧪 Testing & Debugging Notes](#-testing--debugging-notes)
* [🧵 Test Scenarios](#-test-scenarios)
* [📊 Advanced Testing Examples](#-advanced-testing-examples)
* [⚠️ Caveats & Defense Notes](#️-caveats--defense-notes)
* [🙌 Acknowledgments](#-acknowledgments)
* [👤 Authors](#-authors)

---

## 🧠 Introduction

**Minishell** re-implements a subset of Bash with a focus on:

* Correct **built-in** behavior in the parent process
* Proper **redirections** (`<`, `>`, `>>`, `<<`)
* **Pipelines** `|`
* **Expansion** of `$VAR` and `$?` with quote rules
* **Signals** at the prompt and for child processes
* Accurate **exit statuses** (`126`, `127`, `130`, `131`)

Goal: a clean, Norm-compliant, defense-ready shell.

---

## ✨ Features

* 🧩 **Built-ins** (parent, non-piped): `echo`, `cd`, `pwd`, `export`, `unset`, `env`, `exit`
* 🚀 **External commands** via `fork` + `execve`
* 🔁 **Pipes**: `cmd1 | cmd2 | ...`
* 📥 **Redirections**: `<`, `>`, `>>`, `<<` (heredoc with Bash-like quote semantics)
* 🔎 **Expansion**: `$VAR`, `$?`, quote-aware, **no extra spaces** (e.g., `echo "a"'b'c` → `abc`)
* 🧵 **Signals**

  * Prompt: `Ctrl-C` clears line + sets `$?=130`; `Ctrl-\` ignored
  * Children: `Ctrl-C` → `130`, `Ctrl-\` → `131`
* 🗂 **Env mgmt**: custom `char **env` copy, `export` sorted listing, safe `unset`
* 🌟 **Bonus kept minimal**: wildcard `*` expansion (no hidden files unless pattern starts with `.`)

Out of scope (by choice): `&&`, `||`, subshells `()`, command substitution, assignment words (`X=1 cmd`).

---

## 📁 Project Structure

```
.
├── include/            # minishell.h, builtin.h, struct.h
├── libft/              # 42 libft (used, not re-documented here)
├── src/
│   ├── builtin/        # echo, cd, env, export, unset, pwd, exit
│   ├── cmd/            # execute_commands.c (dispatcher)
│   ├── env/            # ft_getenv.c (+ env helpers)
│   ├── executor/       # redirection.c, heredoc.c, run_external_cmd.c
│   │   └── pipes/      # execute_pipeline.c
│   ├── expand/         # expand.c, expand_utils.c, wildcard_*.c
│   ├── initiation/     # tokenize.c, tokenizer_utils.c, parse_*.c, init_minishell.c
│   ├── signal/         # signal.c (prompt vs child handlers)
│   └── utils/          # free_*, helpers
├── minishell.c         # main loop & readline
├── Makefile
└── README.md
```

---

## 🛠 Compilation

**Submission flags (Norm-only):**

```
CFLAGS = -Wall -Wextra -Werror
```

**Build:**

```bash
make         # build minishell
make clean   # remove objects
make fclean  # remove objects + binary
make re      # full rebuild
# Tip: make -n  # dry run (print commands without executing)
```

---

## 🚀 Usage

```bash
./minishell
minishell $> echo hello | grep h
minishell $> cat << EOF | grep test
> test
> line
> EOF
```

---

## 🔍 How It Works

**Loop** → `readline` → tokenize → expand → parse → execute.

* **Tokenizer** splits into `WORD`, `PIPE`, `REDIRECT_*`, `HEREDOC`, `SINGLE_QUOTE`, `DOUBLE_QUOTE`, `ENV_VAR`.
* **Expander** resolves `$VAR`, `$?`; `"double quotes"` expand, `'single quotes'` don’t; adjacent tokens concatenate without extra spaces.
* **Parser** builds a simple command list (`argv`, redirs, `next` for pipelines).
* **Executor**

  * Single command: built-ins in parent; external via child `fork`+`execve`
  * Pipeline: one child per stage, statuses propagated
  * **Redirections**: open/dup2 with clear errors; heredoc via a child process that uses default SIGINT so `Ctrl-C` aborts immediately and sets parent’s `$?=130` without executing the pending command.

---

## ⚙️ Design Highlights

* 🍽 **Builtins in parent** (when not piped) so `PWD`, `OLDPWD`, `env`, and shell `exit_status` update correctly.
* 🧱 **Clear 126/127 split**

  * `127` = not found (also when `PATH` is unset)
  * `126` = found but **not executable** or **is a directory**
* 📜 **Heredoc semantics**

  * Unquoted delimiter → expands `$VAR`, `$?`
  * Quoted delimiter → **no expansion**
  * `Ctrl-C` in heredoc → immediate abort, newline, `$?=130`, **no command run**
* 🧼 **Memory safety**: no leaking/double-free on repeated `unset` / empty env mutations
* 📏 **42 Norm**: ≤25 lines/function, ≤5 functions/file, headers/spacing fixed

---

## 🔄 Improvements Over Early Versions

* ✅ **Signals fixed**: prompt redraw on `Ctrl-C`; children return 130/131; heredoc `Ctrl-C` no longer needs extra Enter.
* ✅ **Exit handling improved**: `exit` matches Bash (`2` for non-numeric, `1` for too many args without exiting, numeric mod 256).
* ✅ **126/127 correctness**: permission/dir vs. not found.
* ✅ **PATH unset**: external commands error with “No such file or directory”, status `127`.
* ✅ **Redirections**: rightmost `>` wins (`echo hi > a > b` → `a` empty, `b` “hi”).
* ✅ **Concatenation**: `"a"'b'c` → `abc` (no spurious spaces).
* ✅ **Heredoc expansion**: quoted delimiter blocks expansion; unquoted expands `$?`/`$VAR`.

---

## 🧪 Testing & Debugging Notes

* Manual sessions cover signals, heredoc, pipelines, and redirections.
* Light valgrind checks on common flows (no obvious leaks).
* Wildcard kept minimal and isolated; it won’t break mandatory flows.

---

## 🧵 Test Scenarios

```bash
# Builtins
echo hello                     # hello
echo -n -n hey                 # hey (no newline)
pwd                            # prints cwd
cd /does/not/exist             # error, $?=1

# Env
export X=
export | grep '^declare -x X=' # shows X=
unset X; unset X               # no crash

# PATH unset
unset PATH
ls                             # "No such file or directory", $?=127
export PATH=/bin:/usr/bin

# Redirections
echo hi > a > b; wc -c a b
# 0 a
# 3 b
# 3 total
cat < a                        # (empty)

# Heredoc – unquoted expands
export A=42
cat << EOF
$A $? ok
EOF
# 42 0 ok

# Heredoc – quoted blocks expansion
cat << "EOF"
$A literal
EOF
# $A literal

# Heredoc – Ctrl-C
cat << EOF
^C
echo $?                        # 130 (no command executed)

# Pipes
echo a | cat -e                # a$
cat << EOF | grep hi | wc -l
hi
EOF
# 1
```

---

## 📊 Advanced Testing Examples

```bash
# Check rightmost redirect behavior (b has the data, a is empty)
rm -f a b
echo hi > a > b
wc -c a b

# Verify $? after signals
sleep 10  # press Ctrl-C
echo $?

# PATH restored; not found vs permission
export PATH=/bin:/usr/bin
nosuchcmd; echo $?
chmod 000 ./minishell; ./minishell; echo $?   # 126 (permission)
```

---

## ⚠️ Caveats & Defense Notes

* **Not implemented (by choice):** `&&`, `||`, subshells `( … )`, command substitution, assignment words (`X=1 cmd`). Typing `X=` alone is treated as a command (not required by the subject).
* **Heredoc & history:** Only the main prompt lines are added to history; heredoc lines are not (matches the scale’s expectation).
* **`make -n` vs `make`:** `make -n` **prints** commands without executing (dry run), helpful to verify the Makefile.

**Key defense answers**

* Why builtins in parent? They must change shell state (env, cwd, exit).
* 126 vs 127? `126` found but not executable / directory; `127` not found.
* Heredoc quotes? Unquoted expands; quoted doesn’t; `Ctrl-C` → 130 and skip command.
* Concatenation? `"a"'b'c` → `abc` (no spaces added by the lexer).

---

## 🙌 Acknowledgments

* Readline, GNU manpages, and 42 testers
* 42 peers for feedback & debugging sessions
* Minimal wildcard logic sourced from our own utils (kept separate from mandatory path)

---

## 👤 Authors

**Christian (chrrodri)**

* 42 Intra: `chrrodri`
* GitHub: **@kitearuba**

**bsamy**

* 42 Intra: `bsamy`

---

*This project is for educational purposes at 42 School. Not intended for production.*
