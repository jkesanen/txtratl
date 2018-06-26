# txtratl

A simple texture atlas creation tool written in C++11

The tool's source is self-containing in sense that the dependencies are bundled
in into `vendor` directory.

## Requirements

This software is build using CMake. Download Windows CMake from [CMake homepage](https://cmake.org/download/)

On Ubuntu GNU/Linux CMake can be installed with apt:

    sudo apt install cmake

## Compiling

### Windows

Tested to compile on Visual Studio 2015 Express.

* Create a build directory:

    `mkdir build`
    `cd build`

* Create a project file from sources for Visual Studio:

    `cmake -G "Visual Studio 14 Win64" ..\txtratl\src`

* Compile from command line:

    `MSBuild txtratl.sln`

### Linux

Tested to compile in Ubuntu GNU/Linux 18.04 with GCC 7.3.0.

* Create a build directory:

    `mkdir build && cd build`

* Create a Makefile from sources:

    `cmake ../txtratl/src`

* Compile:

    `make`

## Usage

Pass a path containing texture images in PNG and JPEG format to the executable.

    txtratl ../images

Files `atlas.jpg` and `atlas.txt` are created into the working directory.

## Limitations

There are many.

* Maximum side length for the atlas is fixed 4096 pixels
* Only JPEG output is supported (lossy, no alpha channel) due to zupply's PNG writing issues
* If images don't fit into 4096 x 4096 sized atlas, then nothing is generated
* If you feed too many images to the program, it will run out of memory
* Blitting 32-bit images requires SSE3 support

## Sample output

This sample atlas is created out of 11 png and jpeg files.

![atlas.jpg](https://jkesanen.github.io/txtratl/atlas_sample.jpg)

```
asphalt1.jpg	1024	420	128	128
snow1.jpg	0	256	512	512
sakura1.png	932	768	256	400
sakura2.png	0	768	512	128
sand1.png	512	768	420	319
sand2.jpg	1024	548	64	64
sky1.png	0	0	893	256
stone1.png	512	256	512	512
tree1.jpg	1024	256	164	164
treesnow1.png	893	0	256	256
water1.png	0	896	512	256
```
