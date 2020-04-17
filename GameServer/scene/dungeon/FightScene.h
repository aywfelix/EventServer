#pragma once
#include "scene/map/Scene.h"

class FightSceneImpl;
//Scene* fightScene = new FightScene(FightSceneImpl* impl);
class FightScene : public Scene
{
public:
	FightScene(FightSceneImpl* impl);
	~FightScene();
	virtual bool ObjEnterScene(Obj* obj) { return true; }
	virtual bool ObjLeaveScene(Obj* obj) { return true; }
	virtual bool LoadScene(Obj* obj) { return true; }
private:
	FightSceneImpl* m_scene_impl;
};



