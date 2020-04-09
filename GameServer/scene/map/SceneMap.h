#pragma once
#include <thread>
#include <vector>
#include <string>
#include <memory>
#include <functional>
#include "MapNav.h"

class Scene;
using scene_ptr_t = std::shared_ptr<Scene>;
// 使用多线程加载地图网格信息
class SceneMap
{
public:
	bool Init(std::string mapfile);
	void Start();
	void LoadMap();
private:
	std::string m_mapfile;
	std::thread m_thread;
	MapNav m_map_nav;
};


