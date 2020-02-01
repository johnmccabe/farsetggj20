# farsetggj20

## SDL2

```
mkdir build
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=../32blit/darwin.toolchain
make
```

## STM32 on OSX

```
docker run -ti -v ${PWD}:/farsetggj20 johnmccabe/32blit-stm-build bash
mkdir build.stm32
cd build.stm32
cmake .. -DCMAKE_TOOLCHAIN_FILE=../32blit/darwin.toolchain
make
```

## Create asset map

```
pip3 install bitstring
pip3 install construct
pip3 install pillow

./32blit/tools/sprite-builder --tilesizex=8 --tilesizey=8 packed ./art/greta/greta-thunberg-arms-crossed-spritesheet.png > greta.hpp
```