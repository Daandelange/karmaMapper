//
//  singletonModule
//  karmaMapper
//
//  Created by Daan de Lange on 12/02/2016.
//
//	An abstract class for transforming karmaMapper modules into singletons
//

#pragma once

#include "karmaModule.h"
#include "moduleFactory.h"

template<typename T>
class singletonModule : public karmaModule {
	
public:
	
	static T& getInstance(){
		if (singletonModule::typedSingletonInstance == 0) {
			singletonModule::typedSingletonInstance = getSingleton();
		}
		return *(singletonModule::typedSingletonInstance);
	}
	
protected:
	virtual ~singletonModule(){
		bInitialised = false;
		if (singletonModule::typedSingletonInstance != 0) {
			delete singletonModule::typedSingletonInstance;
		}
		singletonModule::typedSingletonInstance = 0;
	}
	inline explicit singletonModule() : karmaModule(true) {
		moduleType = "singletonModule";
		moduleName = "singletonModule";
		
		if( singletonModule::typedSingletonInstance == 0 ){
			//assert(singletonModule::typedSingletonInstance == 0);
			singletonModule::typedSingletonInstance = static_cast<T*>(this);
		}
		else {
			ofLogNotice("inline explicit singletonModule()") << "Singleton already loaded, not re-instantiating...";
		};
		
		
	}
	
private:
	static T* typedSingletonInstance;
	// your own class has to implement its own initializer function
	static T* getSingleton(){
		return new T();
	}
	
	// prevents accidentally creating copies of your singleton
	singletonModule(singletonModule const&)     = delete;
	void operator=(singletonModule const&)  = delete;
//	singletonModule& operator=(const singletonModule& other){
//		// always keep the singleton version
//		// todo: could be a better condition...
//		if ( isSingleton ||  other.isSingleton ) {
//			return getInstance();
//		}
//		else return *this;
//	}
};

template<typename T>
T* singletonModule<T>::typedSingletonInstance = 0;

