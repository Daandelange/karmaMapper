//
//  abletonLink.h
//  karmaMapper
//
//  Created by Daan de Lange on 24/03/2016.
//
//
//

#pragma once

#include "ofMain.h"
#include "ofEvents.h"
#include "abletonLinkEvents.h"
#include "singletonModule.h"
#include "OSCNode.h"
#include "OSCRouter.h"
//#include "ofxAbletonLink.h"
#include "alsLinkEventHandler.h"

// this class handles OSC events comming from abletonLink.pd, caches the data and calculate some more with it.
// also fires MIR events
// todo: listen to multiple ports at the same time ?

#define KM_AL_OSC_ADDR "localhost"
#define KM_AL_OSC_PORT_OUT 2345
#define KM_AL_PREFIX "/midi/clock/"
#define KM_AL_PREFIX_LEN sizeof(KM_AL_PREFIX)-1

class abletonLink : public OSCNode, public singletonModule<abletonLink> {
	
public:
	abletonLink( );
	~abletonLink();

	// singleton stuff
//	static abletonLink& getInstance(){
//		static abletonLink instance; // Guaranteed to be destroyed and instantiated on first use
//		return instance;
//	}
	
	// prevents accidentally creating copies of your singleton
	abletonLink(abletonLink const&)     = delete;
	void operator=(abletonLink const&)  = delete;
	
	struct AbletonLinkStatus{
		double beat = 0;
		double phase = 0;
		std::chrono::microseconds time;
		AbletonLinkStatus() : beat(0.0), phase(0.0), time(std::chrono::microseconds(0)) {}
	};
	
	// virtuals from OSCNode
	virtual bool canHandle( const ofxOscMessage &_msg ) const;
	virtual bool handle( const ofxOscMessage &_msg );
	virtual void detachNode();
	
	// listeners
	static ofEvent<abletonLinkSyncEventArgs> abletonLinkSyncEvent;
	
	void setupLink();
	double getTempo();
	void setQuantum(double quantum);
	double getQuantum();
	bool linkIsEnabled() const;
	std::size_t getNumPeers();
	void updateLink();
	bool enableLink(bool _enable);
	ableton::Link::Clock getClock();
	ableton::Link::Timeline getTimeline();
	AbletonLinkStatus ALStatus;
	
	// VIRTUALS FROM karmaModule
	virtual bool enable();
	virtual bool disable();
	virtual void update(const animationParams& params);
	virtual void draw(const animationParams& params);
	virtual bool reset();
	void showGuiWindow();
	void drawMenuEntry();
    virtual bool saveToXML(ofxXmlSettings& xml) const;
    virtual bool loadFromXML(ofxXmlSettings& xml);
	
	// basic functions
	//bool parseALSFile();
	//bool stopListening();

	// getters
	double getAbletonElapsedBeats() const;
	float  getAbletonElapsedTimeSec() const;
	std::chrono::microseconds  getAbletonElapsedTime() const;
	void resetAbletonTimeLine(double _beatsOrigin = 0.f);
	
	// listeners
	//static ofEvent<abletonLinkSyncEventArgs> abletonLinkSyncEvent;
//	static ofEvent<abletonLinkTempoEventArgs> abletonLinkTempoEvent;
//	static ofEvent<abletonLinkNoteEventArgs> abletonLinkNoteEvent;
//	static ofEvent<abletonLinkFloatEventArgs> abletonLinkFloatEvent;
	
	
protected:
	
	//ofxAbletonLink AL;
	//AbletonLinkStatus ALStatus;
	bool bEnableSyncEvents;
	//ableton::link::platform::Controller moduleTimeController;
	
	// keep an instance of this alive to be notified of events
	//ofxAbletonLiveSet::EventHandler eventHandler;
	
	static ableton::Link* link;
	
	// listen to ableton for syncing with liveOSC
	ofMutex oscMutex; // needed because audioIn() runs on a separate thread
	struct oscParams {
		string host = KM_AL_OSC_ADDR;
		int port = KM_AL_OSC_PORT_OUT;
	} oscSendParams;
	
	double abletonBeatOffset = 0;
	std::chrono::microseconds abletonTimeOffset;
	
	
private:
	
	double quantum_ = 4;
	
	
	// threaded var
	bool bRequestTimeReset = true;
};


