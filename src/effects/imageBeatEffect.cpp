//
//  imageBeatEffect.cpp
//  karmaMapper
//
//  Created by Daan de Lange on 12/3/14.
//
//  - - - -
//
//  Parent class for all effects.
//  Implements some standard methods for overall usage.
//

// tmp disabled
#ifdef PLEASESKIPME

#include "imageBeatEffect.h"

imageBeatEffect::imageBeatEffect(){
	//basicEffect::basicEffect();
	
	ofScopedLock lock(effectMutex);
	
	// effect type mutch match with class
	effectType = "imageBeatEffect";
	
	beatItemSpeed = 0;
	intensity=0;
	totalbeatItemsNb=0;
	
	beatItems.clear();
	beatsToAdd.clear();
}

imageBeatEffect::~imageBeatEffect(){
	//basicEffect::~basicEffect();
	
	ofScopedLock lock(effectMutex);
	cout << "beatEffectMutexLock+unlock ~" << endl;
	
	/*for(std::list<imageBeatEffectItem>::reverse_iterator it=beatItems.rbegin(); it!= beatItems.rend(); it--){
			it++;
			it= std::list<imageBeatEffectItem>::reverse_iterator( beatItems.erase( it.base() ));
			it--;
	}*/
	//beatItems.erase();
	
	//effectMutex.unlock();
	//effectMutex.unlock();
	ofRemoveListener(mirReceiver::mirOnSetEvent, this, &imageBeatEffect::onSetEventListener);
	ofRemoveListener(durationReceiver::durationFloatEvent, this, &imageBeatEffect::floatListener);
}

bool imageBeatEffect::initialise(){
	//bool success = basicEffect::initialise();
	
	//if(success){ // tmp
		//effectMutex.lock();
		cout << "beatEffectMutexLock init" << endl;
		isLoading = true;
		
		// do stuff here
		ofAddListener(mirReceiver::mirOnSetEvent, this, &imageBeatEffect::onSetEventListener);
		ofAddListener(durationReceiver::durationFloatEvent, this, &imageBeatEffect::floatListener);
		//ofAddListener(mirReceiver::mirTempoEvent, this, &lineEffect::tempoEventListener);
		
		isLoading = false;
		isInitialised=true;
		//effectMutex.unlock();
		cout << "beatEffectMutexunLock init" << endl;
	//}
	
	return isInitialised;
}

bool imageBeatEffect::render(){
	ofScopedLock lock(effectMutex);
	
	//cout << "beatEffectMutexLock render" << endl;
	if( !isReady() || shapes.size()==0 ) return;
	
	for(std::list<imageBeatEffectItem>::iterator it=beatItems.begin(); it!= beatItems.end(); ++it){
		(*it).render();
	}
	//cout << "beatEffectMutexunLock render" << endl;
}

void imageBeatEffect::update(){
	
	// you can chose to do the default behaviour too or not by commenting this out
	//basicEffect::update(); // tmp
	
	//effectMutex.lock();// lock(effectMutex);
	//cout << "beatEffectMutexLock upd" << endl;
	
	ofScopedLock lock(effectMutex);
	if(!isReady()) return;
	
	// add items ?
	int i= beatsToAdd.size()-1;
	if(i>2) i=2;
	if(shapes.size()!=0) for(; i>=0;i--){
		basicShape* beatItem = beatsToAdd[0];
		beatItems.push_back( getBeatItem( beatItem) );
		totalbeatItemsNb++;
		beatsToAdd.pop_back();
	}
	
	// check for dead lines
	for(std::list<imageBeatEffectItem>::reverse_iterator it=beatItems.rbegin(); it!= beatItems.rend(); it--){
		
		if( !(*it).isAlive() ) {
			it++;
			it= std::list<imageBeatEffectItem>::reverse_iterator( beatItems.erase( it.base() ));
			it--;
		}
	}
	//effectMutex.unlock();
	//cout << "beatEffectMutexunLock upd" << endl;
}

