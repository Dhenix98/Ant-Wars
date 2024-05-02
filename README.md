# Ant-Wars
This repository is heavily inspired by the game genre "Tentacle Wars", using SDL as a way to both study the SDL engine and the C++ language itself.

## Requirements

 * [SDL2](https://github.com/libsdl-org/SDL)
 * [SDL2_image](https://github.com/libsdl-org/SDL_image)
 * [SDL2_ttf](https://github.com/libsdl-org/SDL_ttf)
 * [SDL2_mixer](https://github.com/libsdl-org/SDL_mixer)

## Compilation

For UNIX platforms, is available a [Makefile](Makefile) to compile the game with the command: 
```sh
make compile
```

## Documentation

To generate documentation Doxygen is required ([Windows](https://www.doxygen.nl/manual/install.html#install_src_windows) and [Unix](https://www.doxygen.nl/manual/install.html#install_src_unix) links)

```sh
doxygen Doxyfile
```

Alternatively there is option through makefile (still requires Doxygen)
```sh
make doc
```

## Execution

To run the game, type the command in a terminal: 
```sh
./Ant-Wars
```
