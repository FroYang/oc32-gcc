#!/bin/bash
set -e
set -o pipefail

# 设置构建参数
export OC32=$HOME/oc32
export PREFIX=$OC32/linux
export TARGET=oc32-none-elf    # 与 binutils 保持一致
export SRC=$HOME/oc32/gcc/gcc-15.3.0
export BUILD=$HOME/oc32/gcc/gcc-15.3.0-build-lin
export PATH=$PATH:$OC32/linux/bin

if [ ! -f "$OC32/linux/bin/${TARGET}-as" ] || [ ! -f "$OC32/linux/bin/${TARGET}-ld" ]; then
	echo "错误：未找到binutils工具，请先构建binutils"
	exit 1
fi

echo "=== 检查binutils构建产物 ==="
which ${TARGET}-as
which ${TARGET}-ld
${TARGET}-as --version

# 关闭把警告当错误处理，避免 Werror discarded-qualifiers 导致编译失败
export ERROR_ON_WARNING=no
export CFLAGS="$CFLAGS -Wno-error=discarded-qualifiers"
export CXXFLAGS="$CXXFLAGS -Wno-error=discarded-qualifiers"


echo "=== Building Stage Linux OC32-GCC ==="
cd $BUILD



# 构建 GCC
echo "=== 开始编译 GCC ==="
make -j$(nproc) V=2 all-gcc 2>&1 | tee make-all-gcc.log

# 安装 GCC
echo "=== 安装 GCC ==="
sudo make install-gcc 2>&1 | tee make-install-gcc.log

# build GCC libgcc
echo "=== 编译 libgcc ==="
make -j1 V=2 all-target-libgcc 2>&1 | tee make-libgcc.log

# 安装 libgcc
echo "=== 安装 libgcc ==="
sudo bash -c "export PATH=\"$BUILD/gcc:$PREFIX/bin:$PATH\" && make install-target-libgcc" 2>&1 | tee make-install-libgcc.log
