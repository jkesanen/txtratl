# txtratl

A simple texture atlas creation library and tool.

The source is self-containing and the dependencies are bundled into `libs` directory.

This is not a serious best tool ever to generate texture atlases. This was originally a homework that was completed in a few hours. And later on updated to be a simple example project / memory pad for CMake configuration.

## Requirements

This software is build using CMake.

## Compiling

### Windows

Tested to compile on Visual Studio Community 2022.

* Create project files from sources for Visual Studio:

    `cmake -G "Visual Studio 17 2022" -B ..\build`

* Compile in the build directory:

    `MSBuild txtratl.sln`

### Linux

Tested to compile in Ubuntu GNU/Linux 22.04 with GCC 11.4.0.

* Create a Makefile from sources:

    `cmake -B ../build`

* Compile in the build directory:

    `make -j`

## Usage

Pass a path containing texture images in PNG and JPEG formats to the executable.

    txtratl ../images

Files `atlas.png` and `atlas.txt` are created into the working directory.

## Limitations

* Maximum side length for the atlas is fixed 4096 pixels.
* Only PNG output is supported.
* If images don't fit into 24000 x 24000 sized atlas, then nothing is generated.
* If you feed too many images to the program, it will run out of memory.
* Blitting 32-bit images requires SSE3 support.

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
