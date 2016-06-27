//
//  chladniRC.cpp
//  karmaMapper
//
//  Created by Daan de Lange on 05/06/2016.
//  Copyright (c) 2014 __MyCompanyName__. All rights reserved.
//

#include "chladniRC.h"

// - - - - - - - -
// CONSTRUCTORS
// - - - - - - - -
chladniRC::chladniRC( ){
	setupPureDataOSC();
	setupArduinoSerial();
}

chladniRC::~chladniRC(){
	
	// close Pd OSC
	ofxOscSender::disableBroadcast();
	//ofxOscSender::shutdown();
	
	// close Arduino Serial
	disconnectArduino();
}

bool chladniRC::setupPureDataOSC(){
	ofxOscSender::setup(KM_CHLADNI_PD_OSC_DEFAULT_HOST, KM_CHLADNI_PD_OSC_DEFAULT_PORT);
	bPdIsConnected = true;
	// (shuts down automatically)
	
	return bPdIsConnected;
}

bool chladniRC::pureDataIsConnected() const {
	return bPdIsConnected;
}


// - - - - - - - -
// RC COMMANDS - Remote Control chladni / cymatics generator (pd)
// - - - - - - - -
bool chladniRC::sendBang(const string& _name) {
	if(!bPdIsConnected){
		return false;
	}
	
	ofxOscMessage m;
	m.setAddress("/bang");
	m.addStringArg(_name);
	sendMessage(m);
	return true;
}

bool chladniRC::sendFloat(const string& _name, const float& _float){
	if(!bPdIsConnected){
		return false;
	}
	
	ofxOscMessage m;
	m.setAddress(_name);
	m.addFloatArg(_float);
	sendMessage(m);
	return true;
}

bool chladniRC::sendPing(){
	if(!bPdIsConnected){
		return false;
	}
	
	ofxOscMessage m;
	m.setAddress("/ping");
	m.addImpulseArg();
	sendMessage(m);
	return true;
}

// - - - -
// Electro Valves Control (Arduino)
// - - - -

// args: solenoid num, waterflow (0-1);
bool chladniRC::setSolenoidFlow(const int& _solenoidID, const float& _flowRate){
	
	if( arduinoIsConnected() ){
	
		if( _solenoidID >= 0 && _solenoidID < KM_CHLADNI_NUM_ELECTROVALVES ){
		
			if( _flowRate < KM_CHLADNI_NUM_ELECTROVALVES && _flowRate >= 0 ){
				
				waterControlSettings.waterFlowRate[_solenoidID] = _flowRate;
				
				// Create a byte buffer.
				ofx::IO::ByteBuffer buffer("setEV:");
				buffer.writeByte( (uint8_t)(_solenoidID) );
				buffer.writeByte( '-' );
				buffer.writeByte( (uint8_t)(_flowRate*255) );
				
				// Send the byte buffer.
				// ofx::IO::PacketSerialDevice will encode the buffer, send it to the
				// receiver, and send a packet marker.
				try {
					arduino.send(buffer);
				}
				catch(...){
					// failed
					cout << "Error sending solenoid flow rate" << endl;
				}
				
				return true;
			}
			else {
				// invalid flow value
			}
		}
		else {
			// invalid solenoid ID
#ifdef KARMAMAPPER_DEBUG
			ofLogWarning("chladniRC::setWaterFlow") << "Invalid device ID: "<< _solenoidID;
#endif
		}
	}
	else {
		// arduino not connected...
	}
	
	return false;
}

float chladniRC::getSolenoidFlow(const int& _solenoidID) const {
	if(_solenoidID<0 || _solenoidID>KM_CHLADNI_NUM_ELECTROVALVES-1){
		return -1.f;
	}
	return waterControlSettings.waterFlowRate[_solenoidID];
}

float chladniRC::getFlowMeterRate(const int& _flowmeterID) const {
	if(_flowmeterID<0 || _flowmeterID>KM_CHLADNI_NUM_FLOWMETERS-1){
		return -1.f;
	}
	return waterControlSettings.flowMeters[_flowmeterID];
}

