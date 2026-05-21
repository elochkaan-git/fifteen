# Fifteen

The "15-puzzle" game featuring two interfaces: a modern graphical interface powered by **Qt6 / QML** and a classic terminal-based interface using **Ncurses**.

## Project Architecture

The project is split into three main components:

* `libfifteen` — A static library containing the core game logic (engine, cell, and board factories).
* `appfifteen` — The graphical user interface (GUI) application built with **Qt6 Quick / QML**.
* `cfifteen` — The command-line interface (CLI) version of the game for terminals.

---

## Prerequisites (Debian)

Since the project relies on Qt6 and Curses components, you need to install the appropriate development packages before building.

Run the following command in your terminal to install all required dependencies on Debian (or Ubuntu):

```bash
sudo apt update
sudo apt install build-essential cmake qt6-base-dev qt6-declarative-dev libncurses5-dev libncursesw5-dev
```

*Minimum requirements:*

* **CMake**: version `3.16` or higher.
* **Compiler**: A compiler supporting the **C++17** standard (e.g., GCC 9+).
* **Qt**: version `6.8` or higher.

---

## Building the Project

The project uses a standard CMake build workflow:

1. Clone the repository and navigate to the project root directory.
2. Create a build directory and enter it:
```bash
mkdir build && cd build
```

3. Configure the project:
```bash
cmake ..
```

4. Build the project:
```bash
cmake --build .
```

Once the compilation is complete, you will find two executable files in the `build` directory: `appfifteen` and `cfifteen`.

---

## Running the Game

### Graphical User Interface (Qt6)

To launch the game with a full GUI (menus and the game board):

```bash
./appfifteen
```

### Command-Line Interface (CLI)

To play the classic text-based version directly inside your terminal emulator:

```bash
./cfifteen
```
