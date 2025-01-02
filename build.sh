#!/bin/bash

set -e # Means if any command fails it will exit

api="metal"
build="debug"
type="build" # clean, configure, build
test=false
editor=true
verbose=false
sanitized=false
pedantic=true
volk=true

cmake_conf_cmd="cmake -H. -G Ninja -B"
cmake_buil_cmd="cmake --build "
build_folder="build-"
configure_build_type="-DCMAKE_BUILD_TYPE="
build_volk="-DROAR_USE_VOLK="
build_api="-DROAR_RENDER_TYPE="
build_tests="-DROAR_BUILD_TESTS="
build_editor="-DROAR_BUILD_EDITOR="
build_sanitized="-DROAR_BUILD_SANITIZED="
build_pedantic="-DROAR_BUILD_PEDANTIC="
build_build_type="--config "
build_target="--target "
target="RoarEditor"

print_help() {
	echo "Build accepts the following arguments
-a [vulkan/metal]                         Metal renderer or Vulkan renderer
-b [debug/release]                        Debug or Release build
-c [build/clean/configure]                Builds only or build + configure only or clean + build + configure
-t [true/false]                           Builds tests
-e [true/false]                           Builds editor
-v [true/false]                           Verbose make output
-s [true/false]                           Build with clang sanatisers enabled [clang only]
-p [true/false]                           Build with extra warning and errors enabled
-k [true/false]                           Build with volk vulkan library
"
}

while getopts a:b:c:t:e:r:v:s:p:h flag
do
    case "${flag}" in
        a) api=${OPTARG};;
        b) build=${OPTARG};;
        c) type=${OPTARG};;
        t) test=${OPTARG};;
        e) editor=${OPTARG};;
        v) verbose=${OPTARG};;
        s) sanitized=${OPTARG};;
        p) pedantic=${OPTARG};;
        k) volk=${OPTARG};;
        h) print_help ; exit 0;;
    esac
done

echo "api:        $api";
echo "build:      $build";
echo "type:       $type";
echo "test:       $test";
echo "editor:     $editor";
echo "verbose:    $verbose";
echo "sanitized:  $sanitized";
echo "pedantic:   $pedantic";
echo "volk:       $volk";

NUMCPUS=8 # Default number looks good

if [[ "$OSTYPE" == "linux-gnu" ]]; then
	NUMCPUS=$(awk '/^processor/ {++n} END {print n+1}' /proc/cpuinfo)
fi

if [[ "$OSTYPE" == "darwin"* ]]; then
	NUMCPUS=$(sysctl -n hw.ncpu)
fi

if [ "$api" = "metal" ] ; then
	build_folder+="metal-"
	build_api+="Metal"
else
	build_folder+="vulkan-"
	build_api+="Vulkan"
fi

if [ "$build" = "debug" ] ; then
	build_folder+="debug"
	configure_build_type+="Debug"
	build_build_type+="Debug"
else
	build_folder+="release"
	configure_build_type+="Release"
	build_build_type+="Release"
fi

if [ "$test" = true ] ; then
	build_tests+="1"
	target="roar_test"
else
	build_tests+="0"
fi

if [ "$editor" = true ] ; then
	build_editor+="1"

	if [ "$test" = true ] ; then
		target="all"
	fi
else
	build_editor+="0"
fi

if [ "$sanitized" = true ] ; then
	build_sanitized+="1"
else
	build_sanitized+="0"
fi

if [ "$pedantic" = true ] ; then
	build_pedantic+="1"
else
	build_pedantic+="0"
fi

if [ "$volk" = true ] ; then
	build_volk+="1"
else
	build_volk+="0"
fi

cmake_conf_cmd+="$build_folder"
cmake_conf_cmd+=" $configure_build_type"
cmake_conf_cmd+=" $build_api"
cmake_conf_cmd+=" $build_tests"
cmake_conf_cmd+=" $build_editor"
cmake_conf_cmd+=" $build_sanitized"
cmake_conf_cmd+=" $build_pedantic"
cmake_conf_cmd+=" $build_volk"

if [ "$verbose" = true ] ; then
	export VERBOSE=1
fi

cmake_buil_cmd+="$build_folder"
cmake_buil_cmd+=" $build_build_type"
cmake_buil_cmd+=" $build_target"
cmake_buil_cmd+="$target"

cmake_buil_cmd+=" -j${NUMCPUS}"

if [ "$type" = "clean" ]; then
	if [ -d $build_folder ]; then
		echo "Removing $build_folder"
		rm -rf $build_folder
	fi
fi

echo "type is $type"

if [ ! "$type" = "build" ] || [ ! -d $build_folder ]; then
	echo "Configuring with command: $cmake_conf_cmd"
	$cmake_conf_cmd
fi

rm compile_commands.json
ln -s $build_folder/compile_commands.json compile_commands.json

if [ -h "build" ]; then
	rm build
fi

ln -s $build_folder build

echo "Building with command: $cmake_buil_cmd"
$cmake_buil_cmd
