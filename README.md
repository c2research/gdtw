# GDTWSystem

## why

This project allows you to use warped versions of any distance! This is a powerful capability and opens avenues for further research. The process of adding a new distance and using it is fast and easy (described below).

## datasets

All the datasets can be found at http://www.cs.ucr.edu/~eamonn/time_series_data/
for Time Series Classification experiment as described in Section 5.2.1 of paper, dataset must have 
.mat" extension. Following link explains how to convert the text file in .mat file. 
https://www.mathworks.com/help/matlab/ref/save.html
Rest of the experiments use datasets as text files delimited by ' '.

Use case Study on Heart Arrythmia uses the dataset from "http://physionet.org/cgi-bin/atm/ATM"


## install build dependencies

### macOS:

```bash
  brew install cmake
  brew install boost
  brew install readline
```

### Linux:

```bash
  sudo apt-get install cmake
  sudo apt-get install boost
  sudo apt-get install readline
```

### Windows:

- Install [CMake for Windows](https://cmake.org/download/). Make sure that the **bin** directory of CMake is in your PATH environment variable.
- Install [MinGW Distro with Boost](https://nuwen.net/mingw.html) at **C:\\**
- Install [WinEditLine](https://sourceforge.net/projects/mingweditline/files/) at **C:\\**
- Copy and replace **C:\wineditline\include\editline\readline.h** by the **readline.h** in this repository.

## building the system

### Linux & macOS
The executable online will be in the build folder.
Tests will also be run as part of the build script.

```bash
  ./run.sh
```

### Windows

Change current directory to this repository and run the following commands in **cmd**
```
mkdir build
cd build
cmake ..
make
```
## matlab extension

If you prefer to interface with the project in matlab rather than in c++, we support this as well! In order to compile the executable file used by matlab you must get a license for matlab, and then find your instances root location. You can do this via the matlab IDE by running the command `matlabroot`. From here adjust the field Matlab_ROOT_DIR in the upper-level CMakeLists.txt. Assuming that you have Boost installed as well, you will generate a .mex* file (exact extension depends on your architecture) when you `./run.sh`. This file will appear in your build folder. Add it to your path (most easily done via the matlab IDE) and then you will be able to use the dtw with any distance function. An example of how to do this is in the directory `experiments`. You can also see `MatlabMex.cpp` documentation for details on function usage.

In short:
* install matlab
* add matlab root to CMakeLists.txt at project level, found by running `matlabroot` in the ide
* build project as usual `./run.sh`

### adding new distances

#### mathematical derivation

As described in (link to paper), define a non-decreasing distance in terms of a recursive function. As part of this process you need to define what is neccesary to normalize your result. Furthermore, you need to store the information necessary to compute your distance recursively. These ideas are discussed in detail in the paper and are necessary for moving forward. Concrete *code* examples are given in folder `src/distance/`, and a description coming up next.

#### creating a distance

* Navigate to the folder `lib/distance/`
* Duplicate a distance function header file to work with
  * remember to rename class and header file and the header guards
* If you are creating a complicated distance and you need to cache more than one numeric value use `Cosine.hpp` or `Sorensen.hpp` as your example. Instead of storing one value to compute the distance you may need to store several. Both the `cosine` and `sorensen` need two. More details next!
* Implement the `init` function
  * This function determines the first value in the recursive process of calculating a distance
  * Its generally zero and so usually will not need to be changed
  * If you need more *memory* values to calculate your distance, you may instead return any value of any type. You will need to update the return types of this function and the parameters of `reduce` and `norm` accordingly.
* Implement the `reduce` function
  * This is the critical part of the process
  * Its the recursive step of the algorithm. It must calculate the distance between `x` and `y` and then store whatever information necessary to compute the distance given your previous computation. This value will be of the same type you chose for your `init` function.
* Implement the `norm` function
  * There are two critical pieces to this function
    * (a) it normalizes distance as you wish it to (divide by n, 2, etc)
    * (b) Converts from your chosen type in `init` to a data_t, (numeric value).
  * There is room to experiment here. Its inputs are references to both TimeSeries you are finding the distance between and the total distance so far. A good starting point is computing the raw distance and dividing by the average of the lengths between the timeseries.
  * Note: return a data_t

#### add distance to list

* Following the examples, in `Distance.hpp` add your distance to both gAllDistance and gAllDistanceName.
* Include the header file you created in this file as well.

#### finished

See notes on how to run the CLI below.

### CLI documentation

See source for documented functions. The CLI itself provides help functions.

```bash
./build/online
help      # shows available functions
help best # shows detailed help for distance
...       # the fun begins
```

### additional notes

We define a type data_t as a floating point number with precision that you can change dependent on your needs.

### pull requests

We're excited to see others implement their own distance functions and add those distances to library of distances. To do this, please document your work using doxygen style and add tests to `test/distance`.

### contact

If any issues come up please reach out to us at `genex@wpi.edu`.