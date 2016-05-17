//
//  karmaSerialMsg.h
//  karmaMapper
//
//  karmaSerialMsg.h
//  karmaMapper
//
//  Created by Daan de Lange on 08/05/2016.
//
//	This class routes OSC messages (to ofEvents and other)
//
//

#pragma once

#include "ofMain.h"
#include "karmaSerialMsg.h"

template<typename T>
class karmaSerialMsg {
	
public:
	karmaSerialMsg(string _addr, T _value): address(_addr), value(_value) {
	}
	~karmaSerialMsg(){
		age = ofGetElapsedTimef();
	};
	
	// fix for no default constructor
//	karmaSerialMsg& operator=(const karmaSerialMsg& crap) {
//		return *this;
//	}
	
	//string getAddress() const;
	string getAddress() const{
		return address;
	}
	//T getValue() const;
	T getValue() const{
		return value;
	}
	
protected:
	string address;
	T value;
	float age; // relative to ofGetElapsedTimef()
	
private:
	
};