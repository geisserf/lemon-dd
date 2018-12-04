# LEMON-DD
Library for Edge-valued MONoid-based decision diagrams.

# Description
LEMON is a decision diagram library with a focus on providing the ability to define edge-valued decision diagrams.
This README is only a very brief overview. For more information please refer to the [wiki](https://github.com/geisserf/lemon-dd/wiki).

# Credits
## Contributors
- [Florian Geißer](https://cecs.anu.edu.au/people/florian-geisser) (core developer 2017-2019)
- [Benedict Wright](http://www2.informatik.uni-freiburg.de/~bwright/) (core developer 2017-2019)
- [David Speck](http://www2.informatik.uni-freiburg.de/~speckd/) (implemented variable ordering heuristic support, 2018)
- Ece Tavaslı (HiWi 2018)

## References
- For unit tests we rely on the [Catch](https://github.com/catchorg/Catch2) unit test framework
- Some parts of the code make use of the [Boost](https://www.boost.org/) library (see also LICENSE.txt)
- The underlying code for expressions (see also 'src/catamorph/') is based on the [deque.blog tutorial on catamorphisms](https://deque.blog/2017/01/30/catamorph-your-dsl-c-port/)

If you have contributed to LEMON as well and your name is not in this list (we apologize sincerely in this case, there surely was no harm intended!) please let us know via [e-mail](mailto:florian.geisser@anu.edu.au).
 

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
This is currenlty being worked on. See issue #1. For now, we recommend to just include the necessary headers into your project.

## Running lemon
For basic instructions on how to run lemon, see [Usage](https://github.com/geisserf/lemon-dd/wiki/Usage).
