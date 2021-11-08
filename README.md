# Ghostless Pacman
Pacman without the ghosts!

## Installation
### Using Installer
Head to [Releases](https://github.com/D4N13LxD4V1D/ghostless-pacman-gui/releases "Ghostless Pacman Releases") to find the latest working installer for the game.

### Building from Source
#### Requirements
You Need to have **MinGW** installed and properly set-up to be able to build and run the game from source.

#### Instructions
To build and run the code from source, follow the following steps:
1. Download the source of the game by cloning this repository or by downloading it as ZIP file and extracting it to your preffered directory.
2. Head inside the cloned/extracted folder and run the **"run.bat"** file. The batch file will execute the following commands inside the directory:
```batch
gcc -std=c17 "Ghostless Pacman.c" -ISDL2\include -LD:SDL2\lib -Wall -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -o "Ghostless Pacman" my.res -mwindows

".\Ghostless Pacman.exe"
``` 

## Gameplay
-- TODO --
