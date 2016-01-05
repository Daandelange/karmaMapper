//
//  shapesDB.cpp
//  karmaMapper
//
//  Created by Daan de Lange on 28/08/2015.
//  Copyright (c) 2015 Daan de Lange. All rights reserved.
//

// A class that extends shapesScene with some serving utilities

#include "shapesDB.h"

// - - - - - - - -
// CONSTRUCTORS
// - - - - - - - -
shapesDB::shapesDB(){

#ifdef KM_LOG_INSTANCIATIONS
	cout << "shapesScene::shapesScene()" << endl;
#endif
}

shapesDB::~shapesDB(){

}

// - - - - - - - - -
// SHAPE SERVING FUNCTIONS
// - - - - - - - - -
basicShape* shapesDB::getRandomShape(){
	if(shapes.size()<1){
		ofLogError("shapesDB::getRandomShape()", "There are not yet any shapes to serve!");
		
		// todo; return an error shape ?
		return NULL;
	}
	
	int offset( round( ofRandomuf()-.49f*(shapes.size()-.011f) ) );
	list<basicShape*>::iterator it = shapes.begin();
	std::advance(it,offset);
	return *it;
	
}

vector<basicShape*> shapesDB::getRandomShapes(int _amount, bool _returnExactAmount ) {
	
	// select unique shapes when possible
	// todo: add a vector<int> remainingIndexes for optimisation
	vector<int> selected;
	vector<basicShape*> retShapes;
	selected.resize(_amount);
	int numAdded = 0;
	while(numAdded < _amount && numAdded < getNumShapes() ){
		int newSh = ofRandom( 0.49f+getNumShapes()-1 );
		if( std::find( selected.begin(), selected.end(), newSh)==selected.end() ){
			// found a new one
			list<basicShape*>::iterator it = shapes.begin();
			std::advance(it,newSh);
			retShapes.push_back(*it);
			selected[numAdded]=newSh;
			numAdded++;
			//cout << "Rand=" << newSh << "[/"<< getNumShapes() << "]" << endl;
		}
	}
	
	// if there are les shapes then the asked amount, fill them with random duplicates
	if(_returnExactAmount && numAdded<_amount){
		while( numAdded!=_amount){
			retShapes.push_back( getRandomShape() );
			numAdded++;
		}
	}
	
	return retShapes;
}

// todo: add int _limit restriction param (and randomize)
vector<basicShape*> shapesDB::getShapesByType(string _type){
	vector<basicShape*> ret;
	ret.clear();
	ret.resize(0);
	
	// loop trough shapes and return the wanted ones
	for(auto it = shapes.begin(); it != shapes.end(); it++){
		if((*it)->getShapeType() == _type ) ret.push_back((*it));
	}
	
	return ret;
}

map<string, vector<basicShape*> > shapesDB::getAllShapesByType() const {
	map<string, vector<basicShape*> > ret;
	for( auto it=shapes.begin(); it!=shapes.end(); ++it ){
		if( ret.find( (*it)->getShapeType() ) == ret.end() ){
			ret[ (*it)->getShapeType() ] = vector<basicShape*>();
		}
		ret[ (*it)->getShapeType() ].push_back(*it);
	}
	return ret;
}

map<string, vector<basicShape*>> shapesDB::getAllShapesByGroup() const {
	
}

basicShape* shapesDB::getRandomShapeByType(string _type){
	vector<basicShape*> ret = getShapesByType(_type);
	if(ret.size()<=0) return NULL;
	
	return ret[round(-.49f+ofRandomuf()*(ret.size()-0.011f))];
}

// todo: add int _limit restriction param (and randomize)
vector<basicShape*> shapesDB::getShapesByGroup(int _group){
	vector<basicShape*> ret;
	ret.clear();
	ret.resize(0);
	
	// loop trough shapes and return the wanted ones
	for(auto it = shapes.begin(); it != shapes.end(); it++){
		if((*it)->getGroupID() == _group ) ret.push_back((*it));
	}
	
	return ret;
}

basicShape* shapesDB::getRandomShapeByGroup(int _group){
	vector<basicShape*> ret = getShapesByGroup(_group);
	if(ret.size()<=0) return NULL;
	return ret[round(-.49f+(ofRandomuf()*(ret.size()-.011f)))];
}

