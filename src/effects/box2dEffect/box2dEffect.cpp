//
//  box2dEffect.cpp
//  karmaMapper
//
//  Created by Daan de Lange on 12/05/2016.
//
//  - - - -
//
//  Physics simulation using box2d ! :D

#include "box2dEffect.h"

// - - - - - - -
// CONSTRUCTORS
// - - - - - - -

box2dEffect::box2dEffect(){
	
	effectType = "box2dEffect";
	box2dEffect::reset();
	//bFlagBox2dReset = false;
	box2dShapeItems.clear();
	particles.destroy();
	box2dEdges.clear();
	newBox2dItemsFromThread.empty();
	newBox2dParticlesFromThread.empty();
}

box2dEffect::~box2dEffect(){
	// unbind listeners
	ofRemoveListener( ofEvents().mousePressed, this, &box2dEffect::_mousePressed);
	ofRemoveListener(liveGrabberOSC::liveGrabberNoteEvent, this, &box2dEffect::liveGrabberNoteEventListener);
	
//	if(isThreadRunning()){
//		waitForThread(true);
//	}
	newBox2dItemsFromThread.close();
	newBox2dParticlesFromThread.close();
}

// - - - - - - -
// BASIC EFFECT FUNCTIONS
// - - - - - - -

// initialises the effect
bool box2dEffect::initialise(const animationParams& params){
	// init values
	shaderEffect::initialise(params);
	
	bIsLoading = true;
	bInitialised = false;
	
	// will trigger box2D reset. (done in thread)
	//bFlagBox2dReset = true;
	
//	if(isThreadRunning()){
//		waitForThread(true);
//	}

	box2dEffect::reset();
	
	return bInitialised;
}

bool box2dEffect::render(karmaFboLayer& renderLayer, const animationParams &params){
	//if( !videoShader::render(renderLayer, params) ) return false;
	
	if(!isReady()){
		return false;
	}
	
	renderLayer.begin();
	
	if(shader.isLoaded()){
		shader.begin();
	}
	
	ofPushStyle();
	ofFill();
	ofSetColor(mainColor[0], mainColor[1], mainColor[2], mainColor[3]);
	//ofSetColor(255,255,255,255);
	
	if(bDrawBox2dEdges){
		for(auto e=box2dEdges.begin(); e!=box2dEdges.end(); ++e){
			(*e)->draw();
		}
	}
	
//	ofFill();
//	ofSetColor(mainColor[0], mainColor[1], mainColor[2], mainColor[3]);
	for(int i=0; i<box2dShapeItems.size(); i++) {
		box2dShapeItems[i].get()->draw();
	}
	
	//ofEnableBlendMode(OF_BLENDMODE_ADD);
	//ofEnableAlphaBlending();
	//ofEnableBlendMode(OF_BLENDMODE_ADD);
	ofSetColor(mainColor[0], mainColor[1], mainColor[2], mainColor[3]);
	
	
	particles.draw();
	//ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	
	//ground.updateShape();
		
	ofPopStyle();
	
	if(shader.isLoaded()){
		shader.end();
	}
	
	renderLayer.end();
	
	return true;
}

