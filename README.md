Doxygenmd
=========

| **Linux + Mac** |
|-----------------|
| [![Build status](https://travis-ci.com/d99kris/doxygenmd.svg?branch=master)](https://travis-ci.com/d99kris/doxygenmd) |

Doxygenmd is a simple tool for generating markdown API documentation
from source code.

Supported languages: C++


Example Usage
=============

    $ doxygenmd src doc


Supported Platforms
===================

Doxygenmd is developed and tested on Linux and macOS. Current version has been
tested on:

- macOS 10.15 Catalina
- Ubuntu 20.04 LTS


Build / Install
===============

Linux / Ubuntu
--------------

**Dependencies**

    sudo apt install bison flex git

**Source**

    git clone https://github.com/d99kris/doxygenmd && cd doxygenmd

**Build**

    mkdir -p build && cd build && cmake .. && make -s

**Install**

    sudo make install

macOS
-----

**Dependencies**

    brew install bison flex

**Source**

    git clone https://github.com/d99kris/doxygenmd && cd doxygenmd

**Build**

    mkdir -p build && cd build && cmake .. && make -s

**Install**

    make install


Technical Details
=================
Doxygenmd is implemented in Python. It uses Doxygen to parse source code
and generate man-pages, which it subsequently processes and converts into
Markdown.

The source tree includes the source code of the following third-party
softwares:

- [Doxygen](https://github.com/doxygen/doxygen) - GPLv2 License
  Copyright (C) by Dimitri van Heesch


License
=======
Doxygenmd is distributed under the GPLv2 license. See [LICENSE](/LICENSE) file.


Keywords
========
api documentation, doxygen, generate, markdown.

