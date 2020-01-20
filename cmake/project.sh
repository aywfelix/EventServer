# 用于linux 环境的编译脚本

#!/bin/bash
# set path
PWD=$(cd $(dirname $0); pwd)
cd $PWD/../
# create bin/log dir
if [ ! -d bin  ];then
  mkdir bin
  chmod 777 bin
  mkdir ServerLogs
  chmod 777 ServerLogs
fi
# install third part
cd $PWD/../ThirdPart
git submodule update --init --recursive
# install hiredis
cd hiredis
mkdir build && cd build
cmake ..
make && make install
# install mariacpp
cd ../MariaCpp
mkdir build && cd build
cmake ..
make && make install
#install protobuf
cd ../protobuf/cmake
mkdir build && cd build
cmake ..
make && make install
# gen cmakelist files
python3 project_cmake.py

# make servers
cd $PWD
Servers=("Common","SeNet","MasterServer","GateServer","ChatServer","LoginServer","WorldServer","GameServer")
make_all_servers(){
    for str in ${Servers[@]};do
        cd $PWD/../$str/
        if [ ! -d build ];then
            mkdir build
        fi
        cd build
        cmake ..
        make -j8
        # cp lib to libs and cp server to bin dir
        $LIB = `ls | grep -E '*\.a'`
        if [ ! -n "$LIB" ]; then
            cp -f $LIB $PWD/../libs/     
        else
            cp -f $str $PWD/../libs/
        fi
    done 
}
