/*************************************************************************************
* File: Init.h; Date of creation: 09.Feb.2015; Date of Last Modification: 09.Feb.2015
*
* This is a file that keeps track of time.
*
* Copyright (c) Dominik Horòák. All rights reserved.
**************************************************************************************/
#include "GameTimer.h"
#include <Windows.h>

GameTimer::GameTimer( void )
	:bSecondsPerCount( 0.0 ), bDeltaTime( -1.0 ), bBaseTime( 0 ), bPausedTime( 0 ), bPrevTime( 0 ), bCurrTime( 0 ), bStopped( false )
{
	__int64 countsPerSec;
	QueryPerformanceFrequency( ( LARGE_INTEGER* )&countsPerSec );
	bSecondsPerCount = 1.0 / countsPerSec;
}

float GameTimer::TotalTime( void ) const
{
	if( bStopped )
	{
		return ( float )( ( ( bStopTime - bPausedTime ) - bBaseTime ) * bSecondsPerCount );
	}
	else
	{
		return ( float )( ( ( bCurrTime - bPausedTime ) - bBaseTime ) * bSecondsPerCount );
	}
}

float GameTimer::DeltaTime( void ) const
{
	return ( float )bDeltaTime;
}

void GameTimer::Reset( void )
{
	__int64 currTime;
	QueryPerformanceCounter( ( LARGE_INTEGER* )&currTime );

	bBaseTime = currTime;
	bPrevTime = currTime;
	bStopTime = 0;
	bStopped = false;
}

void GameTimer::Start( void )
{
	__int64 startTime;
	QueryPerformanceCounter( ( LARGE_INTEGER* )&startTime );

	if( bStopped )
	{
		bPausedTime += ( startTime - bStopTime );

		bPrevTime = startTime;
		bStopTime = 0;
		bStopped = false;
	}
}

void GameTimer::Stop( void )
{
	if( !bStopped )
	{
		__int64 currTime;
		QueryPerformanceCounter( ( LARGE_INTEGER* )&currTime );

		bStopTime = currTime;
		bStopped = true;
	}
}

void GameTimer::Tick( void )
{
	if( bStopped )
	{
		bDeltaTime = 0.0;
		return;
	}

	__int64 currTime;
	QueryPerformanceCounter( ( LARGE_INTEGER* )&currTime );
	bCurrTime = currTime;

	bDeltaTime = ( bCurrTime - bPrevTime ) * bSecondsPerCount;
	bPrevTime = bCurrTime;

	if( bDeltaTime < 0.0 )
	{
		bDeltaTime = 0.0;
	}
}