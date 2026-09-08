#!/bin/bash
set -e
set -o pipefail

# 设置构建参数
export PREFIX=$HOME/oc32/windows
export TARGET=oc32-none-elf
export SRC=$HOME/oc32/gcc/binutils-gdb
export BUILD=$HOME/oc32/gcc/binutils-gdb-build-win

# 设置 POSIX 版本的 MinGW 工具链
export CC=x86_64-w64-mingw32-gcc-posix
export CXX=x86_64-w64-mingw32-g++-posix
export AR=x86_64-w64-mingw32-gcc-ar-posix
export RANLIB=x86_64-w64-mingw32-gcc-ranlib-posix

# 关闭把警告当错误处理，避免 Werror discarded-qualifiers 导致编译失败
export ERROR_ON_WARNING=no
export CFLAGS="$CFLAGS -Wno-error=discarded-qualifiers"
export CXXFLAGS="$CXXFLAGS -Wno-error=discarded-qualifiers"

# 确保目录存在
mkdir -p $BUILD

echo "=== Building Windows binutils ==="
# 清理之前的构建
rm -rf $BUILD/*

cd $BUILD

# 配置 binutils（使用 mingw64 交叉编译工具链）
$SRC/configure \
	--host=x86_64-w64-mingw32 \
	--build=x86_64-linux-gnu \
	--target=$TARGET \
	--prefix=$PREFIX  \
	LDFLAGS="-static" \
	--disable-shared \
	--enable-static

# 构建 binutils
echo "=== 开始编译 binutils ==="
make -j$(nproc) V=2 2>&1 | tee make.log

# 安装 binutils
echo "=== 安装 binutils ==="
sudo make install 2>&1 | tee make-install.log

echo "=== Windows binntils 构建完成 ==="