float chladniRC::getLEDStripIntensityManu(const int& _LEDStripID) const {
	if(_LEDStripID<0 || _LEDStripID>KM_CHLADNI_NUM_LED_STRIPS-1){
		return -1.f;
	}
	return waterControlSettings.LEDStripsIntensityManu[_LEDStripID];
}

float chladniRC::getLEDStripIntensityAuto(const int& _LEDStripID) const {
	if(_LEDStripID<0 || _LEDStripID>KM_CHLADNI_NUM_LED_STRIPS-1){
		return -1.f;
	}
	return waterControlSettings.LEDStripsIntensityAuto[_LEDStripID];
}

bool chladniRC::setLEDStripIntensityAuto(const int &_LEDStripID, float _intensity) {
	if(_LEDStripID<0 || _LEDStripID>KM_CHLADNI_NUM_LED_STRIPS-1){
		// invalid led strip
		return false;
	}
	
	// restrict intensity range ?
	if(_intensity < 0) _intensity = 0.f;
	else if(_intensity > 1) _intensity = 1.f;
	
	if( arduinoIsConnected()){
		waterControlSettings.LEDStripsIntensityAuto[_LEDStripID] = _intensity;
		
		// Create a byte buffer.
		ofx::IO::ByteBuffer buffer("setLEDstrips:");
		buffer.writeByte( (uint8_t)(_LEDStripID) );
		buffer.writeByte( '-' );
		buffer.writeByte( (uint8_t)(_intensity*255) );
		
		// Send the byte buffer.
		// ofx::IO::PacketSerialDevice will encode the buffer, send it to the
		// receiver, and send a packet marker.
		try {
			arduino.send(buffer);
		}
		catch(...){
			// failed
		}
		
		return true;
	}
	
	return false;
}

bool chladniRC::arduinoIsConnected() const {
	
	if( arduino.isOpen() ){
		return true;
	}
	
	return false;
}

bool chladniRC::connectToArduino(const int &_deviceID){
	
	// get devices list
	std::vector<ofx::IO::SerialDeviceInfo> devicesInfo = ofx::IO::SerialDeviceUtils::listDevices();
	
//	for (std::size_t i = 0; i < devicesInfo.size(); ++i) {
//		cout << devicesInfo[i];
//	}
	
	if( !devicesInfo.empty() && _deviceID >= 0 && _deviceID < devicesInfo.size() ){
		
		devicesInfo.size();
		ofx::IO::SerialDeviceInfo deviceInfo = devicesInfo[_deviceID];
		
		return connectToArduino(deviceInfo);
	}
	else {
		// device unavailable
	}
	
}

bool chladniRC::connectToArduino( const ofx::IO::SerialDeviceInfo& _deviceInfo ){
	std::vector<ofx::IO::SerialDeviceInfo> devicesInfo = ofx::IO::SerialDeviceUtils::listDevices();
	
	if (!devicesInfo.empty()){
		for (std::size_t i = 0; i < devicesInfo.size(); ++i) {
			if( devicesInfo[i].getHardwareId() == _deviceInfo.getHardwareId() ){
				
				// disonnect
				disconnectArduino();
				
				// setup
				if( arduino.setup(devicesInfo[i], 115200) ){
					arduino.registerAllEvents(this);
					
#ifdef KARMAMAPPER_DEBUG
					ofLogNotice("chladniRC::connectToArduino") << "Connection to Arduino successful: "<< devicesInfo[i].getDescription() << " [open=" << arduino.isOpen() << "]";
#endif
					return arduino.isOpen();
				}
				else {
					// connection failed
#ifdef KARMAMAPPER_DEBUG
					ofLogWarning("chladniRC::connectToArduino") << "Failed to connect to Arduino device "<< devicesInfo[i].getDescription();
#endif
					return false;
				}
			}
		}
			// device not found or connected
#ifdef KARMAMAPPER_DEBUG
		ofLogWarning("chladniRC::connectToArduino") << "Device not found or connected: "<< _deviceInfo.getDescription();
#endif
		_deviceInfo.getDescription();
		
	}
	
	else {
		// no devices unavailable
#ifdef KARMAMAPPER_DEBUG
		ofLogWarning("chladniRC::connectToArduino") << "No Arduino devices available..."<< _deviceInfo.getDescription();
#endif
	}
	
	
	
	return false;
}

