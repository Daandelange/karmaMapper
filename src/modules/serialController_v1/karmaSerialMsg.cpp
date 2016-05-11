//
//  OSCNode.cpp
//  karmaMapper
//
//  Created by Daan de Lange on 30/05/2016.
//  Copyright (c) 2014 __MyCompanyName__. All rights reserved.
//

#include "OSCNode.h"


// - - - - - - - -
// CONSTRUCTORS
// - - - - - - - -
OSCNode::OSCNode( ) : nodeName("OSCNode") {
	nodeName = "OSCNode";
	//bLoaded = true;
}

OSCNode::~OSCNode(){
	
}

////
//bool OSCNode::isEnabled() const {
//	return bLoaded;
//}

string OSCNode::getName() const {
	return nodeName;
}

//void OSCNode::notifyDetached(){
//	
//}

// - - - - - - - -
// EVENT LISTENERS
/*/ - - - - - - - -
void OSCNode::update(ofEventArgs &event){
	if(!isEnabled()) return;
	
	// update GUI data
	if(guiNumRoutes!=NULL) guiNumRoutes->setLabel(GUIOSCNodeNumRoutes + ofToString(routes.size()) );
	
}*/
