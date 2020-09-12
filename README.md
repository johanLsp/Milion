# Milion

[![Build Status](https://travis-ci.org/johanLsp/Milion.svg?branch=master)](https://travis-ci.org/johanLsp/Milion) [![Build status](https://ci.appveyor.com/api/projects/status/7wg8lmslg1ycmj5w?svg=true)](https://ci.appveyor.com/project/johanLsp/milion)


Milion is a free experimental synthesizer plugin based on FM and Formant synthesis.

It is available as a VST/VST3/AU plugin or as a standalone application for OSX, Windows & Linux

## Build this project
This repository relies on submodules. To recursively clone all submodules:
```
git clone --recurse-submodules https://github.com/johanLsp/Milion.git
```

The project is based on CMake and suppports Linux/Macos/Windows. To build it, run:
```
mkdir build && cd build
cmake ..
make
```
