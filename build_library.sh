#!/bin/bash
set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

LIB_DIR="lib"
SRC_DIR="$LIB_DIR/src"
INC_DIR="$LIB_DIR/include"
DICT_DIR="$LIB_DIR/dict"
OBJ_DIR="$LIB_DIR/obj"

mkdir -p "$OBJ_DIR" "$DICT_DIR"

CXX=${CXX:-g++}
ROOT_CFLAGS="$(root-config --cflags)"
ROOT_LIBS="$(root-config --libs) -lTreePlayer"

INCLUDES="-I$LIB_DIR -I$INC_DIR"
PROJECT_ROOT_DEF="-DPROJECT_ROOT=\"$SCRIPT_DIR\""
CXXFLAGS="-std=c++11 -fPIC -O3 -Wall $INCLUDES $ROOT_CFLAGS $PROJECT_ROOT_DEF"

echo -e "\033[1;34m=== Generating ROOT dictionaries ===\033[0m"

generate_dict() {
    local dict_name=$1
    local class_header=$2
    local linkdef=$3
    echo "  rootcling: $dict_name"
    rootcling -f "$DICT_DIR/${dict_name}.cpp" \
        $INCLUDES \
        "$class_header" "$linkdef"
}

generate_dict MBCalibratedDict    MBCalibratedRootEvent.h    MBCalibratedLinkDef.h
generate_dict HiRACalibratedDict  HiRACalibratedRootEvent.h  HiRACalibratedLinkDef.h

echo -e "\033[1;34m=== Compiling object files ===\033[0m"

SOURCES=(
    "$SRC_DIR/shared.cpp"
    "$SRC_DIR/MBCalibratedRootEvent.cpp"
    "$SRC_DIR/HiRACalibratedRootEvent.cpp"
    "$DICT_DIR/MBCalibratedDict.cpp"
    "$DICT_DIR/HiRACalibratedDict.cpp"
)

OBJECTS=()
for src in "${SOURCES[@]}"; do
    base=$(basename "$src")
    obj="$OBJ_DIR/${base%.*}.o"
    OBJECTS+=( "$obj" )
    echo "  compile: $src"
    $CXX $CXXFLAGS -c "$src" -o "$obj"
done

echo -e "\033[1;34m=== Linking shared library ===\033[0m"

OUTPUT="$LIB_DIR/libE15190Calibrated.so"
$CXX -shared -fPIC -o "$OUTPUT" "${OBJECTS[@]}" $ROOT_LIBS

cp "$DICT_DIR"/*_rdict.pcm "$LIB_DIR/" 2>/dev/null || true

echo -e "\033[1;32mBuilt $OUTPUT\033[0m"
