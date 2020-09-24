# OS Assignment 2

# Files:
## main.c
- execute: Execute non-builtin functions (called by main)
- printexit: Print for when SIGCHLD is encountered (background process is closed, called by signal inside main)
- main: Main REPL loop, basic parsing and calling functions.

## ls.c
- lsdo: Print output, given file/folder and flags (called by ls)
- ls: Parse input, set flags and call lsdo (called by main)

## pinfo.c
- pinfo: Print pinfo (called by main)

## cd.c
- cd: Change directory (called by main)

## echo.c
- echo: Print arguments (called by main)

## shell.h
- Contains all the function prototypes

## makefile
- Compiles the code into ./shell 