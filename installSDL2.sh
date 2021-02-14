#!/bin/bash
cd lib/SDL2/
./configure
make
make install
cd ../SDL2_image/
./configure
make
make install
cd ../SDL2_ttf/
./configure
make
make install