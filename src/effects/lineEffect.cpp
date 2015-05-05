//
//  lineEffect.cpp
//  karmaMapper
//
//  Created by Daan de Lange on 12/3/14.
//
//  - - - -
//
//  Parent class for all effects.
//  Implements some standard methods for overall usage.
//

#include "lineEffect.h"

lineEffect::lineEffect(){
	basicEffect::basicEffect();
	
	// effectType must match the class name
	effectType = "lineEffect";
	
	lines.clear();
	/*for(int i=0; i<shapes.size()*5; i++){
		
		lines.push_back( getRandomLine() );
	 }*/
	
	ofAddListener(ofx::AbletonLiveSet::EventHandler::noteEvent, this, &lineEffect::noteEventListener);

}

lineEffect::~lineEffect(){
	
	ofRemoveListener(ofx::AbletonLiveSet::EventHandler::noteEvent, this, &lineEffect::noteEventListener);
}

// update --> animation
// overrule this function with your own
bool lineEffect::render(){
	if( shapes.size()==0 ) return;
	
	for(std::list<lineEffectLine>::iterator it=lines.begin(); it!= lines.end(); ++it){
		(*it).render();
	}
}

void lineEffect::update(){
	// you can chose to do the default behaviour too or not by commenting this out
	basicEffect::update();
	
	// add lines ?
	if(lines.size() < shapes.size()*30) lines.push_back( getRandomLine() );
	
	// check for dead lines
	for(std::list<lineEffectLine>::reverse_iterator it=lines.rbegin(); it!= lines.rend(); it--){
		
		lineEffectLine bla = *it;
		
		// replace dead one with new one
		if( !(*it).isAlive() ) {
			//*it = getRandomLine();
			it++;
			it= std::list<lineEffectLine>::reverse_iterator( lines.erase( it.base() ));
			it--;
		}
	}
}

// resets all values
// overrule this function with your own.
void lineEffect::reset(){
	// do everything the basicEffect does
	basicEffect::reset();
	
	// do other stuff
}

/*bool lineEffect::grabSomeShapes(){
	// clear
	shapes.clear();
	shapes.resize(0);
	
	// we just want all vertex shapes
	//shapes = shapesHandler::getInstance().getShapesByType("vertexShape");
	
	// then sort them by group
	//map<int, int> groups; // <index, count>
	//groups.clear();
	shapeGroups.clear();
	for(int i=0; i<shapes.size(); i++){
		int group = shapes[i]->getGroupID();
		
		// create map entry ?
		if( shapeGroups.find(group) == shapeGroups.end() ){
			shapeGroups[group] = vector<int>();
		}

		shapeGroups[group].push_back(i);
	}
	
	
}*/

// returns a line from 1 shape vertex to another
lineEffectLine lineEffect::getRandomLine( const bool onSameShape){
	
	basicShape* fromShape = shapes[ round( ofRandom(-0.49f, shapes.size()-0.51f) )];
	basicShape* toShape;
	if( onSameShape ) toShape = fromShape;
	else toShape = shapes[ round( ofRandom(-0.49f, shapes.size()-0.51f) )];
	
	ofVec2f* from;
	ofVec2f* to;
	if( !fromShape->isReady() || !toShape->isReady() ){
		from = &basicShape::zeroPoint;
		to = &basicShape::zeroPoint;
	}
	else if( fromShape->isType("vertexShape") ){
		from = ((vertexShape*) fromShape)->getRandomVertexPtr();
		to = ((vertexShape*) toShape)->getRandomVertexPtr();
	}
	else {
		from = fromShape->getPositionPtr();
		to = toShape->getPositionPtr();
	}
	
	return lineEffectLine( from, to );
}

void lineEffect::noteEventListener(ofx::AbletonLiveSet::LSNoteEvent &noteEvent){
	//lines.push_back();
}

