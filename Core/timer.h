#pragma once
class Timer
{
public:
	Timer();
	~Timer();
	double GetTime();
	double GetFrameTime();
private:
	double countsPerSecond = 0.0;
	__int64 counterStart = 0;

	int frameCount = 0;
	int fps = 0;

	__int64 frameTimeOld = 0;

};

