#pragma once

class Game
{
public:
	Game();
	~Game();
	void Init();
	void Start();
	void Stop();
private:
	void InitManager();
};

