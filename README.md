# lemon-dd
Library for Edge-valued MONoid-based decision diagrams.

# Description
LeMON is a decision diagram library with a focus on providing the ability to define edge-valued decision diagrams.
This README is only a very brief overview. For more information please refer to the (wiki)[https://github.com/geisserf/lemon-dd/wiki/Overview].

# Running the library
## Supported platforms

### Linux
Linux is the main platform for which the library is developed. All features should work without restrictions under Linux.

### Mac OS X
The library was successful compiled with Clang under Mac OS X. There may still arise issues, as we do not develop under this platform. If you have any issues or questions contact us.

### Windows
We currently do not provide Windows support.

## Dependencies
- *CMake >= 3.2*
- A *C++ 14 compatible compiler*
- *Python* to run the scripts
- If you want to generate and view decision diagrams we recommend *xdot*

## Obtaining the code
The command `git clone --recursive https://github.com/geisserf/lemon-dd.git DIRNAME` will create a clone of the master repository in directory DIRNAME and also clones all necessary submodules.

**If you cloned the repository without --recursive, make sure that all submodules are initialized.** To initialize and clone submodules use the commands `git submodule init` and `git submodule update`.

## Compiling lemon
The lemon executable allows to generate decision diagrams for given functions. To build lemon, run `./build.py`. This will create the default build *release32* in *builds/*. Running `./run_tests.sh` will compile the debug version and execute available unit tests.
We recommend to execute all unit tests in debug mode at least one time, to make sure that everything works.

## Adding lemon as a library to external projects
This is currenlty being worked on. See issue #1.

## Running lemon
For basic instructions on how to run lemon, see [Usage](Usage).