// resets all values
// overrule this function with your own.
void imageBeatEffect::reset(){
	basicEffect::reset();
	
	ofScopedLock lock(effectMutex);
	cout << "beatEffectMutexLock rst" << endl;
	beatItems.clear();
	cout << "beatEffectMutexunLock rst" << endl;
}

// can be removed ?
bool imageBeatEffect::grabSomeShapes(){
	return true;// tmp
	ofScopedLock lock(effectMutex);
	
	// clear
	shapes.clear();
	shapes.resize(0);
	
	// we just want all vertex shapes
	//shapes = shapesHandler::getInstance().getShapesByType("vertexShape");
	
	// then sort them by group
	//map<int, int> groups; // <index, count>
	shapeGroups.clear();
	for(int i=0; i<shapes.size(); i++){
		int group = shapes[i]->getGroupID();
		
		// create map entry ?
		if( shapeGroups.find(group) == shapeGroups.end() ){
			shapeGroups[group] = vector<int>();
		}

		shapeGroups[group].push_back(i);
	}
	
	// return
	return shapeGroups.size() > 0;
}

// to rename : imageBeatItenOnRandomShape
// note: when you call this function, mutex must be locked
// ONLY call this from the main thread // GL-thread
imageBeatEffectItem imageBeatEffect::getRandomBeatItem(){
	//ofScopedLock lock(effectMutex);
	
	basicShape* shape = shapes[ round( ofRandom(-0.49f,shapes.size()-0.51f)) ];

	// return
	return imageBeatEffectItem( shape );
}
imageBeatEffectItem imageBeatEffect::getBeatItem(basicShape* _shape){
	if(_shape==NULL) return; // todo: how to return a right value here ?
	
	// return
	return imageBeatEffectItem( _shape );
}

// - - - - - - - - -
// EVENT LISTENERS
// - - - - - - - - -
void imageBeatEffect::floatListener(durationFloatEventArgs &_args){
	if( _args.track.compare("imageBeatEffectIntensity")!=0 ) return;
	
	ofScopedLock lock(effectMutex);
	intensity = _args.value;
	//lines.push_back();
}

/*void imageBeatEffect::noteEventListener(ofx::AbletonLiveSet::LSNoteEvent &noteEvent){
	return false;
	ofScopedLock lock(effectMutex);
	//lines.push_back();
}*/

void imageBeatEffect::onSetEventListener(mirOnSetEventArgs &_args){
	if( _args.source.compare("aubioOnSet")==0 ){
		
		ofScopedLock lock(effectMutex);
		
		if(shapes.size()==0 || !isReady() ) return; // not yet bound with a shape
		
		//cout << "beatEffectMutexLock tmpo" << endl;
		int amount = round(mirReceiver::mirCache.zcr*100.f*intensity+1.f);
		//cout << amount<< endl;
		// spawn some beats
		if(amount > 20) amount = 20;
		for(int i=0; i<amount;i++){
			basicShape* shape;
			int rand = round( ofRandom(-0.49f, shapes.size()-0.51f ) );
			//cout << rand << endl;
			shape = shapes[ rand ];
			beatsToAdd.push_back(shape);
		}
			
			//totalbeatItemsNb++;
		//cout << "beatEffectMutex unLock tmpo" << endl;
	}
}

/*void imageBeatEffect::onSetEventListener(mirOnSetEventArgs &_args){
	ofScopedLock lock(effectMutex);
	cout << "beatEffectMutexLock onset" << endl;
	if(shapes.size()<=0) return;
	
	if(_args.source.compare("aubioOnSet")==0) for(int i=0; i<round(mirReceiver::mirCache.zcr*130); i++){
		beatItems.push_back( getRandomBeatItem() );
	}
	
	//else if(_args.source.compare("")==0){}
	
}*/
	
// END KM_PLEASESKIPME
#endif