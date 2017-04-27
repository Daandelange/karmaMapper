#include "alsLinkEventHandler.h"

// prototype seems to be needed for static ofEvents
ofEvent<ofx::AbletonLiveSet::LSNoteEvent> ofx::AbletonLiveSet::alsLinkEventHandler::alsNoteEvent;
ofEvent<ofx::AbletonLiveSet::LSMetronomEvent> ofx::AbletonLiveSet::alsLinkEventHandler::alsMetronomEvent;
ofEvent<alsTrackEventArgs> ofx::AbletonLiveSet::alsLinkEventHandler::alsTrackEvent;

OFX_ALS_BEGIN_NAMESPACE

alsLinkEventHandler::~alsLinkEventHandler(){
	//delete timer;
}

alsLinkEventHandler::alsLinkEventHandler(){
	//timer = new Timer(0, 10);
	LSNoteEvents.clear();
	LSTrackEvents.clear();
	
	abletonTimeOffset = abletonLink::getInstance().getClock().micros();
}


//template <class ListenerClass>
bool alsLinkEventHandler::enableNoteEvents(  ){
	
	if( LSNoteEvents.size() < 1 ) return false;
	
	//startThreadedTimer();
	//abletonLink::getInstance() .micros();
	
	currentNoteEventIndex = 0;
	bSyncingWithLive = false;
	
	bNoteEvents = true;
	
	return true;
}

bool alsLinkEventHandler::enableNoteEvents(ofx::AbletonLiveSet::LiveSet &LS){
	parseNoteEvents(LS);
	enableNoteEvents();
}



void alsLinkEventHandler::fireNextNoteEvents(alsLinkEventHandler::durationSecs curTime){
	if(LSNoteEvents.size() < currentNoteEventIndex) return;
	
	// stopWatch.elapsed() is in milliseconds
	// note.time is in seconds
	
	for(int i=currentNoteEventIndex; i<LSNoteEvents.size(); i++){
		
		if( curTime.count() >= LSNoteEvents[i].note.time){
			// fire the event!
			ofNotifyEvent( alsNoteEvent, LSNoteEvents[i] );
			
			// remember
			currentNoteEventIndex=i+1;
		}
		// interrupt for loop ?
		if(LSNoteEvents[i].note.time > curTime.count()) break;
	}
}
//
bool alsLinkEventHandler::parseNoteEvents( LiveSet& LS ){
	
	// ALS loaded ?
	if( !LS.isLoaded() ){
		ofLogNotice("OfxAbletonLiveSet::alsLinkEventHandler::parseNoteEvents()", "before parsing events, make sure to parse() a live set.");
		return false;
	}
	
	// clean up
	LSNoteEvents.clear();
	
	// loop trough LS data
	for (int trackNb = 0; trackNb < LS.miditracks.size(); trackNb++){
		int nthNote=0;
		
		for(int clipNb=0; clipNb < LS.miditracks[trackNb].clips.size(); clipNb++){
			
			string clipName = LS.miditracks[trackNb].clips[clipNb].name;
			int clipColor = LS.miditracks[trackNb].clips[clipNb].color;
			int nthInClip = 0;
			
			for( vector<Note>::iterator it=LS.miditracks[trackNb].clips[clipNb].notes.begin(); it != LS.miditracks[trackNb].clips[clipNb].notes.end(); it++ ){
				
				LSNoteEvent noteEvent( clipName, clipColor, nthNote, nthInClip, trackNb, *it );
				
				LSNoteEvents.push_back(noteEvent);
				
				nthInClip ++;
			}
		}
	}
	std:sort(LSNoteEvents.begin(), LSNoteEvents.end(), sort_by_time<LSNoteEvent>);
	
	return true;
}

bool alsLinkEventHandler::enableTrackEvents(  ){
	if( LSTrackEvents.size() < 1 ) return false;
	
	currentTrackEventIndex = 0;
	
	bTrackEvents = true;
	
	return true;
}

bool alsLinkEventHandler::enableTrackEvents(ofx::AbletonLiveSet::LiveSet &LS){
	parseTrackEvents(LS);
	enableTrackEvents();
}



void alsLinkEventHandler::fireNextTrackEvents(alsLinkEventHandler::durationSecs curTime){
	if(LSTrackEvents.size() < currentTrackEventIndex) return;
	
	for(int i=currentTrackEventIndex; i<LSTrackEvents.size(); i++){
		if(curTime.count() >= LSTrackEvents[i].audioClip.time){
			// fire the event!
			ofNotifyEvent( alsTrackEvent, LSTrackEvents[i] );
			
			// remember
			currentTrackEventIndex=i+1;
		}
		// interrupt for loop ?
		if(LSTrackEvents[i].audioClip.time > curTime.count()){
			break;
		}
	}
}

