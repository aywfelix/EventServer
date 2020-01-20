linux 编译环境处理
首先 更新protobuff 编译 安装
编译安装 libconfig 
编译安装 mariadb

执行python3 project_cmake.py
编译Common 拷贝libCommon.a 到libs 目录
编译SeNet      拷贝libSeNet.a 到libs 目录

下载子模块
git submodule update --init --recursive