// updates shape data
void box2dEffect::update(karmaFboLayer& renderLayer, const animationParams& params){
	
	if(!isReady()){
		return false;
	}
    
    basicEffect::update(renderLayer, params);
	
	// got particles to remove ?
	ofRemove(box2dShapeItems, ofxBox2dBaseShape::shouldRemoveOffScreen);
	
	// get new particles to add ?
	static box2dItemProperties newItem;
	bool gotNew = false;
	while(newBox2dItemsFromThread.tryReceive(newItem)){
		gotNew = true;
	}
	if(gotNew){
		// create particle
		shared_ptr<ofxBox2dCircle> p = shared_ptr<ofxBox2dCircle>(new ofxBox2dCircle);
		p.get()->setup(box2d.getWorld(), newItem.position, newItem.radius);
		p.get()->setPhysics(newItem.physics.x, newItem.physics.y, newItem.physics.z);
		p.get()->setVelocity(newItem.velocity);
		
		box2dShapeItems.push_back(p);
	}
	
	static box2dParticleProperties newParticle;
	gotNew = false;
	while(newBox2dParticlesFromThread.tryReceive(newParticle)){
		gotNew = true;
	}
	if(gotNew){
		// create particle
//		shared_ptr<ofxBox2dCircle> p = shared_ptr<ofxBox2dCircle>(new ofxBox2dCircle);
//		p.get()->setup(box2d.getWorld(), newParticle.position, newParticle.radius);
//		p.get()->setVelocity(newParticle.velocity);

		particles.setColor(newParticle.color);
		particles.createParticle(newParticle.position, newParticle.velocity);
	}
	
	// rebuild world ?
	if(bSyncBox2dOnUpdate){
		syncBox2dWithShapes();
		box2d.setBounds(ofPoint(0), ofPoint(renderLayer.getWidth(),renderLayer.getHeight()));
	}
	
	box2d.update();
	
	
	//box2d.update();
	
	// calc some stats
	box2dGlobalRotation = 0;
	box2dGlobalVelocity = ofVec2f(0);
	box2dAveragePosition = ofVec2f(0);
	if(box2dShapeItems.size()>0){
		
		for(auto p=box2dShapeItems.begin(); p!=box2dShapeItems.end(); ++p){
			box2dGlobalRotation += p->get()->getRotation();
			box2dGlobalVelocity += p->get()->getVelocity();
			box2dAveragePosition += p->get()->getPosition();
		}
		box2dGlobalRotation /= (float) box2dShapeItems.size();
		box2dGlobalVelocity /= ofVec2f((float)box2dShapeItems.size());
		box2dAveragePosition /= ofVec2f((float)box2dShapeItems.size());
		
		// send stats
		// if liveGrabberOSC::getInstance().isEnabled()
		if( ofGetFrameNum()%2==0){
			//static
			ofxOscMessage m;
			m.setAddress("box2dRotation");
			m.addFloatArg(box2dGlobalRotation/360.f);
			liveGrabberOSC::getInstance().sendOscMessage(m);
			
			m.clear();
			m.setAddress("box2dVelocityX");
			m.addFloatArg((float)(box2dGlobalVelocity.x/((float)renderLayer.getWidth()))/4.f);
			liveGrabberOSC::getInstance().sendOscMessage(m);
			m.clear();
			m.setAddress("box2dVelocityY");
			m.addFloatArg((float)(box2dGlobalVelocity.y/(float)renderLayer.getHeight())/4.f);
			liveGrabberOSC::getInstance().sendOscMessage(m);
			
//			m.setAddress("box2dSpeed");
//			m.addFloatArg(box2dGlobalVelocity.x/(float)renderLayer.getWidth()/4.f);
			
			m.clear();
			m.setAddress("box2dPositionX");
			m.addFloatArg((float)box2dAveragePosition.x/((float)renderLayer.getWidth()*0.5f));
			liveGrabberOSC::getInstance().sendOscMessage(m);
			m.clear();
			m.setAddress("box2dPositionY");
			m.addFloatArg((float)box2dAveragePosition.y/((float)renderLayer.getHeight()*0.5f));
			liveGrabberOSC::getInstance().sendOscMessage(m);
			
			m.clear();
			m.setAddress("box2dNumParticles");
			m.addFloatArg((float)box2dShapeItems.size());
			liveGrabberOSC::getInstance().sendOscMessage(m);
		}
	}
	
	// call the shaderEffect update
	//shaderEffect::update( renderLayer, params );
	
	//for(int p=0; p<particles.getParticleCount(); ++p){
		//particles.particleSystem->GetParticleGroupCount();
	//if(particles.getParticleCount()>0){
		//cout << particles.particleSystem->GetParticleGroupCount() << endl;
		//particles.particleSystem->GetDensity()
		//cout << particles.particleSystem->GetContactCount() <<  endl;
		
		
	if( ofGetFrameNum()%2==0){
		
		//static
		ofxOscMessage m;
		
        static float maxParticles = 10.f;
        float numParticles = particles.getParticleCount();
        maxParticles *= 0.99;
        if(maxParticles < numParticles ) {
            maxParticles = numParticles;
        }
		m.setAddress("box2dParticlesNum");
        m.addFloatArg( numParticles / maxParticles );
		liveGrabberOSC::getInstance().sendOscMessage(m);
		
		box2dParticlesCollisionEnergy *= 0.8;
		box2dParticlesCollisionEnergy += (particles.particleSystem->ComputeCollisionEnergy()/10.f) * 0.2f;
		box2dParticlesCollisionEnergy = ofClamp(box2dParticlesCollisionEnergy, 0.f,1.f);
		m.clear();
		m.setAddress("box2dParticlesCollisionEnergy");
		m.addFloatArg( box2dParticlesCollisionEnergy );
		liveGrabberOSC::getInstance().sendOscMessage(m);
		
		m.clear();
		m.setAddress("box2dParticlesGroupedPercentage");
		m.addFloatArg( (float) particles.particleSystem->GetContactCount() / (float) particles.getParticleCount() / 10.f );
		liveGrabberOSC::getInstance().sendOscMessage(m);
		
		box2dParticlesAveragePosition = ofVec2f(0);
		box2dParticlesAverageVelocity = ofVec2f(0);
		box2dParticlesSpeed = 0;
		if(particles.getParticleCount()>0){
			b2Vec2* posIt = particles.particleSystem->GetPositionBuffer();
			b2Vec2* velIt = particles.particleSystem->GetVelocityBuffer();
			for(int i=0; i<particles.getParticleCount(); ++i){
				box2dParticlesAveragePosition += ofVec2f(posIt[i].x, posIt[i].y);
				box2dParticlesAverageVelocity += ofVec2f(velIt[i].x, velIt[i].y);
			}
			
			box2dParticlesAverageVelocity /= ofVec2f(particles.getParticleCount());
			box2dParticlesAverageVelocity = ofVec2f(0.5f)+(box2dParticlesAverageVelocity/ofVec2f(5.f));
			
			box2dParticlesAveragePosition /= ofVec2f(particles.getParticleCount());
			box2dParticlesAveragePosition /= ofVec2f(renderLayer.getWidth()/particles.particleSize, renderLayer.getHeight()/particles.particleSize );
			
			box2dParticlesSpeed = abs(box2dParticlesAveragePosition.distance(ofVec2f(0)));
		}
		
		m.clear();
		m.setAddress("box2dParticlesAveragePositionX");
		m.addFloatArg( box2dParticlesAveragePosition.x );
		liveGrabberOSC::getInstance().sendOscMessage(m);
		m.clear();
		m.setAddress("box2dParticlesAveragePositionY");
		m.addFloatArg( box2dParticlesAveragePosition.y );
		liveGrabberOSC::getInstance().sendOscMessage(m);
		
		m.clear();
		m.setAddress("box2dParticlesAverageVelocityX");
		m.addFloatArg( box2dParticlesAverageVelocity.x );
		liveGrabberOSC::getInstance().sendOscMessage(m);
		
		m.clear();
		m.setAddress("box2dParticlesAverageVelocityY");
		m.addFloatArg( box2dParticlesAverageVelocity.y );
		liveGrabberOSC::getInstance().sendOscMessage(m);
		
		m.clear();
		m.setAddress("box2dParticlesSpeed");
		m.addFloatArg( box2dParticlesSpeed );
		liveGrabberOSC::getInstance().sendOscMessage(m);
	}
		
		//UpdatePairsAndTriadsWithReactiveParticles
	//}
	
	// delete particles for quicker removal ?
	if(bDisableMeSoon && particles.getParticleCount()>0){
		int random = 1+ofRandom( ((float)particles.getParticleCount())*0.05f );
		for(int i=0; i<random; i++){
			particles.particleSystem->DestroyOldestParticle(i, true);
		}
		
		box2dShapeItems.erase( box2dShapeItems.begin() );
	}
}

