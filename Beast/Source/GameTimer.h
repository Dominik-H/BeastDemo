/*************************************************************************************
* File: Init.h; Date of creation: 09.Feb.2015; Date of Last Modification: 09.Feb.2015
*
* This is a header file for GameTimer class that handles keeping track of time.
*
* Copyright (c) Dominik Horòák. All rights reserved.
**************************************************************************************/
#ifndef GAMETIMER_H
#define GAMETIMER_H

class GameTimer
{
public:
	GameTimer( void );
	
	float TotalTime( void ) const;
	float DeltaTime( void ) const;

	void Reset( void );
	void Start( void );
	void Stop( void );
	void Tick( void );

private:
	double bSecondsPerCount;
	double bDeltaTime;

	__int64 bBaseTime;
	__int64 bPausedTime;
	__int64 bStopTime;
	__int64 bPrevTime;
	__int64 bCurrTime;

	bool bStopped;
};

#endif // GAMETIMER_H