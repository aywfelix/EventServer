﻿游戏服务器介绍：
1、一个mmo服务器框架，服务器端实现多个逻辑服（login、gate、world、chat、game）自动互联互通，db server 处理与mysql交互，提供redis 服务等。
2、提供win、Linux两个平台部署，Linux自动编译（cmake文件夹下的脚本自动将proj文件转成linux下cmakelist）
3、提供读表生成c++代码（见DataTable文件）
其他：
1、编译开发环境
windows10开发环境，vs2019
linux环境为centos8，gcc 9.2
2、第三方库
libconfig、protobuffer、mariadb、mariacpp、redis、sol2
3、Common、SeNet编译为静态库，其他逻辑Server加载即可使用
