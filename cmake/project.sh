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

# make servers
Servers=("Common","SeNet","MasterServer","GateServer","ChatServer","LoginServer","WorldServer","GameServer")
make_all_servers(){
    for str in ${Servers[@]};do
        cd $DIR/../$str/
        if [ ! -d build ];then
            mkdir build
        fi
        cd build
        cmake ..
        make -j8
        # cp lib to libs and cp server to bin dir
        $LIB = `ls | grep -E '*\.a'`
        if [ ! -n "$LIB" ]; then
            cp -f $LIB $DIR/../libs/     
        else
            cp -f $str $DIR/../libs/
        fi
    done 
}
