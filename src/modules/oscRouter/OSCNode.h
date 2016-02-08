//
//  OSCNode.h
//  karmaMapper
//
//  OSCNode.h
//  karmaMapper
//
//  Created by Daan de Lange on 30/04/2016.
//
//	This class routes OSC messages (to ofEvents and other)
//
//

#pragma once

#include "ofMain.h"
#include "ofxOscMessage.h"

// based upon this model to register an OSC router
// http://gamedev.stackexchange.com/a/17759

class OSCNode {
	
public:
	OSCNode( );
	~OSCNode();
	// fix for no default constructor
	OSCNode& operator=(const OSCNode& crap) { return *this; }
	
	virtual bool canHandle( const ofxOscMessage &_msg ) const = 0;
	virtual bool handle( const ofxOscMessage &_msg ) = 0;
	virtual void detachNode() = 0;
	
protected:
	//ofMutex ooscMutex;
	vector<string> moduleDependencies;
	
private:
	
};