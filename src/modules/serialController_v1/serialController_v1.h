//
//  Created by Daan de Lange on 08/05/2016.
//
//	This class enables effects to easily do threaded serial communication with the karmaMapper Arduino Leonardo / Olimex hardware controller.
//

#pragma once

#include "ofMain.h"
#include "singletonModule.h"
#include "karmaSerialMsg.h"
#include "ofxSerial.h"

#define KM_SERIAL_BAUD_RATE 115200

// todo: (var) device should have a mutex ?

class serialControllerV1 : public singletonModule<serialControllerV1>, public ofThread {
	friend class ofxImGui;
	
public:
	serialControllerV1();
	virtual ~serialControllerV1();
	
	
	// virtual methods from karmaModule
	virtual bool enable();
	virtual bool disable();
	virtual void update(const animationParams& params);
	virtual void draw(const animationParams& params);
	virtual bool reset();
	virtual void showGuiWindow();
	virtual void drawMenuEntry();
	virtual bool saveToXML(ofxXmlSettings& xml) const;
	virtual bool loadFromXML(ofxXmlSettings& xml);
	
	// serialControllerV1 methods
	bool tryConnect(string _hwID="");
	bool disconnectDevice();
	virtual void threadedFunction();
	
	// serial communication
	void onSerialBuffer(const ofx::IO::SerialBufferEventArgs& args);
	void onSerialError(const ofx::IO::SerialBufferErrorEventArgs& args);
	
	// OSC Router methods
//	bool addNode( OSCNode* _node );
//	bool removeNode( OSCNode* _node );
//	bool clearAllNodes();
//	bool startOSC( int _port = KM_OSC_PORT_IN );
//	bool stopOSC();
//	void reconnectKMSA();
//	static void ImGuiShowserialControllerV1ConnectionTester();
	
protected:
	
	ofThreadChannel<karmaSerialMsg<string> > incomingMessages;
	ofThreadChannel<karmaSerialMsg<string> > outgoingMessages;
	
	list<karmaSerialMsg<string> > availableMessages;
	
	//int OSCListeningPort;
	bool bIsConnected;
	string hardwareID;
	
	// remember connected device interface
	ofx::IO::PacketSerialDevice device;
	//ofx::IO::SerialDevice device;
	
private:
	// prevents accidentally creating copies of your singleton
	serialControllerV1(serialControllerV1 const&)     = delete;
	void operator=(serialControllerV1 const&)  = delete;
	
	// called by getInstance()
	serialControllerV1(bool _isSingleton);
};


// GUI translations
#define GUIserialControllerV1Status		("Status")
