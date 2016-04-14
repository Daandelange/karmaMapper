//
//  shapesDB.h
//  karmaMapper
//
//  Created by Daan de Lange on 28/08/2015.
//  Copyright (c) 2015 Daan de Lange. All rights reserved.
//

#pragma once

#include "ofMain.h"
#include "KMSettings.h"
#include "shapesScene.h"
#include "shapes.h"


class shapesDB : public shapesScene { // holds the whole shapes "scene"

public:
	shapesDB();
	~shapesDB();
	
	// shape serving functions
	list<basicShape*>& getShapesRef(){ return shapes; }
	list<basicShape*>::const_iterator getShapesItConstBegin() const {
		return shapes.cbegin();
	}
	list<basicShape*>::const_iterator getShapesItConstEnd() const {
		return shapes.cend();
	}
	basicShape* getRandomShape();
	vector<basicShape*> getRandomShapes(int _amount=1, bool _returnExactAmount = true );
	vector<basicShape*> getShapesByType(string _type);
	map<string, vector<basicShape*> > getAllShapesByType() const;
	map<int, vector<basicShape*>> getAllShapesByGroup() const;
	
	
	basicShape* getRandomShapeByType(string _type);
	vector<basicShape*> getShapesByGroup(int _group);
	basicShape* getRandomShapeByGroup(int _group);
	basicShape* getShapeByName( string _name ) const;
	
	
	
private:
	
	
};
