# CASH (C Assignment SHell)
by Junhee Lee
## Features
Checks if commands issued exist
Allows simple redirection (<, >, >>)
Allows multiple pipes using |
Displays current directory when prompting for a command
Allows for directory inputs to begin with ~ for the home directory
Ignores extra spaces
## Attempted
Delimiting quotes using quotations (was deleted because it required significant changes to the parse_args command)
## Bugs
Redirecting stdin from a file (e.g. ./cash < input) will cause the issued commands to not be visible
Pressing tab glitches the program so some characters from the input cannot be deleted but others not from the input can be
Pressing arrow keys simply enters characters into the command line
## Files & Function Headers
### shio.c
Handles user input and parsing
##### char * get_input()
###### Inputs:
None
###### Returns:
Pointer to the beginning of the line
###### Other:
Prints out the current working directory
Removes the terminating newline character
##### char *** parse_args(char * line)
###### Inputs:
Pointer to the beginning of the line
###### Returns:
Pointer to an array of a commands to issue, in the form of arrays of strings
###### Other:
Each pointer points to an array of commands to issue via execvp
The array terminates in a NULL pointer
##### void print_home()
###### Inputs:
None
###### Returns:
None
###### Other:
Prints out the current directory, using ~ if it is a subdirectory of the home directory
##### char redir(char ** args)
###### Inputs:
Pointer to an array of arguments for a command
###### Returns:
An indicator of whether the stdin, stdout, or both were redirected
###### Other:
Redirects stdin and stdout based on the first argument requiring the redirection of stdin and the first of stdout. Other redirection arguments are deleted from the array.
Creates stdout if needed
##### void rmempty(char ** args)
###### Inputs:
Pointer to an array of arguments for a command (must be to allocated memory)
###### Returns:
None
###### Other:
Removes all empty strings from the array
##### char ** nonnull(char ** args)
###### Inputs:
Pointer to an array of arguments for a command
###### Returns:
Pointer to an array of arguments for a command containing no empty strings
### shlib.c
Handles forking and executing of commands as well as redirecting stdin/stdout
##### int run(char *** args)
###### Inputs:
An array of commands to execute
###### Returns:
None
###### Other:
Forks and executes each command based on pointers, ending at the NULL pointer
Frees each memory allocation used in the storage of commands
##### void execute(char ** args)
###### Inputs:
An array of arguments detailing the command
###### Returns:
None
###### Other:
Separates commands by the pipe character and executes, creating pipes at each pipe character
##### void parse_exec(char ** args, int r, int w)
###### Inputs:
Pointer to an array of arguments of a command
File descriptor of stdin
File descriptor of stdout
###### Returns:
None
###### Other:
Sets the stdin and stdout to r and w if specified, else the stdin and stdout specified by the commands, else doesn't change stdin and stdout
Forks and checks if command given exists
Executes the command given
### dir.c
Handles directory home shortcuts functionality and changing directories
##### void chcwd(char * path)
###### Inputs:
A string that represents the desired working directory
###### Returns:
None
###### Other:
Changes working directory into the path given with any ~ replaced by the home directory
##### void homify(char * path)
###### Inputs:
A string representing a path
###### Returns:
None
###### Other:
If the path starts with the home directory, it is replaced with ~
