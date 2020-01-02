#pragma once

class Master
{
public:
	Master();
	~Master();
	void Init();
	void Start();
	void Stop();
private:
	void InitManager();
};