// resets all values
void box2dEffect::reset(){
	shaderEffect::reset();
	
//	if(isThreadRunning()){
//		waitForThread(true);
//	}
	
	bReactWithLiveGrabber = true;
	bAddParticlesWithMouse = false;
	bSyncBox2dOnUpdate = true;
	bDrawBox2dEdges = false;
	mouseAddAmount = 10;
	box2dParticleLifeSpan = 60.f;
	
	if( !loadShader(effectFolder("box2dShader.vert", "box2dEffect"), effectFolder("box2dShader.frag", "box2dEffect") ) ){
		setError(true, "Box2D shader (default shader) not loaded. (error)");
	}
	
	// effect type must match with class
	effectType = "box2dEffect";
	//bFlagBox2dReset = true; // asks for a threaded reset
	initBox2d();
	
	syncBox2dWithShapes();
	//box2d.registerGrabbing();
	
	// rebind listeners
	ofRemoveListener( ofEvents().mousePressed, this, &box2dEffect::_mousePressed);
	ofAddListener(ofEvents().mousePressed, this, &box2dEffect::_mousePressed);
	
	ofRemoveListener(liveGrabberOSC::liveGrabberNoteEvent, this, &box2dEffect::liveGrabberNoteEventListener);
	ofAddListener(liveGrabberOSC::liveGrabberNoteEvent, this, &box2dEffect::liveGrabberNoteEventListener);
	
//	if(!isThreadRunning()){
//		startThread();
//	}

	// set this when done
	bInitialised = true;
	bIsLoading = false;
	
}

