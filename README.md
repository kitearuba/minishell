# Minishell_bsamy

**Note:** This project is a work in progress. It has been made public temporarily to allow a collaborator to access and contribute. **Do not use this as a finished or reference project.** The repository will be updated and/or made private upon completion.

---

## ⚙️ Project Overview

Minishell replicates basic shell behavior, including:
- Executing commands (both built-in and external)
- Handling input/output redirection
- Managing pipes (`|`)
- Expanding environment variables
- Tracking process exit statuses (`$?`)
- Interacting via `readline`

This project deepens understanding of:
- **Processes and forks**
- **Signals and file descriptors**
- **Lexical analysis and command parsing**
- **Environment management**

---

## ✅ Current Progress (as of Day 4)

### Implemented:
- [x] **Makefile**, header file, and `main.c` structure
- [x] Custom structure `t_bash` for environment & shell state
- [x] Function to copy and manage `envp[]` locally
- [x] Custom `ft_getenv()` to access copied environment variables
- [x] `free_2d_array()` to free `char **` arrays (env, tokens, etc.)
- [x] `exit_failure()` to free memory and print error on failure
- [x] Skeleton structure for future command handling in `cmd/` directory
- [x] Readline is initialized and being tested
- [x] Modular code separation: `init_minishell()` and clean `main()`

### In progress:
- [ ] Readline loop with input capture
- [ ] Input tokenization (quotes, pipes, redirections)
- [ ] Command dispatching (builtin vs. external)
- [ ] Using copied `envp[]` for command resolution instead of system env
- [ ] Builtin command scaffolding (empty files created)

---

## 🧠 Notes & Brainstorm (TAHA & Team)

@TAHA REVIEW

vpath Makefile -MMD
echo flags: “-nnnn -n”

→ Tasks:
	1.	Review authorized functions (e.g., readline)
	2.	Link built-ins to function pointers
	3.	Signal handling: ctrl + C, ctrl + , ctrl + D
	4.	Export test: PATH=$PATH:/folder1:/folder2
→ access example: /usr/bin
	5.	Review Pipex bonus execution logic

---

## 👥 Contributors

This Minishell implementation is being developed by:

- `chrrodri`
- `bsamy`

Both students at **42 School Barcelona**.
