#pragma once

class Gate
{
public:
	Gate();
	~Gate();
	void Init();
	void Start();
	void Stop();
private:
	void InitManager();
};