// - - - - - - -
// GUI STUFF
// - - - - - - -
// When called, ImGui is already pushed into a Gui surface
// Just draw your gui items
bool box2dEffect::printCustomEffectGui(){
	
	if( ImGui::CollapsingHeader( GUIbox2dEffectPanel, "GUIbox2dEffectPanel", true, true ) ){
		
		ImGui::TextWrapped("Box2d physics simulation on shapes! :D");
		
		ImGui::Separator();
		ImGui::Separator();
		
		ImGui::Text("box2D Settings");
		ImVec2 canvas_pos = ImGui::GetCursorScreenPos();
		
		//ImGui::TextWrapped("Box 2D bounds:\t%s x %spx", box2D.getWorld()->);
		if(ImGui::DragFloat("Gravity Direction", &box2dWorldProperties.gravityAngleDeg)){
			
			syncBox2dWorldSettings();
		}
		if(ImGui::DragFloat("Gravity Force", &box2dWorldProperties.gravityIntensity, 0.05)){
			syncBox2dWorldSettings();
		}
		if(ImGui::SliderInt("Box2d Target FPS", &box2dWorldProperties.fps, 0, 60)){
			syncBox2dWorldSettings();
		}
		if(ImGui::SliderInt("Box2d Velocty Iterations", &box2dWorldProperties.iterationsVelocityTimes, 0, 50)){
			syncBox2dWorldSettings();
		}
		if(ImGui::SliderInt("Box2d Position Iterations", &box2dWorldProperties.iterationsPositionTimes, 0, 10)){
			syncBox2dWorldSettings();
		}
		
		// draw direction
		ImDrawList* draw_list = ImGui::GetWindowDrawList();
		ImVec2 canvas_size(ImGui::GetItemRectSize().y, ImGui::GetItemRectSize().y);// = ImGui::GetContentRegionAvail();
		draw_list->AddRect(canvas_pos, ImVec2(canvas_pos.x + canvas_size.x, canvas_pos.y + canvas_size.y), ImColor(255,255,255));
		ImVec2 rotOffset( cos((box2dWorldProperties.gravityAngleDeg/360.f)*TWO_PI)*(canvas_size.x/2.f), sin((box2dWorldProperties.gravityAngleDeg/360.f)*TWO_PI)*(canvas_size.y/2.f));
		
		draw_list->AddLine(
						   ImVec2(canvas_pos.x + canvas_size.x/2.f, canvas_pos.y + canvas_size.y/2.f),
						   ImVec2(
								  canvas_pos.x + canvas_size.x/2.f + rotOffset.x,
								  canvas_pos.y + canvas_size.y/2.f + rotOffset.y
							), 0xFFFFFFFF, 2.0f);
		
		ImGui::Separator();
		ImGui::Separator();
		
		if(ImGui::Checkbox("React to LiveGrabber", &bReactWithLiveGrabber)){
			// todo: (un)bind listeners
		}
		if(ImGui::Checkbox("Add particles with mouse", &bAddParticlesWithMouse)){
			// todo: (un)bind mouse listeners
		}
		ImGui::DragInt("Mouse add amount", &mouseAddAmount, 1, 1, 300);
		if(ImGui::DragFloat("Particles life span", &box2dParticleLifeSpan, 0.5f, 1.f, 300.f)){
			particles.setParticleLifetime(box2dParticleLifeSpan);
		}
		
		if(ImGui::Checkbox("Show Box2D world", &bDrawBox2dEdges)){
			
		}
		if(ImGui::Checkbox("Update Box2DWorld with shapes on update", &bSyncBox2dOnUpdate)){
			
		}
		if(ImGui::Button("ReSync with Shapes")){
			syncBox2dWithShapes();
		}
		
		if(ImGui::Button("Reset Box2D###reset_particles")){
			box2dShapeItems.erase( box2dShapeItems.begin(), box2dShapeItems.end() );
			
			for(int i=0; i<particles.getParticleCount(); i++){
				particles.particleSystem->DestroyOldestParticle(i, true);
			}
			
		}
		
		static int rmNb(10);
		if(ImGui::Button("Remove some particles")){
			
			for(int i=0; i<particles.getParticleCount() && i<rmNb; i++){
				particles.particleSystem->DestroyOldestParticle(i, true);
			}
		}
		ImGui::SameLine();
		ImGui::DragInt("Remove amount", &rmNb, 1, 1, 300);
		
		ImGui::Separator();
		ImGui::Separator();
		
		ImGui::Text("Box 2D Stats:");
		ImGui::Text("Num Particles:\t%d", particles.getParticleCount());
		ImGui::Text("Num Box2D Shapes:\t%lu", box2dShapeItems.size());
		
		ImGui::Text("Average Rotation:\t%f", box2dGlobalRotation);
		ImGui::Text("Average Velocity:\t%f\t%f", box2dGlobalVelocity.x, box2dGlobalVelocity.y);
		ImGui::Text("Average Position:\t%f\t%f", box2dAveragePosition.x, box2dAveragePosition.y);
	}
	
	shaderEffect::printCustomEffectGui();
	
	return true;
}

