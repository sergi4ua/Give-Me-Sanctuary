#!/usr/bin/env bash

# exit when any command fails
set -euo pipefail

#set compiler params
export TARGET='m68k-amigaos'
export SYSROOT=/opt/$TARGET
export M68K_CPU=68040
export M68K_FPU=hard
export M68K_CPU_FPU="-m${M68K_CPU} -m${M68K_FPU}-float"
export M68K_COMMON="-s -ffast-math -fomit-frame-pointer"
export M68K_CFLAGS="${M68K_CPU_FPU} ${M68K_COMMON}"
export M68K_CXXFLAGS="${M68K_CPU_FPU} ${M68K_COMMON}"

mkdir -p deps
mkdir -p ${SYSROOT}/usr/lib
mkdir -p ${SYSROOT}/usr/include
cd deps

# ZLIB
wget https://www.zlib.net/zlib-1.2.11.tar.gz -O zlib-1.2.11.tar.gz
tar -xvf zlib-1.2.11.tar.gz
mkdir -p zlib-1.2.11/build
cd zlib-1.2.11/build
cmake .. -DM68K_CPU="$M68K_CPU" -DM68K_FPU="$M68K_FPU" -DM68K_COMMON="${M68K_COMMON} -O3 -fno-exceptions -w -noixemul -DBIG_ENDIAN -DAMIGA -fpermissive -std=c++14"
cmake --build . --config Release --target install -- -j$(getconf _NPROCESSORS_ONLN)
cd ../..

# SDL1.2
wget https://github.com/AmigaPorts/libSDL12/archive/master.tar.gz -O SDL-1.2.tar.gz
tar -xvf SDL-1.2.tar.gz
cd libSDL12-master
make PREFX=${SYSROOT} PREF=${SYSROOT} -j$(getconf _NPROCESSORS_ONLN)
mkdir -p ${SYSROOT}/usr/lib
mkdir -p ${SYSROOT}/usr/include
cp -fvr libSDL.a ${SYSROOT}/usr/lib/
cp -fvr include/* ${SYSROOT}/usr/include/
cd ..

# SDL_mixer
wget https://github.com/SDL-mirror/SDL_mixer/archive/SDL-1.2.tar.gz -O SDL_mixer-SDL-1.2.tar.gz
tar -xvf SDL_mixer-SDL-1.2.tar.gz
cd SDL_mixer-SDL-1.2
./autogen.sh
SDL_LIBS='-lSDL -ldebug' SDL_CFLAGS="-I${SYSROOT}/usr/include/SDL -noixemul" CFLAGS="${M68K_CFLAGS}" CXXFLAGS="${M68K_CXXFLAGS}" ./configure --disable-sdltest --disable-shared --enable-static --host=${TARGET} --prefix="${SYSROOT}/usr"
make -j$(getconf _NPROCESSORS_ONLN)
make install
cd ..

# FreeType
wget https://download.savannah.gnu.org/releases/freetype/freetype-2.10.1.tar.gz -O freetype-2.10.1.tar.gz
tar -xvf freetype-2.10.1.tar.gz
mkdir -p freetype-2.10.1/build
cd freetype-2.10.1/build
cmake .. -DUNIX=1 -DM68K_CPU="$M68K_CPU" -DM68K_FPU="$M68K_FPU" -DM68K_COMMON="${M68K_COMMON}"
cmake --build . --config Release --target install -- -j$(getconf _NPROCESSORS_ONLN)
cd ../..

# SDL_ttf
wget https://github.com/SDL-mirror/SDL_ttf/archive/SDL-1.2.tar.gz -O SDL_ttf-SDL-1.2.tar.gz
tar -xvf SDL_ttf-SDL-1.2.tar.gz
cd SDL_ttf-SDL-1.2
./autogen.sh
LDFLAGS="-L${SYSROOT}/usr/lib" SDL_LIBS='-lSDL -ldebug' SDL_CFLAGS="-L${SYSROOT}/usr/lib -I${SYSROOT}/usr/include/SDL -noixemul"  CFLAGS="${M68K_CFLAGS}" CXXFLAGS="${M68K_CXXFLAGS}" FT2_CFLAGS="-L${SYSROOT}/usr/lib -I${SYSROOT}/usr/include/freetype2" FT2_LIBS="-lfreetype -lzlib" ./configure --disable-shared --enable-static --host=${TARGET} --prefix=${SYSROOT}/usr
make -j$(getconf _NPROCESSORS_ONLN)
make install
