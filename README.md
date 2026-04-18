# Blackjack

A command-line Blackjack game written in C++20. Play against a dealer in the terminal with ANSI-colored output and two card display modes — compact inline text or full ASCII art card boxes.

## Features

- Standard Blackjack rules: hit or stand against a dealer
- Player starts with a **$1,000** balance; place a bet before each round
- Two display modes selectable at launch:
  - `--compact` — inline card notation (e.g., `A♠  K♥`)
  - `--full` — ASCII art card boxes with ANSI color-coded suits
- ANSI color highlighting for suits, status messages, and win/loss results
- Dealer follows standard rules (stands on 17+)

## Requirements

| Platform | Toolchain |
|---|---|
| Linux / macOS | `g++` ≥ 10 or `clang++` ≥ 12 with C++20 support; GNU `make` |
| Windows (MSVC) | Visual Studio 2019 or 2022 (any edition) with the **Desktop development with C++** workload installed |
| Windows (MinGW / WSL) | MinGW-w64 `g++` or WSL `g++` with C++20 support; GNU `make` |

## Project Structure

```
Blackjack/
├── src/
│   ├── Blackjack.cpp    # Entry point — game loop, betting logic, CLI flag parsing
│   ├── Card.cpp/.h      # Card class (rank value + suit character)
│   ├── Deck.cpp/.h      # Deck class — 52-card construction and shuffle/deal
│   ├── Hand.cpp/.h      # Hand class — card collection and score calculation
│   ├── GameLogic.cpp/.h # Game logic — dealer hit rules
│   └── Display.cpp/.h   # Display class — ANSI rendering, card art, screen management
├── tests/
│   ├── doctest.h        # doctest v2 single-header unit test framework
│   ├── test_card.cpp    # Unit tests for the Card class
│   ├── test_deck.cpp    # Unit tests for the Deck class
│   ├── test_hand.cpp    # Unit tests for the Hand class and score calculation
│   └── test_bet.cpp     # Integration tests for bet input validation and dealer logic
├── output/              # Compiled binaries and object files (created during build)
├── design/              # UML/design diagrams
├── Makefile             # Linux / macOS / MinGW build file (GNU make + g++)
├── Makefile.win         # Windows NMake build file (MSVC cl.exe)
└── build.bat            # Windows convenience wrapper — auto-detects Visual Studio
```

## Building

### Linux and macOS

Requires `g++` (or `clang++`) and GNU `make`.

```bash
# Build
make

# Build and run immediately
make run

# Remove compiled objects and binary
make clean
```

To use `clang++` instead of `g++`:

```bash
make CXX=clang++
```

### Windows — MSVC (recommended)

**Option A: `build.bat` (easiest)**

Open any Command Prompt (no special setup needed). `build.bat` automatically locates Visual Studio 2019 or 2022 and initializes the build environment for you.

```bat
:: Build
build.bat

:: Build and run immediately
build.bat run

:: Remove compiled objects and binary
build.bat clean
```

**Option B: NMake from a Developer Command Prompt**

Open a **Developer Command Prompt for VS 2019/2022** (found in the Start menu under your Visual Studio installation), then:

```bat
:: Build
nmake /f Makefile.win

:: Build and run immediately
nmake /f Makefile.win run

:: Remove compiled objects and binary
nmake /f Makefile.win clean
```

**Option C: Windows Terminal / PowerShell**

`build.bat` works directly in PowerShell — it handles Visual Studio environment setup internally, so no additional configuration is required.

Open **Windows Terminal** (PowerShell tab) in the repo directory and run:

```powershell
# Build
.\build.bat

# Build and run immediately
.\build.bat run

# Remove compiled objects and binary
.\build.bat clean
```

If you prefer to call `nmake` directly from PowerShell (e.g. for scripting), load the VS Developer Shell module first:

```powershell
# Adjust the path to match your Visual Studio edition and version
$vsPath = "C:\Program Files\Microsoft Visual Studio\2022\Community"
Import-Module "$vsPath\Common7\Tools\Microsoft.VisualStudio.DevShell.dll"
Enter-VsDevShell -VsInstallPath $vsPath -SkipAutomaticLocation -DevCmdArguments "-arch=x64"

# Then build with nmake as normal
nmake /f Makefile.win
nmake /f Makefile.win run
nmake /f Makefile.win clean
```

> **Tip:** To open Windows Terminal in the repo folder from File Explorer, right-click the folder and choose **Open in Terminal** (Windows 11) or hold **Shift** and right-click then choose **Open PowerShell window here** (Windows 10).

### Windows — MinGW or WSL

Use the standard `Makefile` with GNU `make`:

```bash
make
make run
make clean
```

## Unit Tests

Tests are written using [doctest](https://github.com/doctest/doctest) (v2, single-header), located in the `tests/` directory. Each test suite builds into its own standalone executable.

| Test binary (Linux) | Test binary (Windows) | Source | What is tested |
|---|---|---|---|
| `test_card_tests` | `test_card.exe` | `test_card.cpp` | `Card` construction, value and suit accessors |
| `deck_tests` | `test_deck.exe` | `test_deck.cpp` | `Deck` deals exactly 52 valid cards; sentinel on empty deck |
| `hand_tests` | `test_hand.exe` | `test_hand.cpp` | `Hand` scoring — number cards, face cards, aces (soft/hard) |
| `bet_tests` | `test_bet.exe` | `test_bet.cpp` | Bet input validation (alpha, negative, overflow); dealer hit logic |

### Running the tests

**Linux / macOS / MinGW:**

```bash
make test
```

This builds all four test executables and runs them in sequence. All results are reported by doctest to stdout.

**Windows (`build.bat`):**

```bat
build.bat test
```

**Windows (NMake):**

```bat
nmake /f Makefile.win test
```

### Adding new tests

1. Create a new `.cpp` file in `tests/` with `#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN` and `#include "doctest.h"`.
2. Add a new `TEST_TARGET` / `TEST_OBJS` pair in `Makefile` (and `Makefile.win`) following the existing pattern.
3. Add the new target to the `test:` phony rule dependencies and run line.

## Running

The compiled binary is placed in the `output/` directory:

```
output/blackjack        # Linux / macOS
output/blackjack.exe    # Windows
```

**Command-line flags** (optional):

| Flag | Description |
|---|---|
| `--compact` | Compact inline card display (default) |
| `--full` | Full ASCII art card boxes with colored suits |

Examples:

```bash
# Compact display (default)
./output/blackjack        # Linux / macOS
output\blackjack.exe      # Windows

# Full ASCII art card display
./output/blackjack --full
output\blackjack.exe --full
```

## Gameplay

1. The game starts with a **$1,000** balance.
2. Before each round, enter your **bet** (must be within your current balance).
3. You and the dealer are each dealt two cards; the dealer's first card is hidden.
4. Choose to **Hit** (draw a card) or **Stand** (end your turn).
5. Going over **21** is a bust — you lose the bet.
6. The dealer reveals their hidden card and draws until reaching 17 or higher.
7. The higher hand (without busting) wins. Ties are not implemented — the dealer wins ties.
8. After each round, choose to play another hand or quit.
9. The game ends when you choose to stop; your final balance is displayed.