// - - - - - - -
// LOAD & SAVE FUNCTIONS
// - - - - - - -

// writes the effect data to XML. xml's cursor is already pushed into the right <effect> tag.
bool box2dEffect::saveToXML(ofxXmlSettings& xml) const{
	bool ret = shaderEffect::saveToXML(xml);
	
	xml.addValue("bReactWithLiveGrabber", bReactWithLiveGrabber);
	xml.addValue("bAddParticlesWithMouse", bAddParticlesWithMouse);
	xml.addValue("mouseAddAmount", mouseAddAmount);
	xml.addValue("bSyncBox2dOnUpdate", bSyncBox2dOnUpdate);
	xml.addValue("bDrawBox2dEdges", bDrawBox2dEdges);
	
	xml.addValue("box2dFPS", box2dWorldProperties.fps);
	xml.addValue("box2dGravityAngle", box2dWorldProperties.gravityAngleDeg);
	xml.addValue("box2dGravityIntensity", box2dWorldProperties.gravityIntensity);
	xml.addValue("box2dIterationsVelocityTimes", box2dWorldProperties.iterationsVelocityTimes);
	xml.addValue("box2dIterationsPositionTimes", box2dWorldProperties.iterationsPositionTimes);
	xml.addValue("box2dParticlesLifeSpan", box2dWorldProperties.particlesLifeSpan);
	
	return ret;
}

