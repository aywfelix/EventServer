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

TODO log 日志文件名太长了
TODO 修改dump路径到bin目录下








