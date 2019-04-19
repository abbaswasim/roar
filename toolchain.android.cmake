set(CMAKE_SYSTEM_NAME Android)
set(CMAKE_SYSTEM_VERSION 28) # API level
set(CMAKE_ANDROID_ARCH_ABI arm64-v8a)
set(CMAKE_ANDROID_NDK /development/android/ndk-r18b)
set(CMAKE_ANDROID_STL_TYPE c++_static)

set(ROAR_REQUIRED_LIBRARIES "log") # android specific libraries for Roar
