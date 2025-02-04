#!/bin/sh
#This is a batch file to build and package the Game, simply run this and all important information should be output to the terminal
# Authored By: De'Lano Wilcox
# Notes: If you're reading this; If there are any errors that stop the game from packaging copy the output in the terminal and send it to me

PWD="$(pwd)"

if [ -f python ]; then
	./PackageGame.py PWD/IronDefiance.uproject PWD/Content/IronDefiance/Maps/Levels/

else
	echo "Python not installed on this machine. Please visit https://www.python.org/downloads/macos/ and download the latests Stable Release!"
fi
