#pragma once

// 对象类型列举
enum EObjType
{
	E_OBJ_INVALID = 0,
	E_OBJ_PLAYER = 1,  // 代表玩家
	E_OBJ_PET = 2,
	E_OBJ_MONSTER = 3,
	E_OBJ_MAX
};

// 各个模块名称枚举
enum EModuleType
{
	E_MODULE_VOID = 0,
	E_MODULE_PET = 1,
	E_MODULE_RANK = 2,
	E_MODULE_BUFF = 3,
};

// 游戏场景分类
enum ESceneType
{
	E_SCENE_VOID = 0,
	E_SCENE_NORM = 1,		// 普通场景
	E_SCENE_MIRROR = 2,		// 游戏镜像
	E_SCENE_DUNGEON = 3,	// 游戏副本
};

