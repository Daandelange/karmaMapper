//
//  karmaParam.h
//  karmaMapper
//
//  Created by Daan on 30/05/2016.
//
//

#pragma once

#include "ofMain.h"
#include "ofxImGui.h"
#include <type_traits> // needed for std::is_same

class karmaHasUID {

public:
	karmaHasUID(string& _name) {
		if(_name.length()<1){
			_name="Object";
		}
		
		registerUniqueIdentifier(_name, this);
	}
	
	virtual ~karmaHasUID(){
		// rm name from available names
		if(allUIDs.find(this)!=allUIDs.end()){
			allUIDs.erase(this);
		}
	}
	
	string getUID(){
		return UID;
	}
	
	static bool registerUniqueIdentifier(string& _name, karmaHasUID* instance){
		
		if(instance == nullptr){
			return false;
		}
		
		// instance already registered? Update it's ID.
		for(auto it=allUIDs.begin(); it!=allUIDs.end(); ++it){
			if(it->first == instance){
				// update name if changed
				if(it->second.compare(_name)!=0){
					makeIdentifierUnique(_name);
					it->second = _name;
				}
				return true;
			}
		}
		
		// instance not found, add new one
		makeIdentifierUnique(_name);
		allUIDs.emplace(std::make_pair(instance, _name));
		
		return true;
	}
	
	static void makeIdentifierUnique(string& _name, unsigned int _incrementer=0){
		
		// todo: if incrementer is 0, try to guess it from the end of the string
		
		for(auto it=allUIDs.begin(); it!=allUIDs.end(); ++it){
			if(_incrementer==0){
				if(it->second.compare(_name) == 0){
					makeIdentifierUnique(_name, 1);
					return;
				}
			}
			else {
				if(it->second.compare(_name.append("_"+ofToString(_incrementer))) == 0){
					makeIdentifierUnique(_name, _incrementer+1);
					return;
				}
			}
		}
	}
	
protected:
	string UID;
	
	static std::map<karmaHasUID*, std::string> allUIDs;
};


// inspired from OF's ofParam<type>
template <typename T>
class karmaParam : public karmaHasUID {

public:
	
    karmaParam(T _value, string _paramName) : karmaHasUID(_paramName), value(_value) {
		
		registerUniqueIdentifier(_paramName, this);
	}
	
	virtual ~karmaParam(){
		
	}
	
	virtual bool set(const T& _value){
		value = _value;
		
		// todo: notify
		
		return true;
	}
	
	// copy constructor
	karmaParam<T>& operator=(T const & _value){
		set(_value);
		
		return *this;
	}
	
//	karmaParam<T>& operator=(T&& _value){
//		set(_value);
//		
//		return *this;
//	}
	
	// cast karmaParam<type> to type.
	T& operator=(const karmaParam<T>& _param) const{
		return _param.get();
	}
	T& operator=(const karmaParam<T>&& _param){
		return _param.get();
	}
	
	// implicit conversion operator
	//operator T() const { return get(); }
	operator T&() { return get(); }
	
	virtual T& get(){
		return value;
	}
	
	// copy value to respect const correctness (but slower)
	virtual T get() const{
		return value;
	}
	
	// alternative method to get() the value
	T* operator->(){
		return &get();
	}
	
	virtual bool drawToImGui(){
		
//		if( std::is_same<T, int>::value ){
			ostringstream out;
			out << &value;
			string tmpStr = out.str();
			ImGui::LabelText( getUID().c_str() , "%s", tmpStr.c_str() );
			
			//ImGui::DragInt( getUID(), &value);
			return true;
//		}
		
		return false;
	}

protected:
	T value;
	
	// todo:
	bool bSavingEnabled;
	bool bReadOnly;
};


// creates and keeps in sync a copy of the variable, to access from a thread
template <typename T>
class karmaThreadedParam : public karmaParam<T> {

public:
//	karmaThreadedParam(T _value) : karmaParam<T>(_value), threadedValue(_value) {
//		
//	}
	
