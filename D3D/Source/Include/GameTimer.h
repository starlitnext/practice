#ifndef _GAME_TIMER_H_
#define _GAME_TIMER_H_

#include "Common.h"

NS_D3DAPP_BEGIN

class D3D_DLL GameTimer
{
public:
	GameTimer();

	float TotalTime() const; // in seconds
	float DeltaTime() const; // in seconds

	void Reset();		// Call before message loop.
	void Start();		// Call when unpaused.
	void Stop();		// Call when paused.
	void Tick();		// Call every frame.

private:
	double mSecondsPerCount;
	double mDeltaTime;

	__int64 mBaseTime;
	__int64 mPausedTime;
	__int64 mStopTime;
	__int64 mPrevTime;
	__int64 mCurrTime;

	bool mStopped;
};

NS_D3DAPP_END

#endif // _GAME_TIMER_H_
