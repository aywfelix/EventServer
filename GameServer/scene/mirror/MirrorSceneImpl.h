#pragma once
#include "scene/map/SceneImpl.h"
class Obj;
class MirrorSceneImpl : public SceneImpl
{
public:
	virtual void OnLoadScene(Obj* obj) {}

	virtual void OnPlayerEnterScene(Obj* obj) {}
	virtual void OnPlayerLeaveScene(Obj* obj) {}
	virtual void OnPlayerKilledScene(Obj* obj) {}
	virtual void OnPlayerReturnLastScene(Obj* obj) {}

	virtual void OnObjEnterScene(Obj* obj) {}
	virtual void OnObjLeaveScene(Obj* obj) {}
	virtual void OnObjKilledScene(Obj* obj) {}
};

