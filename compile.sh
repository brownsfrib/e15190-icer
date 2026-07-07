#!/bin/bash

LIB_DIR="./lib"
INCLUDES="-I$LIB_DIR -I$LIB_DIR/include -I$LIB_DIR/HTRunInfo -I$LIB_DIR/Nuclear_Masses -IPlugins"
#LINK_FLAGS="-L$LIB_DIR -lE15190Calibrated -Wl,-rpath \$ORIGIN/../lib"
LINK_FLAGS="-L$LIB_DIR -Wl,--no-as-needed -lE15190Reader -Wl,--as-needed -Wl,-rpath,\$ORIGIN/../lib"
PROJECT_ROOT_DEF="-DPROJECT_ROOT=\"$(pwd)\""

if [ "$#" -lt 1 ]; then
        echo -e "\033[1;31mNo input command passed.\033[0m\033[1;33m Call <.cxx> <fileName>\033[0m\n"
else
        exeName=$(basename $1 .cxx)
        g++ -o ./bin/$exeName $1  \
		-std=c++11 \
		-g \
		-O3 \
		-Wall \
		$INCLUDES \
		$LINK_FLAGS \
		$PROJECT_ROOT_DEF \
		$(root-config --cflags --libs) \
                $(gsl-config --cflags)

        if [ $? -ne 0 ]; then
                echo -e "\033[1;31mCompilation failed.\033[0m\n"
        else
                echo -e "Compiled \033[1;32m$1\033[0m into $(pwd)/bin/$1\n"
        fi
fi