// load effect settings from xml
// xml's cursor is pushed to the root of the <effect> tag to load
bool box2dEffect::loadFromXML(ofxXmlSettings& xml, const shapesDB& _scene){
	bool ret = shaderEffect::loadFromXML(xml, _scene);

	bReactWithLiveGrabber = xml.getValue("bReactWithLiveGrabber", bReactWithLiveGrabber);
	bAddParticlesWithMouse = xml.getValue("bAddParticlesWithMouse", bAddParticlesWithMouse);
	mouseAddAmount = xml.getValue("mouseAddAmount", mouseAddAmount);
	bSyncBox2dOnUpdate = xml.getValue("bSyncBox2dOnUpdate", bSyncBox2dOnUpdate);
	bDrawBox2dEdges = xml.getValue("bDrawBox2dEdges", bDrawBox2dEdges);
	
	box2dWorldProperties.fps = xml.getValue("box2dFPS", box2dWorldProperties.fps);
	box2dWorldProperties.gravityAngleDeg = xml.getValue("box2dGravityAngle", box2dWorldProperties.gravityAngleDeg);
	box2dWorldProperties.gravityIntensity = xml.getValue("box2dGravityIntensity", box2dWorldProperties.gravityIntensity);
	box2dWorldProperties.iterationsVelocityTimes = xml.getValue("box2dIterationsVelocityTimes", box2dWorldProperties.iterationsVelocityTimes);
	box2dWorldProperties.iterationsPositionTimes =
	box2dWorldProperties.iterationsPositionTimes = xml.getValue("box2dIterationsPositionTimes", box2dWorldProperties.iterationsPositionTimes);
	box2dWorldProperties.particlesLifeSpan = xml.getValue("box2dParticlesLifeSpan", box2dWorldProperties.particlesLifeSpan);
	
	initBox2d();
	//syncBox2dWorldSettings();
	
	return ret;
}

// - - - - - - -
// CONTROLLER FUNCTIONS
// - - - - - - -

// return false if effect should remain a little before deletion
bool box2dEffect::disableSoonIsNow() {
    //cout << (particles.getParticleCount() >= 0) << endl;
	return (particles.getParticleCount() <= 0);
}

bool box2dEffect::randomizePresets(){
	if(!shaderEffect::randomizePresets() ) return false;
	
	// do your stuff here
	
	return true;
}

// - - - - - - -
// box2dEffect FUNCTIONS
// - - - - - - -

void box2dEffect::initBox2d(){
	box2dShapeItems.empty();
	box2dShapeItems.clear();
	box2dEdges.clear();
	//newBox2dItemsFromThread.close();
	
	box2d = ofxBox2d();
	box2d.init();
	box2d.createBounds();
	//
	syncBox2dWorldSettings();
	syncBox2dWithShapes();
	
	ofColor color;
	color.set(255);
	//particles.destroy();
	particles = ofxBox2dParticleSystem();
	particles.setParticleFlag(b2_tensileParticle);
	particles.loadImage(effectFolder("particle32.png", "box2dEffect"));
	particles.setup(box2d.getWorld(), 20000, 60.0, 8.0, 32.0, color);
	
	//newBox2dItemsFromThread = ofThreadChannel< box2dItemProperties >();
}

void box2dEffect::syncBox2dWorldSettings(){
	
	// sync gravity
	box2dWorldProperties.gravityAngleDeg = fmod(box2dWorldProperties.gravityAngleDeg, 360.f);
	
	box2d.setGravity(cos((box2dWorldProperties.gravityAngleDeg/360.f)*TWO_PI)*box2dWorldProperties.gravityIntensity, sin((box2dWorldProperties.gravityAngleDeg/360.f)*TWO_PI)*box2dWorldProperties.gravityIntensity);
	
	box2d.setFPS(box2dWorldProperties.fps);
}

// updates particles
//void box2dEffect::threadedFunction(){
//
//#ifdef KARMAMAPPER_DEBUG
//	//ofThread::setThreadName("box2dEffect::threadedFunction() " + ofToString(ofThread::getThreadId()) );
//#endif
//	
//	while (isThreadRunning()) {
//		
//		// re-init ?
//		if(bFlagBox2dReset){
//			box2d.setGravity(0, 0.5f);
//			box2d.createBounds();
//			box2d.setFPS(60.0);
//		}
//		
//		// get new particles to add ?
//		static shared_ptr<ofxBox2dCircle> newParticle;
//		if(newparticlesForThread.tryReceive(newParticle), 16){
//			box2dShapeItems.push_back(newParticle);
//		}
//		
//		box2d.update();
//		
//		sleep(16); // limits CPU usage to ±60fps
//	}
//	stopThread();
//}


