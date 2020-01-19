#pragma once

class World
{
public:
	World();
	~World();
	void Init();
	void Start();
	void Stop();
private:
	void InitManager();
};

