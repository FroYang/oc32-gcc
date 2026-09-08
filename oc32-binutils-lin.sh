#!/bin/bash
set -e
set -o pipefail

# 设置构建参数
export PREFIX=$HOME/oc32/linux
export TARGET=oc32-none-elf 
export SRC=$HOME/oc32/gcc/binutils-gdb
export BUILD=$HOME/oc32/gcc/binutils-gdb-build-lin

# 关闭把警告当错误处理，避免 Werror discarded-qualifiers 导致编译失败
export ERROR_ON_WARNING=no
export CFLAGS="$CFLAGS -Wno-error=discarded-qualifiers"
export CXXFLAGS="$CXXFLAGS -Wno-error=discarded-qualifiers"

# 确保目录存在
mkdir -p $BUILD

echo "=== Building Linux binutils ==="
# 清理之前的构建
rm -rf $BUILD/*

cd $BUILD

# 配置 binutils
$SRC/configure \
	--host=x86_64-linux-gnu \
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

echo "=== Linux binntils 构建完成 ==="


