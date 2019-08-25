# Crispy Guacamole
__Crispy guacamole__ *(code name)* is an old school 2D real time strategy game.
It comes with an easy to use map editor.

<p align="center">
	<img src="https://raw.githubusercontent.com/mcdooda/crispy-guacamole/master/gifs/preview.gif" alt="Crispy guacamole screenshot" />
</p>

# How to
## Windows
__Crispy guacamole__ can be compiled and ran using the dedicated [windows scripts](https://github.com/mcdooda/crispy-guacamole-env-windows).

The only requirements are [Visual Studio 2017](https://www.visualstudio.com/vs/whatsnew/), git and cmake.
Hopefully, the latters come prepackaged along Visual Studio 2017.

You will need to add them to the __PATH__ environment variable. They are located there by default:
- C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\Common7\IDE
- C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\Common7\IDE\Extensions\eysyln0k.p1s\Git\mingw32\bin *(only if the git extension is installed, another installation of git works fine too)*
- C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin


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
