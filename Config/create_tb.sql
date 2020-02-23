# 创建账号表
CREATE TABLE IF NOT EXISTS `tb_account` (
    `id` INT(11) AUTO_INCREMENT NOT NULL COMMENT "表唯一id 主键",
	`loginname` VARCHAR(50) NOT NULL COMMENT "玩家注册登录名",
	`loginpwd` VARCHAR(50) NOT NULL COMMENT "玩家注册登录密码",
	`gameid` CHAR(30) NOT NULL COMMENT "游戏账号",
	`openid` CHAR(30) NOT NULL,
	`token` CHAR(20) NOT NULL,
	`channelid` CHAR(20) NOT NULL,
	`whitelist` TINYINT(4) DEFAULT 0,
	`snid` CHAR(30) NOT NULL,
	`mac` CHAR(30) DEFAULT NULL,
	PRIMARY KEY (`id`)
)ENGINE=InnoDB DEFAULT CHARSET=utf8 AUTO_INCREMENT=1;

# 创建玩家角色表 （一个玩家可以多个角色）
CREATE TABLE IF NOT EXISTS `tb_player` (
	`playerid` INT(64) NOT NULL COMMENT '玩家角色唯一id',
	`gameid` CHAR(30) NOT NULL COMMENT '游戏账号id',
	`role` TINYINT(4) NOT NULL DEFAULT 1 COMMENT '角色 （从配置表中获取一般4男4女8个角色）',
	`nickname` CHAR(50) NOT NULL COMMENT '昵称',
	`sex` TINYINT(4) NOT NULL DEFAULT 0 COMMENT '性别',
	`level` INT(11) NOT NULL DEFAULT 0 COMMENT '游戏等级',
	`create_tm` TIMESTAMP NOT NULL DEFAULT '0000-00-00 00:00:00' COMMENT '角色创建时间',
	`online_tm` TIMESTAMP NOT NULL DEFAULT '0000-00-00 00:00:00' COMMENT '上线时间',
	`offline_tm` TIMESTAMP NOT NULL DEFAULT '0000-00-00 00:00:00' COMMENT '下线时间',
	`gold` INT(11) NOT NULL DEFAULT 0 COMMENT '玩家金币',
	`guildid` CHAR(20) NOT NULL COMMENT '工会id',
    PRIMARY KEY (`playerid`)
)ENGINE=InnoDB DEFAULT CHARSET=utf8;

# 创建工会表

#插入测试数据
INSERT INTO tb_account(loginname,loginpwd,gameid,openid,token,channelid,snid) values('haha','123456','xx11xx','xx22xx','xxxxx','1234','1321-wewae');
INSERT INTO tb_player(playerid,gameid,nickname,guildid) values(1,'xx11xx','haha','guidid123');
