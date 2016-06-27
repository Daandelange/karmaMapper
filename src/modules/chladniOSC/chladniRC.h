//
//  chladniRC
//  karmaMapper
//
//  Created by Daan de Lange on 10/05/2015.
//
//
//

#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxSerial.h"

// you might have to adjust these to match your setup
#define KM_CHLADNI_NUM_ELECTROVALVES 3
#define KM_CHLADNI_NUM_FLOWMETERS 1
#define KM_CHLADNI_NUM_LED_STRIPS 2
#define KM_CHLADNI_PD_OSC_DEFAULT_PORT 12002
#define KM_CHLADNI_PD_OSC_DEFAULT_HOST "localhost"

// this class sends OSC commands to chladni
// Thus allowing you to remote control it.

// note:
// For now I changed ofxOSC/ofxOscSender::shutdown() from private to protected.

struct chladniWaterControlSettingsStruct {
	
	chladniWaterControlSettingsStruct(){
		
	}
	
	float waterFlowRate[KM_CHLADNI_NUM_ELECTROVALVES] = {0.f};
	float flowMeters[KM_CHLADNI_NUM_FLOWMETERS] = {0.f};
	float LEDStripsIntensityManu[KM_CHLADNI_NUM_LED_STRIPS] = {0.f};
	float LEDStripsIntensityAuto[KM_CHLADNI_NUM_LED_STRIPS] = {0.f};
};

class chladniRC : public ofxOscSender {
	friend class ofxImGui;
	
public:
	chladniRC( );
	~chladniRC();
	
	// singleton stuff
	static chladniRC& getInstance(){
		static chladniRC instance; // Guaranteed to be destroyed and instantiated on first use
		return instance;
	}
	
	// prevents accidentally creating copies of your singleton
	chladniRC(chladniRC const&)     = delete;
	void operator=(chladniRC const&)  = delete;
	
	// basic functions
	bool setupPureDataOSC();
	
	// Sound Vibration RC commands
	bool pureDataIsConnected() const;
	bool sendBang(const string& _name);
	bool sendPing();
	bool sendFloat(const string& _name, const float& _float);
	
	// Arduino RC Commands (water control)
	bool setSolenoidFlow(const int& _solenoidID, const float& _flowRate);
	float getSolenoidFlow(const int& _solenoidID) const;
	float getFlowMeterRate(const int& _flowmeterID) const;
	float getLEDStripIntensityManu(const int& _LEDStripID) const;
	float getLEDStripIntensityAuto(const int& _LEDStripID) const;
	bool setLEDStripIntensityAuto(const int& _LEDStripID, float _intensity);
	bool arduinoIsConnected() const;
	bool connectToArduino(const int& _deviceID);
	bool connectToArduino( const ofx::IO::SerialDeviceInfo& _deviceInfo );
	void onSerialBuffer(const ofx::IO::SerialBufferEventArgs& args);
	void onSerialError(const ofx::IO::SerialBufferErrorEventArgs& args);
	bool setupArduinoSerial();
	string getArduinoDevicePort() const;
	void disconnectArduino();
	void pingArduino();
	string getLastArduinoPingMessage() const {
		return arduinoLastPingReturn;
	}
	
	// getters
	bool isConnected() const;
	
protected:
	
	bool bPdIsConnected;
	
	// Arduino Control (water + light control)
	ofx::IO::PacketSerialDevice arduino;
	
	// setting variables
	chladniWaterControlSettingsStruct waterControlSettings;
	string arduinoLastPingReturn;
	
	// analysis variables
	
	
	ofMutex oscMutex; // needed because audioIn() runs on a separate thread
	
private:
	
	
};


