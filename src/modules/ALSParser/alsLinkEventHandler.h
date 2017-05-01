#pragma once

//#include "Poco/Stopwatch.h"
#include "Poco/Thread.h"
#include "Poco/Timestamp.h"
//#include "Poco/Timer.h"

#include "Constants.h"
#include "Tempo.h"
#include "Model.h"
#include "ofxAbletonLink.h"
#include "abletonLink.h"
#include "alsParserEvents.h"

OFX_ALS_BEGIN_NAMESPACE

//using Poco::Stopwatch;
//using Poco::Thread;
//using Poco::Timer;
//using Poco::TimerCallback;

class alsLinkEventHandler {
public:
	typedef std::chrono::duration<float> durationSecs;
	typedef std::chrono::microseconds duration;
	
	~alsLinkEventHandler();
	alsLinkEventHandler();
	
	void setEventFireDelay(int delay_ms);
	int getEventFireDelay() const;
	
	bool enableNoteEvents( );
	bool enableNoteEvents( LiveSet& LS );
	bool parseNoteEvents( LiveSet& LS );
	
	bool enableTrackEvents( );
	bool enableTrackEvents( LiveSet& LS );
	bool parseTrackEvents( LiveSet& LS );
	
	bool enableMetronomEvents( );
	bool enableMetronomEvents( LiveSet& LS );
	bool parseMetronomEvents( LiveSet& LS );
	
	void enableSyncWithLive();
	void disableSyncWithLive();
	void resetTimeline(abletonLinkSyncEventArgs &args);
	
	void timerTick( alsLinkEventHandler::duration curTime );
	
	// OF event listener / notifier
	static ofEvent<alsNoteEventArgs> alsNoteEvent;
	static ofEvent<alsTrackEventArgs> alsTrackEvent;
	static ofEvent<LSMetronomEvent> alsMetronomEvent;
	
	alsLinkEventHandler::duration getAbletonElapsedTime();
	alsLinkEventHandler::durationSecs getAbletonElapsedTimeSec();
	
private:
	
	//duration abletonTimeOffset;
	bool bNoteEvents = false;
	bool bTrackEvents = false;
	bool bMetronomEvents = false;
	bool bSyncingWithLive = false;
	durationSecs eventFireDelay;
	
	int currentNoteEventIndex = 0;
	int currentTrackEventIndex = 0;
	vector<alsLinkEventHandler::duration> nextMetronomEvent;
	
	vector<LSNoteEvent> LSNoteEvents;
	vector<LSTrackEvent> LSTrackEvents;
	vector<LSMetronomEvent> LSMetronomEvents;
	
	void fireNextNoteEvents( alsLinkEventHandler::durationSecs curTime );
	void fireNextTrackEvents( alsLinkEventHandler::durationSecs curTime );
	void fireNextMetronomEvents( alsLinkEventHandler::durationSecs curTime );
	
	template <typename T>
	static bool sort_by_time(const T& v0, const T& v1) { return v0.note.time < v1.note.time; }
	
	template <typename T>
	static bool sort_by_audio_clip_time(const T& v0, const T& v1) { return v0.audioClip.time < v1.audioClip.time; }
};

OFX_ALS_END_NAMESPACE
