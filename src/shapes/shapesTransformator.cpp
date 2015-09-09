//
//  shapesTransformator.cpp
//  karmaMapper
//
//  Created by Daan on 07/09/15.
//
//

#include "shapesTransformator.h"

// #############
// SHAPESTRANSFORMATOR
// - - - - - - -

shapesTransformator::shapesTransformator(list<basicShape*>& _shapes):
shapesSelection( _shapes ){
	centerHandle.x = 0;
	centerHandle.y = 0;
	container.set(0,0,0,0);
	
	containerHandles.clear();
	containerHandles.push_back( basicPoint(0,0) );
	containerHandles.push_back( basicPoint(0,0) );
	containerHandles.push_back( basicPoint(0,0) );
	containerHandles.push_back( basicPoint(0,0) );
}

shapesTransformator::~shapesTransformator(){
	// dont delete shapesSelection, it's only a reference to the real one
}

void shapesTransformator::setShapes(list<basicShape *>& _shapes){
	shapesSelection = _shapes;
}

void shapesTransformator::onShapeSelectionUpdated( list<basicShape *>& _shapes ){
	// Note: one really weird behaviour is that the reference to the list sometimes changes. This is why we pass a reference to the selection to make sure it's always up-to-date.
	// Another way of doing this could be to pass a reference to it's iterator which won't move ?
	// An alternative to std::list might be intrusive lists ?
	cout << "shapesSelection watcher::\t" << &shapesSelection << "\t" << &_shapes << endl;
	shapesSelection = _shapes;
	
	// todo: cancel currently happening transformations ?
	
	// set handlers on its corners
	if(shapesSelection.size()>1){
		
		calculateMultiShapesContainer();
		
		// set center
		centerHandle.setEditable(true);
		centerHandle.setPos(container.getCenter().x, container.getCenter().y );
		
		// update corner handles
		int i=0;
		for(auto it=containerHandles.begin(); it!=containerHandles.end(); it++){
			if(i==0) (*it).setPos( container.x, container.y);
			else if(i==1) (*it).setPos( container.x+container.width, container.y);
			else if(i==2) (*it).setPos( container.x+container.width, container.y+container.height);
			else if(i==3) (*it).setPos(container.x, container.y+container.height);
			(*it).setEditable(true);
			i++;
		}
	}
	
	else {
		centerHandle.setEditable(false);
		
		// make them editable
		for(auto it=containerHandles.begin(); it!=containerHandles.end(); it++){
			(*it).setEditable(false);
		}
	}
}

void shapesTransformator::drawHandles(){
	if( shapesSelection.size()<2 ) return;
	
	for(auto it=containerHandles.begin(); it!=containerHandles.end(); it++){
		(*it).draw();
	}
	centerHandle.draw();
}

void shapesTransformator::calculateMultiShapesContainer(){
	
	if(shapesSelection.size()==0){
		container.set(0,0,0,0);
		containerCached = container;
		multiShapeHandlers.clear();
		return;
	}
	
	// getRectFromShapes
	ofVec2f from(ofGetWidth(),ofGetHeight());
	ofVec2f to(0,0);
	for(auto it=shapesSelection.begin(); it!=shapesSelection.end(); it++){
		ofRectangle bb = (*it)->getBoundingBox();
		
		if(bb.x<from.x) from.x=bb.x;
		if(bb.y<from.y) from.y=bb.y;
		
		if(bb.x+bb.width > to.x) to.x=bb.x+bb.width;
		if(bb.y+bb.height > to.y) to.y=bb.y+bb.height;
	}
	container.set(from.x, from.y, to.x-from.x, to.y-from.y);
	
	// copy
	containerCached = container;
}

bool shapesTransformator::interceptMousePress(ofMouseEventArgs &e){
	// over centerHandle ?
	if( centerHandle.interceptMouseClick(e) ){
		
		return true;
	}
	
	// over container handles ?
	for(auto it=containerHandles.begin(); it!=containerHandles.end(); it++){
		if( (*it).interceptMouseClick(e) ) return true;
	}
	
	return false;
}


