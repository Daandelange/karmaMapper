//
//  alsParser
//  karmaMapper
//
//  Created by Daan de Lange on 24/03/2016.
//
//
//

#pragma once

#include "ofMain.h"
#include "ofEvents.h"
#include "alsParserEvents.h"
#include "alsLinkEventHandler.h"
#include "singletonModule.h"
#include "ofxAbletonLiveSet.h"
#include "ofxAbletonLink.h"
#include "abletonLink.h"

// this class handles OSC events comming from alsParser.pd, caches the data and calculate some more with it.
// also fires MIR events

#define KM_LG_OSC_ADDR "localhost"
#define KM_LG_OSC_PORT_OUT 2345
#define KM_ALLG_PREFIX "/allg/"
#define ALLG_PREFIX_LEN sizeof(KM_ALLG_PREFIX)-1

class alsParser : public singletonModule<alsParser> {
	
public:
	alsParser( );
	~alsParser();
	
	// singleton stuff
//	static alsParser& getInstance(){
//		static alsParser instance; // Guaranteed to be destroyed and instantiated on first use
//		return instance;
//	}
	
	// prevents accidentally creating copies of your singleton
//	alsParser(alsParser const&)     = delete;
//	void operator=(alsParser const&)  = delete;
	
	
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
	bool parseALSFile();
	//bool stopListening();

	
	// listeners
//	static ofEvent<alsParserBangEventArgs> alsParserBangEvent;
//	static ofEvent<alsParserTempoEventArgs> alsParserTempoEvent;
//	static ofEvent<alsParserNoteEventArgs> alsParserNoteEvent;
//	static ofEvent<alsParserFloatEventArgs> alsParserFloatEvent;
	//void resetTimeLine(abletonLinkSyncEventArgs &args);
	
	
protected:
	void parseEventsFromLS();
	
	ofxAbletonLiveSet::LiveSet LS;
	string alsFilePath;
	bool bEnableAbletonLinkSyncing;
	bool bEnableNoteEvents;
	bool bEnableTrackEvents;
	bool bEnableMetronomEvents;
	int eventTimeDelay; // ms
	
	// keep an instance of this alive to be notified of events
	ofxAbletonLiveSet::alsLinkEventHandler eventHandler;
	
	
private:
	
	
};


