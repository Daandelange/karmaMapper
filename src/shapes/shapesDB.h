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
	basicShape* getRandomShape();
	vector<basicShape*> getRandomShapes(int _amount=1, bool _returnExactAmount = true );
	vector<basicShape*> getShapesByType(string _type);
	map<string, vector<basicShape*> > getAllShapesByType() const;
	map<string, vector<basicShape*>> getAllShapesByGroup() const;
	
	
	basicShape* getRandomShapeByType(string _type);
	vector<basicShape*> getShapesByGroup(int _group);
	basicShape* getRandomShapeByGroup(int _group);
	
	
	
private:
	
	
};
