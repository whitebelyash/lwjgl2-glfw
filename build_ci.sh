#!/bin/bash
set -e

install_ndk() {
	# install ndk
	echo "Installing NDK"
	curl https://dl.google.com/android/repository/android-ndk-r29-linux.zip --output android-ndk-r29-linux.zip
	unzip android-ndk-r29-linux.zip
	export ANDROID_NDK_HOME="$(pwd)/android-ndk-r29"
}

if [ "$CI" = "true" ]; then
	# GitHub ships an Ubuntu image with ANDROID_NDK_HOME set to NDK r27,
	# which does not build 16k page libraries by default
	install_ndk
fi

if [ ! -d "${ANDROID_NDK_HOME}" ]; then
	echo "NDK not found. Set ANDROID_NDK_HOME if you want to use a preinstalled NDK"
	install_ndk
fi

echo "Will use NDK at ${ANDROID_NDK_HOME}"

export PLATFORM_BUILD=$(realpath platform_build)

build_android_arch() {
   mkdir -p cibuild-$1
   pushd cibuild-$1
   cmake $PLATFORM_BUILD -DCMAKE_TOOLCHAIN_FILE=$ANDROID_NDK_HOME/build/cmake/android.toolchain.cmake -DANDROID_PLATFORM=android-21 -DANDROID_ABI=$1
   make -j $(($(nproc)*2))
   set +e
   $ANDROID_NDK_HOME/toolchains/llvm/prebuilt/linux-x86_64/bin/llvm-strip liblwjgl.so
   $ANDROID_NDK_HOME/toolchains/llvm/prebuilt/linux-x86_64/bin/llvm-strip liblwjgl64.so
   set -e
   zip ../libs/lwjgl-native-$1.jar liblwjgl.so liblwjgl64.so
   popd
}

#ant jars compile_native
build_android_arch arm64-v8a
build_android_arch armeabi-v7a
build_android_arch x86
build_android_arch x86_64
