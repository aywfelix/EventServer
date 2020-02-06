保存项目需要的脚本工具等
上传安装 工具
yum -y install lrzsz
本项目是centos8 环境
解压 lz4 文件
yum install -y lz4 lz4-devel

coredump 默认文件路径
/var/lib/systemd/coredump  

设置 文件描述符上限
vim /etc/security/limits.conf
在最后加入
* soft nofile 8192
* hard nofile 8192
ulimit -a 查看信息

TODO 修改dump路径到bin目录下
TODO 添加mysql 请求处理， 将sql 语句与protobuff 互转
TODO protobuff 编辑客户端程序
TODO 模拟玩家登录游戏
TODO 策划数值公式转换工具（表达式公式）
TODO AI 
TODO 地图导航 recast navgition


linux下设置coredump文件的开关和路径
ulimit -c unlimited 不限制生成core文件大小
ulimit -c 0 关闭core文件生成开关
ulimit -c 1024 限定生成core文件的大小为1024

2.core文件的命名规则
/proc/sys/kernel/core_uses_pid 1 表示使用procid命名，0表示不使用
/proc/sys/kernel/core_pattern 可以设置格式化的 core 文件保存位置或文件名
echo “/opt/corefile/core-%e-%p-%t” > /proc/sys/kernel/core_pattern
将会控制所产生的 core 文件会存放到 /corefile 目录下，产生的文件名为 core- 命令名 -pid- 时间戳
以下是参数列表 :
%p - insert pid into filename 添加 pid
%u - insert current uid into filename 添加当前 uid
%g - insert current gid into filename 添加当前 gid
%s - insert signal that caused the coredump into the filename 添加导致产生 core 的信号
%t - insert UNIX time that the coredump occurred into filename 添加 core 文件生成时的 unix 时间
%h - insert hostname where the coredump happened into filename 添加主机名
%e - insert coredumping executable name into filename 添加命令名









