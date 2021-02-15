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
cd ../lua-5.3.0/
make macosx
make local
cd ../sol2-develop/single
python3 single.py
echo "Libraries are READY! \<@^@>/"