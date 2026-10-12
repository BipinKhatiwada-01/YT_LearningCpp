# C++ on Linux — Cheatsheet

Reference commands for building, debugging, and troubleshooting C++ as you move through the playlist. Grouped so you can skim to the section you need.

---

## 1. Compiling (g++)

```bash
g++ -g -O0 -std=c++17 -Wall -Wextra main.cpp -o main
```

| Flag | Meaning |
|---|---|
| `-g` | Include debug symbols (required for gdb/breakpoints) |
| `-O0` / `-O1` / `-O2` / `-O3` | Optimization level. `-O0` = none (best for debugging), `-O2`/`-O3` = production speed |
| `-std=c++17` / `c++20` / `c++23` | Language standard. Cherno's videos assume newer standards as the series progresses |
| `-Wall -Wextra` | Turn on most useful warnings — treat warnings as bugs waiting to happen |
| `-Wpedantic` | Extra strict standard-compliance warnings |
| `-c` | Compile to `.o` object file only, don't link |
| `-o <name>` | Output filename |
| `-I<path>` | Add a header search directory |
| `-L<path>` | Add a library search directory |
| `-l<name>` | Link against `lib<name>.so`/`.a` (e.g. `-lm` for math, `-lpthread` for threads) |
| `-fsanitize=address,undefined` | Catch memory bugs and undefined behavior at runtime (see §3) |

Multiple files → one binary:
```bash
g++ -g -O0 -std=c++17 main.cpp helper.cpp -o output/program
```

---

## 2. Debugging without the IDE (raw gdb)

Useful when you're on a remote box, in a terminal-only session, or the IDE debugger misbehaves.

```bash
gdb ./program
```

| Command | Does |
|---|---|
| `b main` / `b file.cpp:42` | Set breakpoint at function or line |
| `r` | Run |
| `n` | Next line (step over) |
| `s` | Step into function call |
| `c` | Continue to next breakpoint |
| `p varname` | Print a variable's value |
| `bt` | Backtrace / call stack |
| `l` | List source around current line |
| `watch varname` | Break whenever this variable changes |
| `info locals` | Show all local variables |
| `q` | Quit |

In VS Code's debug UI, all of the above map to Watch/Variables/Call Stack panels and F9/F10/F11/F5 — same underlying gdb, nicer buttons.

---

## 3. Catching memory bugs

Two options, don't run both on the same binary at once:

**AddressSanitizer / UBSan** (fast, needs a rebuild):
```bash
g++ -g -fsanitize=address,undefined -std=c++17 main.cpp -o main
./main
```
Crashes immediately with a readable report pointing at the exact line — good default while learning pointers/arrays.

**Valgrind** (slower, no rebuild needed, catches more categories of leak):
```bash
valgrind --leak-check=full ./main
```
(install via your package manager if it's missing — `valgrind` is the package name everywhere)

---

## 4. Build systems

**Make** — minimal `Makefile`:
```makefile
program: main.cpp
	g++ -g -O0 -std=c++17 -Wall -Wextra main.cpp -o program

clean:
	rm -f program
```
```bash
make          # builds
make clean    # removes binary
```

**CMake** — the version you'll want once a topic has multiple files/libraries:
```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
./build/program_name
```

---

## 5. Inspecting a compiled binary

| Command | Shows |
|---|---|
| `file program` | What kind of binary it is (32/64-bit, stripped, etc.) |
| `nm program` | Symbol table (function/variable names) |
| `objdump -d program` | Disassembly |
| `readelf -h program` | ELF header info |
| `ldd program` | Which shared libraries it needs at runtime |
| `strings program` | Printable text embedded in the binary |
| `c++filt _Z3fooi` | Demangle a C++ symbol name into human-readable form |

---

## 6. Static analysis & formatting

Install `cppcheck` and `clang-tools-extra` (or your distro's equivalent package name for clang-tidy/clang-format) via your package manager, then:
```bash
cppcheck main.cpp                 # catches likely bugs before you even run it
clang-tidy main.cpp --            # deeper style/bug checks
clang-format -i main.cpp          # auto-format in place
```
`clang-format` needs a `.clang-format` file in your project root to pick a style, otherwise it uses LLVM's default.

---

## 7. Installing libraries

Package names and search syntax differ by distro/package manager — the commands below assume you know which one you're on (`dnf`, `apt`, `pacman`, etc.), so swap the command name in but keep the logic:

```bash
<pkg-manager> search <name>            # find a package
<pkg-manager> install <name>-dev(el)   # dev packages (headers + linker files) usually end in -dev or -devel
pkg-config --cflags --libs <lib>       # get the exact -I/-l flags a library needs, once installed
```
Common ones you'll likely hit later in the series: Boost, SDL2, GLFW — just search your package manager for the dev/devel package of each.

---

## 8. General Linux commands worth knowing for this workflow

| Command | For |
|---|---|
| `grep -rn "text" .` | Search all files recursively, show line numbers |
| `find . -name "*.cpp"` | Find files by pattern |
| `tree -L 2` | Visualize folder structure (install `tree` via your package manager if missing) |
| `chmod +x program` | Make a binary executable, if it complains it can't run |
| `strace ./program` | Trace every syscall — good for "why is this hanging" |
| `time ./program` | Measure how long a run takes |
| `du -sh folder/` | Folder size, human-readable |

---

**Caveat**: sanitizers, valgrind, and gdb all slow a program down significantly — normal, don't mistake it for a real performance bug. And `-O0` debug builds will always run slower than `-O2` release builds; that gap is expected, not something to "fix."
