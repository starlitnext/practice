
#include "GameTimer.h"
#include <windows.h>

NS_D3DAPP_BEGIN

GameTimer::GameTimer()
	: mSecondsPerCount(0.0), mDeltaTime(-1.0), mBaseTime(0),
	mPausedTime(0), mPrevTime(0), mCurrTime(0), mStopped(0)
{
	__int64 countsPerSec;
	::QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);
	mSecondsPerCount = 1.0 / (double)countsPerSec;
}

float GameTimer::TotalTime() const
{
	// If we are stopped, do not count the time that has passed
	// since we stopped. Moreover, if we previously already had
	// a pause, the distance mStopTime - mBaseTime includes paused
	// time, which we do not want to count. To correct this, we can
	// subtract the paused time from mStopTime.

	if (mStopped)
	{
		return static_cast<float>(((mStopTime - mBaseTime)-mPausedTime)*mSecondsPerCount);
	}
	else
	{
		return static_cast<float>(((mCurrTime - mBaseTime) - mBaseTime)*mSecondsPerCount);
	}
}

float GameTimer::DeltaTime() const
{
	return static_cast<float>(mDeltaTime);
}

void GameTimer::Reset()
{
	__int64 currTime;
	::QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

	mBaseTime = currTime;
	mPrevTime = currTime;
	mStopTime = 0;
	mStopped = false;
}

void GameTimer::Start()
{
	__int64 startTime;
	::QueryPerformanceCounter((LARGE_INTEGER*)&startTime);

	//Accumulate the time elapsed between stop and start pairs.
	
	// If we are resuming the timer from a stopped state...
	if (mStopped)
	{
		// then accumulate the paused time.
		mPausedTime += (startTime - mStopTime);

		// since we are starting the timer back up, the current
		// previous time is not valid, as it occurred while paused.
		// So reset it to the current time.
		mPrevTime = startTime;

		// no longer stopped...
		mStopTime = 0;
		mStopped = false;
	}

}

void GameTimer::Stop()
{
	// If we are already stopped, then don't do anything
	if (!mStopped)
	{
		__int64 currTime;
		::QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

		// Otherwise, save the time we stopped at, and set
		// The Boolean flag indicating the timer is stopped.
		mStopTime = currTime;
		mStopTime = true;
	}
}

void GameTimer::Tick()
{
	if (mStopped)
	{
		mDeltaTime = 0.0;
		return;
	}

	// Get the time this frame.
	__int64 currTime;
	::QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
	mCurrTime = currTime;

	// Time difference between this frame and the previous.
	mDeltaTime = (mCurrTime - mPrevTime)*mSecondsPerCount;

	// Prepare for next frame.
	mPrevTime = mCurrTime;

	// Force nonnegative. The DXSDK's CDXUTTimer mentions that if the
	// processor goes into a power save mode or we get shuffled to another
	// processor, then mDeltaTime can be negative.
	if (mDeltaTime < 0.0)
		mDeltaTime = 0.0;
}

NS_D3DAPP_END