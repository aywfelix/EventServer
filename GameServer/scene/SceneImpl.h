#pragma once

class Obj;
class SceneImpl
{
public:
	SceneImpl();
	virtual ~SceneImpl();
	virtual void OnLoadScene(Obj* obj);

	virtual void OnPlayerEnterScene(Obj* obj);
	virtual void OnPlayerLeaveScene(Obj* obj);
	virtual void OnPlayerKilledScene(Obj* obj);
	virtual void OnPlayerReturnLastScene(Obj* obj);

	virtual void OnObjEnterScene(Obj* obj);
	virtual void OnObjLeaveScene(Obj* obj);
	virtual void OnObjKilledScene(Obj* obj);

private:

};