//
bool alsLinkEventHandler::parseTrackEvents( LiveSet& LS ){
	
	// ALS loaded ?
	if( !LS.isLoaded() ){
		ofLogNotice("OfxAbletonLiveSet::alsLinkEventHandler::parseTrackEvents()", "before parsing events, make sure to parse() a live set.");
		return false;
	}
	
	// clean up
	LSTrackEvents.clear();
	
	// loop trough LS data
	for (int trackNb = 0; trackNb < LS.audiotracks.size(); trackNb++){
		
		for(int clipNb=0; clipNb < LS.audiotracks[trackNb].clips.size(); clipNb++){
				
			LSTrackEvent trackEvent( LS.audiotracks[trackNb].name, LS.audiotracks[trackNb].clips[clipNb], trackNb, clipNb );
			
			LSTrackEvents.push_back(trackEvent);
			
			clipNb ++;
		}
	}
	std:sort(LSTrackEvents.begin(), LSTrackEvents.end(), sort_by_audio_clip_time<LSTrackEvent>);
	
	return true;
}

bool alsLinkEventHandler::enableMetronomEvents(){
	//startThreadedTimer();
	
	nextMetronomEvent.clear();
	nextMetronomEvent.resize(LSMetronomEvents.size()); // ,0);
	
	bMetronomEvents = true;
	
	return true;
}

bool alsLinkEventHandler::enableMetronomEvents(ofx::AbletonLiveSet::LiveSet &LS){
	
	return parseMetronomEvents(LS) && enableMetronomEvents();
}

bool alsLinkEventHandler::parseMetronomEvents(ofx::AbletonLiveSet::LiveSet &LS){
	
	if( !LS.isLoaded() ){
		ofLogNotice("OfxAbletonLiveSet::alsLinkEventHandler::parseMetronomEvents()", "before parsing events, make sure to parse() a live set.");
		return false;
	}
	
	LSMetronomEvents.clear();
	
	// loop trough LS data
	for (int trackNb = 0; trackNb < LS.miditracks.size(); trackNb++){
		LSMetronomEvent LSE;
		LSE.timeSignature = LS.miditracks[trackNb].timeSignature;
		LSE.trackNb = trackNb;
		LSE.bpm = LS.tempo.getGlobalTempo();
		LSMetronomEvents.push_back(LSE);
	}
	return true;
}

void alsLinkEventHandler::fireNextMetronomEvents(alsLinkEventHandler::durationSecs curTime){
	
	// todo: the system can only handle 1 metronom for now...
	for(int i=0; i<LSMetronomEvents.size(); i++){
		
		if(curTime < nextMetronomEvent[i] ) continue;
			
		LSMetronomEvent LSE( LSMetronomEvents[i] );
		float oneBar = 60.0f/LSE.bpm;
		LSE.barTime = floor( curTime.count()/oneBar );
		LSE.realTime = LSE.barTime * oneBar;
		LSE.isAccent = (LSE.barTime%LSE.timeSignature.numerator)==0;
		
		//nextMetronomEvent[i] = alsLinkEventHandler::duration( LSE.realTime*1000000+oneBar*1000000 );
		
		ofNotifyEvent( alsMetronomEvent, LSE );
	}

}

// the callback remains in the timer thread. Maybe need to lock mutex ?
void alsLinkEventHandler::timerTick(alsLinkEventHandler::duration curTime ){

	// filter out unneccesary calls
	if(bNoteEvents){
		
		// no more notes ?
		if(LSNoteEvents.size()<=currentNoteEventIndex){
			bNoteEvents = false;
			return;
		}
		
		if( getAbletonElapsedTimeSec().count() >= LSNoteEvents[currentNoteEventIndex].note.time ){
			fireNextNoteEvents( getAbletonElapsedTimeSec() );
		}
	}
	if(bMetronomEvents){
		//if( getAbletonElapsedTimeSec() >= nextMetronomEvent )
		fireNextMetronomEvents( getAbletonElapsedTimeSec() );
	}
	if(bTrackEvents){
		//if( stopWatch.elapsed() >= nextMetronomEvent )
		fireNextTrackEvents( getAbletonElapsedTimeSec() );
	}
	
	return;
}

alsLinkEventHandler::duration alsLinkEventHandler::getAbletonElapsedTime() {
	alsLinkEventHandler::duration time = abletonLink::getInstance().getClock().micros() - abletonTimeOffset;
	return time;
}

alsLinkEventHandler::durationSecs alsLinkEventHandler::getAbletonElapsedTimeSec() {
	alsLinkEventHandler::duration secTime = std::chrono::duration_cast< alsLinkEventHandler::duration >( abletonLink::getInstance().getClock().micros() - abletonTimeOffset );
	return secTime;
}

void alsLinkEventHandler::enableSyncWithLive(){
	if(!bSyncingWithLive){
		bSyncingWithLive = true;
		
		ofAddListener(abletonLink::abletonLinkSyncEvent, this, &::ofx::AbletonLiveSet::alsLinkEventHandler::resetTimeline);
		//abletonLink::abletonLinkSyncEvent.add(this, &alsLinkEventHandler::resetTimeLine);
	}
}

void alsLinkEventHandler::disableSyncWithLive(){
	if(bSyncingWithLive){
		bSyncingWithLive = false;
		ofRemoveListener(abletonLink::abletonLinkSyncEvent, this, &::ofx::AbletonLiveSet::alsLinkEventHandler::resetTimeline );
	}
}

void alsLinkEventHandler::resetTimeline(abletonLinkSyncEventArgs &args){
	abletonTimeOffset = abletonLink::getInstance().getTimeline().timeAtBeat(args.currentBeats, abletonLink::getInstance().getQuantum() );
	//cout << abletonTimeOffset << endl;
}

OFX_ALS_END_NAMESPACE
