# CASH (C Assignment SHell)
by Junhee Lee
## Features
Checks if commands issued exist
Allows simple redirection (<, >, >>)
Allows multiple pipes using |
Displays current directory when prompting for a command
Allows for directory inputs to begin with ~ for the home directory
## Attempted
Delimiting quotes using quotations (was deleted because it required significant changes to the parse_args command)
## Bugs
Redirecting stdin from a file (e.g. ./cash < input) will cause the issued commands to not be visible
Pressing tab glitches the program so some characters from the input cannot be deleted but others not from the input can be
Pressing arrow keys simply enters characters into the command line
## Files & Function Headers
#### shio.c
Handles user input and parsing
###### char * get_input()
#### shlib.c
Handles forking and executing of commands as well as redirecting stdin/stdout
#### dir.c
Handles directory home shortcuts functionality
