# 🐚 Minishell — Tiny Bash (42 Project)

Minishell • C • Unix • Signals • Readline

A compact, Bash-like shell in **C** for the 42 core curriculum. It supports builtins, pipelines, redirections (incl. heredoc), quote-aware expansion, and correct signal/exit-status semantics.

✅ Implements **mandatory** behavior (with a minimal `*` wildcard bonus).  
❌ No logical operators (`&&`, `||`), subshells `( … )`, or assignment words (`X=1 cmd`) — by design.

---

## 📑 Table of Contents

- [🧠 Introduction](#-introduction)
- [✨ Features](#-features)
- [📁 Project Structure](#-project-structure)
- [🛠 Compilation](#-compilation)
- [🚀 Usage](#-usage)
- [🔍 How It Works](#-how-it-works)
- [⚙️ Design Highlights](#️-design-highlights)
- [🔄 Recent Changes](#-recent-changes)
- [🧪 Testing](#-testing)
- [🧵 Examples](#-examples)
- [⚠️ Caveats & Defense Notes](#️-caveats--defense-notes)
- [👤 Authors](#-authors)

---

## 🧠 Introduction

**Minishell** re-implements a subset of Bash with a focus on:

- Correct **builtins** in the parent process
- Robust **redirections** (`<`, `>`, `>>`, `<<`)
- **Pipelines** `|`
- **Expansion** of `$VAR` and `$?` with proper quote rules
- **Signals** (prompt, heredoc, children)
- Accurate **exit statuses** (`126`, `127`, `130`, `131`)

Goal: clean, Norm-compliant, defense-ready shell.

---

## ✨ Features

- 🧩 **Builtins** (in parent if not piped): `echo`, `cd`, `pwd`, `export`, `unset`, `env`, `exit`
- 🚀 **External commands** via `fork` + `execve`
- 🔁 **Pipes**: `cmd1 | cmd2 | ...`
- 📥 **Redirections**: `<`, `>`, `>>`, `<<` (heredoc honors delimiter quotes)
- 🔎 **Expansion**: `$VAR`, `$?`, quote-aware, **no extra spaces** (e.g., `echo "a"'b'c` → `abc`)
- 🧵 **Signals**
  - Prompt: `Ctrl-C` clears line + sets `$?=130`; `Ctrl-\` ignored
  - Children: `Ctrl-C` → 130, `Ctrl-\` → 131
  - Heredoc: `Ctrl-C` aborts immediately, sets `$?=130`, command is **not** executed
- 🗂 **Env mgmt**: custom `char **env` copy, `export` sorted printing, safe `unset`
- 🌟 **Bonus minimal**: `*` wildcard expansion (no hidden files unless pattern starts with `.`)

Out of scope: `&&`, `||`, subshells `()`, command substitution, assignment words.

---

## 📁 Project Structure

```

.
├── include/            # minishell.h, struct.h, builtin headers
├── libft/              # 42 libft
├── src/
│   ├── builtin/        # echo, cd, env, export, unset, pwd, exit
│   ├── executor/       # redirection.c, heredoc.c, run_external_cmd*.c
│   ├── expand/         # expand.c, expand_utils.c, wildcard_*.c
│   ├── initiation/     # tokenize.c, tokenizer_utils.c, parse_*.c, init_minishell.c
│   ├── signal/         # signal.c
│   └── utils/          # free_*, helpers
├── minishell.c
├── Makefile
└── README.md

````

---

## 🛠 Compilation

Uses the usual 42 flags:

```make
CFLAGS = -Wall -Wextra -Werror
````

Build:

```bash
make         # build minishell
make clean   # remove objects
make fclean  # remove objects + binary
make re      # full rebuild
```

> If your includes use `#include "../../include/minishell.h"`, the Makefile already adds `-Iinclude`. If you prefer `#include "minishell.h"` everywhere, keep sources at one depth or keep the `-Iinclude` and adjust headers accordingly.

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

* **Tokenizer** produces: `word`, `pipe_tok`, `redirect_in`, `redirect_out`,
  `redirect_append`, `heredoc_tok`, `single_quote`, `double_quote`, `env_var`.
* **Expander** resolves `$VAR`, `$?`; double quotes expand, single quotes don’t.
  Adjacent tokens are concatenated if there wasn’t a space.
* **Parser**

  * Builds a linked list of commands (`argv`, redirs, `next` for pipes).
  * Allows a command to start **with redirections** (e.g., `| >out cmd`).
  * Concatenates a redirection filename from adjacent tokens with no space.
* **Executor**

  * Non-piped builtins run in the parent.
  * Pipelines spawn children; exit statuses propagated.
  * **Heredoc** runs in a child with default `SIGINT` so `Ctrl-C` cancels cleanly.

---

## ⚙️ Design Highlights

* 🧠 **Builtins in parent** (when not piped) so `PWD`, `env`, and `$?` update in-shell.
* 🧱 **126/127 split**

  * `127` = not found (or `PATH` unset)
  * `126` = found but not executable / is a directory
* 📜 **Heredoc**

  * Unquoted delimiter → expand `$VAR`/`$?`
  * Quoted delimiter → no expansion
  * `Ctrl-C` → newline, `$?=130`, skip command
* 🧼 **Memory safety**: no leaks/double-frees on common flows
* 📏 **42 Norm**: ≤25 lines/function, ≤5 functions/file, ≤4 args/function, ≤80 cols, tabs for indent

---

## 🔄 Recent Changes

* ✅ **Signals unified**: single global
  `volatile sig_atomic_t g_sig` with `get_sigint_flag()` accessor.
* ✅ **Parser**: allows redirection **immediately after `|`** (e.g., `| >out cmd`).
* ✅ **Redirection filenames**: `handle_parse_redirection()` concatenates adjacent
  tokens with `space_before == 0`, then splices extra nodes out.
* ✅ **`$` handling**: inside double quotes, bare `$` stays literal; `echo "$"` prints `$`.
* ✅ **Wildcard**: `*` expands using `opendir/readdir`; ignores hidden files unless pattern starts with `.`.
* ✅ **Norm**: fixed tabs/spacing, argument counts, long functions split.

---

## 🧪 Testing

* Tester used: **[LucasKuhn/minishell_tester](https://github.com/LucasKuhn/minishell_tester)**
* Result: **146 / 146** ✅

> Some tests display “⚠️ Broken pipe” differences in output text; they are not required by the subject, and exit codes/behaviors match.

---

## 🧵 Examples

```bash
# Builtins
echo hello                 # hello
pwd                        # prints cwd
cd "$PWD" hi               # "too many arguments" (exit 1)

# Redirections (rightmost wins)
rm -f a b
echo hi > a > b; wc -c a b
# 0 a
# 3 b
# 3 total

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

# Pipes
echo a | cat -e            # a$
cat << EOF | grep hi | wc -l
hi
EOF
# 1
```

---

## ⚠️ Caveats & Defense Notes

* **Not implemented:** `&&`, `||`, subshells `( … )`, command substitution, assignment words.
* **Leading/trailing pipe**: `|`… or …`|` → syntax error, as in Bash.
* **History:** only prompt lines go to history (heredoc lines don’t).
* **Why builtins in parent?** They must mutate the shell state (env, cwd, exit).
* **126 vs 127?** `126` = found but not executable/dir; `127` = not found.

---

## 👤 Authors

**Christian (chrrodri)** — 42 Intra: `chrrodri` — GitHub: **@kitearuba**
**bsamy** — 42 Intra: `bsamy`

---

*Educational project for 42 School.*