//
void box2dEffect::syncBox2dWithShapes(){
	
	box2dEdges.clear();
	for(auto s=shapes.begin(); s!=shapes.end(); ++s){
		if((*s)->isType("vertexShape")){
			vertexShape* vS = static_cast<vertexShape*>(*s);
			
			// only take shapes that have 3 or more points
			if(vS->getPoints().size()>2){
				vector<ofVec2f> shapePoints;
				shapePoints.clear();
				basicPoint* shapeCenter = vS->getPositionPtr();
				std::shared_ptr<ofxBox2dEdge> edge(new ofxBox2dEdge);
				edge.get()->clear();
				for(auto pt=vS->getPoints().begin(); pt!=vS->getPoints().end(); ++pt){
					shapePoints.push_back( *shapeCenter + (*pt) );
				}
				
				// close
				shapePoints.push_back( *shapeCenter + (*vS->getPoints().begin()) );
				
				edge.get()->addVertexes( shapePoints );
				edge.get()->close();
				
				edge.get()->create(box2d.getWorld());
				box2dEdges.push_back(edge);
			}
		}
	}
}

// - - - - - - -
// box2dEffect LISTENERS
// - - - - - - -
void box2dEffect::_mousePressed(ofMouseEventArgs &args){
	
	if(bDisableMeSoon) return;
	
	// let UI interactions happen normally
	if(bAddParticlesWithMouse && !ImGui::IsMouseHoveringAnyWindow()){
		
		if(args.button==0){
			for(int i=0; i<mouseAddAmount; ++i){
				
				float radius = ofRandom(60, 80);
				float x = cos(ofRandom(PI * 2.0)) * radius + args.x;
				float y = sin(ofRandom(PI * 2.0)) * radius + args.y;
				ofVec2f position = ofVec2f(x, y);
				ofVec2f velocity = ofVec2f(0, 0);
				ofColor color;
				int hue = int(ofGetFrameNum() / 4.0) % 255;
				color.setHsb(hue, 180, 200);
				particles.setColor(color);
				particles.createParticle(position, velocity);
			}
		
			// notify livegrabber
			if(bReactWithLiveGrabber){
				// todo
				//"box2d/newParticle"
			}
		}
		else if(args.button==2){
			shared_ptr<ofxBox2dCircle> p = shared_ptr<ofxBox2dCircle>(new ofxBox2dCircle);
			p.get()->setPhysics(1.0, 0.5, 0.3);
			p.get()->setup(box2d.getWorld(), args.x, args.y, ofRandom(5, 20));
			p.get()->setVelocity(ofRandom(-30, 30), ofRandom(-30, 30));
			//p.get()->setupTheCustomData();
			box2dShapeItems.push_back(p);
			//newparticlesForThread.send(std::move(p));
		}
	}
}

// note: this function is threaded!
void box2dEffect::liveGrabberNoteEventListener(liveGrabberNoteEventArgs &_args){
	//ofScopedLock lock(effectMutex);
	
	if(!bReactWithLiveGrabber || bDisableMeSoon) return;
	
	if(_args.key.length()<2){
		return;
	}
	int numericKey = ofToInt(_args.key.substr(1));
	
	box2dParticleProperties settings;
	settings.color = ofColor( numericKey, ((numericKey+255/3))%255, ((numericKey+(255/3)*2))%255);
	settings.position = ofVec2f(ofRandom(ofGetWidth()), ofRandom(ofGetHeight()));
	settings.velocity = ofVec2f(ofRandom(-30, 30), ofRandom(-30, 30));
	
	newBox2dParticlesFromThread.send(std::move(settings));
}

// register effect type
EFFECT_REGISTER( box2dEffect , "box2dEffect" );
