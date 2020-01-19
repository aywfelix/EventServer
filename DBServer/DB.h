#pragma once

class DB
{
public:
	DB();
	~DB();
	void Init();
	void Start();
	void Stop();
private:
	void InitManager();
};

