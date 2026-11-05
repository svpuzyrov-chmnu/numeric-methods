## Set of libraries and examples to use numerical methods with C++ implementation.

### Plot2D
Plot2D is a library for creating 2D plots in C++.
It contains generic interfaces and under the hood uses [sciplot library](https://github.com/sciplot/sciplot)

To use this library need to install `gnuplot` library
* Windows - use [Msys2](https://www.msys2.org/) and install the following packages:
and setup Msys2 and install the following packages:
```bash        
   pacman -S mingw-w64-ucrt-x86_64-gnuplot
```
* Ubuntu - use the following command to install gnuplot:
```bash
sudo apt install -y gnuplot-qt
```
* MacOS - use the following command to install gnuplot:
```bash
brew install gnuplot
```