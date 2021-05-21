# PokemonTypeCoverageBuilder
A simple program meant to facilitate Pokémon moveset creation by determining which type will improve coverage the most.

Installation instructions:
You are going to compile the program from the source code. If you are using Windows or Linux, the more detailed instructions that follow will tell you how to do this. I am not qualified to give you instructions for compiling the program on Mac, since I don't have one.

1. Download the source and, if you don't already have one, install a c++ compiler, such as g++.
  Here are instructions on how to install g++:
    Windows: Follow the instructions at https://techsupportwhale.com/install-gcc-compiler-on-windows/
    Linux: Simply run the command "sudo apt install g++". You need to be an administrator or have root access.
2. Open a command prompt, navigate to the folder the source code is in (unzip first if it is a zipped folder) and run this command (if using g++; with another compiler the command might be different):
  Windows: g++ -o PokemonTypeCoverageBuilder.exe PokemonTypeCoverageBuilder.cpp
  Linux: g++ -o PokemonTypeCoverageBuilder PokemonTypeCoverageBuilder.cpp
3. Move the generated executable to wherever on your computer you want it.
