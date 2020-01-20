# 用于linux 环境的编译脚本

#!/bin/bash
# set path
DIR=$(cd $(dirname $0); pwd)
cd $DIR/../
# create bin/log dir
if [ ! -d bin  ];then
  mkdir bin
  chmod 777 bin
  mkdir ServerLogs
  chmod 777 ServerLogs
fi
# install third part
cd $DIR/../ThirdPart
git submodule update --init --recursive
# install hiredis
cd $DIR/../ThirdPart/hiredis
cmake .
make -j8 && make install
# install mariacpp
cd $DIR/../ThirdPart/MariaCpp
cmake .
make -j8 && make install
#install protobuf
cd $DIR/../ThirdPart/protobuf/cmake
cmake -Dprotobuf_BUILD_TESTS=OFF .
make -j8 && make install
# gen cmakelist files
cd $DIR
python3 project_cmake.py
# clear bin files
rm -rf $DIR/../bin/*
# make servers
Servers=("Common" "SeNet" "MasterServer" "GateServer" "ChatServer" "LoginServer" "WorldServer" "GameServer")
make_all_servers(){
    for str in ${Servers[@]};do
        cd $DIR/../$str/ && pwd
        if [ ! -d build ];then
            mkdir build
        fi
        cd build
        cmake ..
        make $1
        # cp lib to libs and cp server to bin dir
        LIB=`ls | grep -E '*\.a'`
        if [ "$LIB" = "" ]; then
            cp -f $str $DIR/../bin/
        else
            cp -f $LIB $DIR/../libs/
        fi
    done 
}

make_all_servers "-j8"