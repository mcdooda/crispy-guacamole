# Crispy Guacamole
__Crispy guacamole__ *(code name)* is an old school 2D real time strategy game.
It comes with an easy to use map editor.

<p align="center">
	<img src="https://raw.githubusercontent.com/mcdooda/crispy-guacamole/master/preview.gif" alt="Crispy guacamole screenshot" />
</p>
<p align="center"><em>Smell that guacamole?</em></p>

# How to
## Windows
__Crispy guacamole__ can be compiled and ran using the dedicated [windows scripts](https://github.com/mcdooda/crispy-guacamole-env-windows).

The only requirements are git and [Visual Studio 2017](https://www.visualstudio.com/vs/whatsnew/).

### Compiling
- Clone https://github.com/mcdooda/crispy-guacamole-env-windows.git somewhere on your computer.
- Run install.bat. It will get the latest source code and data, and compile the game.

### Usage
- Run game.bat to open the game
- Run editor.bat to open the map editor

## Linux
Compile and run __Crispy guacamole__ linux-style:

### Compiling
In order to compile __Crispy guacamole__, these softwares are required: cmake, make, g++.

You also need to install these development libraries: lua5.3, SDL2, SDL2_image, SDL2_mixer, SDL2_ttf, GL, GLEW.

Then, open a terminal and type the following:
```Shell
git clone https://github.com/mcdooda/crispy-guacamole.git
cd crispy-guacamole
cmake . -G "Unix Makefiles"
make
```

### Usage
Open the map *map1* in the mod *crispy-guacamole*
```Shell
./crispy-guacamole mods/crispy-guacamole map1
```
- Add `-e` to open the map in the map editor
- Add `-w` to open the game in window mode instead of fullscreen

## Mac OS
Sadly, Mac OS is not supported as the time. Feel free to contribute and help up bring the game to more platforms.