void chladniRC::onSerialBuffer(const ofx::IO::SerialBufferEventArgs& args){
	
	// Buffers will show up here when the marker character is found.
	string message(args.getBuffer().toString());
	//serialMessages.push_back(message);
	
	// get pingback
	string compareString("pingback:");
	if(message.length()>=compareString.length() ){
		if( message.compare(0,compareString.length(),compareString,0,compareString.length())==0){
			arduinoLastPingReturn = message.substr(compareString.length()-1,message.npos);
			return;
		}
	}
	
	//
	compareString = "EVStatus:";
	if(message.length()>=compareString.length() ){
		if( message.compare(0,compareString.length(),compareString,0,compareString.length())==0){
			
			string rawValue = message.substr(compareString.length(),message.npos);
			
			auto dash = rawValue.find("-");
			if(dash != rawValue.npos){
				int valveID = ofToInt( rawValue.substr(0,dash) );
				
				if( valveID>=0 && valveID<KM_CHLADNI_NUM_ELECTROVALVES){
				
					int valveValue = ofToInt( rawValue.substr(dash+1,rawValue.npos-(dash+1) ) );
					waterControlSettings.waterFlowRate[valveID] = ((float)valveValue)/255.f;
				}
				
			}
			return;
		}
	}
	
	// get the manual LED strip value
	compareString = "ledStripIntensityManu:";
	if(message.length()>=compareString.length() ){
		if( message.compare(0,compareString.length(),compareString,0,compareString.length())==0){
			
			string rawValue = message.substr(compareString.length(),message.npos);
			
			auto dash = rawValue.find("-");
			if(dash != rawValue.npos){
				int lightID = ofToInt( rawValue.substr(0,dash) );
				
				if( lightID>=0 && lightID<KM_CHLADNI_NUM_LED_STRIPS){
					
					int lightValue = ofToInt( rawValue.substr(dash+1,rawValue.npos-(dash+1) ) );
					waterControlSettings.LEDStripsIntensityManu[lightID] = ((float)lightValue)/255.f;
				}
				
			}
			return;
		}
	}
	
	// still not returned? --> unrecognised message
	cout << "Unrecognised chladni SERIAL message:\t" << message << endl;
}

void chladniRC::onSerialError(const ofx::IO::SerialBufferErrorEventArgs& args){
	// Errors and their corresponding buffer (if any) will show up here.
	string message(args.getException().displayText());
	//serialMessages.push_back(message);
}

bool chladniRC::setupArduinoSerial(){
	
	// by default, connect to the first Arduino-looking device
	
	std::vector<ofx::IO::SerialDeviceInfo> devicesInfo = ofx::IO::SerialDeviceUtils::listDevices();
	
	if (!devicesInfo.empty()){
		for (std::size_t i = 0; i < devicesInfo.size(); ++i) {
			if( devicesInfo[i].getDescription().find("Arduino") != devicesInfo[i].getDescription().npos ){
				
				// try connect
				if(connectToArduino(devicesInfo[i])){
					return true;
				}
				else {
					// continue searching for arduino devices
					continue;
				}
			}
			else {
				// continue searching for arduino devices
				continue;
			}
			
		}
	}
	
	return false;
}

string chladniRC::getArduinoDevicePort() const {
	if( !arduino.isOpen() ){
		return "";
	}
	
	return arduino.getPortName();
}

void chladniRC::disconnectArduino(){
	arduino.unregisterAllEvents(this);
	if(arduino.isOpen()){
		arduino.setup("", -1);
	}
}

void chladniRC::pingArduino(){
	if( arduinoIsConnected() ){
		// Create a byte buffer.
		ofx::IO::ByteBuffer buffer("ping:karmaMapper");
		
		// Send the byte buffer.
		// ofx::IO::PacketSerialDevice will encode the buffer, send it to the
		// receiver, and send a packet marker.
		try {
			arduino.send(buffer);
			
			// clear ping return msg
			arduinoLastPingReturn = "";
		}
		catch(...){
			// failed
		}
	}
	
	return;
}

// GETTERS
bool chladniRC::isConnected() const {
	return pureDataIsConnected() && arduinoIsConnected();
}

//mirData chladniRC::mirCache;// = mirData();