	karmaThreadedParam(T _value, string _name) : karmaParam<T>(_value, _name), threadedValue(_value) {
		
	}
	
	virtual ~karmaThreadedParam(){
		paramToThread.close();
		paramFromThread.close();
	}
	
	karmaThreadedParam<T>& operator=(T const & _value){
		set(_value);
		
		return *this;
	}
//	// cast karmaThreadedParam<type> to type.
//	T& operator=(const karmaThreadedParam<T>& _param){
//		return _param.get();
//	}
	
	// implicit conversion operator
	operator karmaParam<T>&() { return *this; }
	
	virtual bool set(const T& _value){
		//if(ofThread::isMainThread()){
		if( main_thread_id == std::this_thread::get_id() ){
			// set from main thread
			karmaParam<T>::set(_value);
			paramToThread.send(_value);
		}
		else {
			setFromThread(_value);
		}

        return true; // tmp
	}
	
	// use to change the value from thread
	virtual bool setFromThread(const T& _value){
		threadedValue = _value;
		paramFromThread.send(_value);
		
		// todo: notify

        return true; // tmp
	}
	
	virtual T& get(){
		//if(ofThread::isMainThread()){
		if( main_thread_id == std::this_thread::get_id() ){
			// get from main thread
			return karmaParam<T>::get();
		}
		else {
			return getFromThread();
		}
	}
	
	virtual T get() const {
		return karmaParam<T>::value;
	}
	
	virtual T& getFromThread(){
		return threadedValue;
	}
	
	virtual T getFromThread() const{
		return threadedValue;
	}
	
	// to sync the variable on the thread's update()
	virtual bool syncFromThread(){
		// empty queue and keep last param
		bool changed = false;
		while(paramToThread.tryReceive(threadedValue)){
			changed = true;
		}
		if(changed){
			// todo: notfiy change
		}
		return changed;
	}
	
	virtual bool syncFromMainThread(){
		// empty queue and keep last param
		bool changed = false;
		while(paramToThread.tryReceive(karmaParam<T>::value)){
			changed = true;
		}
		if(changed){
			// todo: notifiy threaded change
		}
		return true;
	}
	
	virtual void triggerUpdate(){
		if( main_thread_id == std::this_thread::get_id() ){
			paramToThread.send(karmaParam<T>::value);
		}
		else {
			paramFromThread.send(threadedValue);
		}
	}
		
	static std::thread::id main_thread_id;
	
protected:
//	string getUniqueName(){
//		return "";
//	}
	
	T threadedValue; // to access from thread only
	
	ofThreadChannel<T> paramToThread;
	ofThreadChannel<T> paramFromThread;
};
template <typename T>
std::thread::id karmaThreadedParam<T>::main_thread_id = std::this_thread::get_id();


// Int param alias
//typedef karmaParam<int> karmaIntParam;
//karmaParam::karmaParam

class karmaIntParam : public karmaParam<int> {

public:
	
	//template <typename T>
	karmaIntParam(int _value, string _name) : karmaParam<int>(_value, _name) {
		
	}
	
	virtual ~karmaIntParam(){
		
	}
	
	virtual bool drawToImGui(){
		ImGui::DragInt( getUID().c_str(), &value);

        return true; // tmp
	}
};

// Enum param implementation
//class karmaEnumeration {
//public:
////	enum {
////	
////	};
//	
//	virtual int getSize() = 0;
//	static enum  { } enumeration;// = 0;
//	
//private:
//	
//	
//};

class karmaEnumParam : public karmaIntParam {

public:
	
	//template <typename T>
	karmaEnumParam(int _value, string _name) : karmaIntParam(_value, _name) {
		
	}
	
	virtual ~karmaEnumParam(){
		
	}
	
	// convert int to float
	//operator float() const { return (float)get(); }
	
	virtual bool drawToImGui(){
		ImGui::DragInt( getUID().c_str(), &value);
        return true; // tmp
	}
};
