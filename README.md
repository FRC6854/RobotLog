# RobotLog - A simple FRC path planning tool

## How to build

### Build dependencies on Debian/Ubuntu
g++ make pkg-config libgtkmm-3.0-dev

### Build dependencies on Windows MSYS2
mingw-w64-x86_64-gcc mingw-w64-pkg-config mingw-w64-x86_64-gtkmm3

### List of Makefile targets
For build using g++ use `make all`  
For build using clang++ use `make clang`  
For build under MSYS2 and make exe file run under Win32 GUI subsystem `make win`  
Use `make run` or `./robotlog` to start the program  
