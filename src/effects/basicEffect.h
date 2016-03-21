//
//  basicEffect.h
//  karmaMapper
//
//  Created by Daan de Lange on 12/3/14.
//
//
//

#pragma once

#include "ofMain.h"
#include "basicShape.h"
#include "effectFactory.h"
#include "ofxImGui.h"
#include "shapesDB.h"
#include "karmaFboLayer.h"
//#include "shapesServer.h"

namespace karmaThreadsSharedMemory {
	
}

// Basic parent class for effects

// todo: make a better hasError collection + notification system

// todo: rename reset() / randomize() / setDefaults() so it's more obvious what they do. Other functions might need renaming too.

// todo : add statistics to effects. Track their rendering time so we have an overview of their performance / load.

// todo: do some serious multithreading handling and check source for race conditions. Eg use Valgrind / Helpgrind. Maybe by namespacing threads and shared areas of code could help to prevent making mistakes leading to race conditions.

// Orgnise all functions of class in a way that you CAN do whatever, but if you don't, it must stay optimized for performence. Encourage interaction with other classes.

// (specific topic)(questionnings)
// todo: constructor(){ classTypes[]="MyClass"] }
// Examples: loopEffect (animates seamlessly from 0.0f to 1.0f and has a loopCount)
//			others animate over time (with lifeTime, etc. )
//			others animate on their own ? (ex: react to music, etc
//				(can there be any other effect specificities?
//				Maybe animator->render(GlobalAnimationStateParams)
//				with information on loop, elapsedTime, seasons?. (They can query music analysis and other data on their own)
// 				Plugins? --> MusicAnalyser, IO Connection, etc.
//			effects with a physical I/O connection
//			effects conneted to a local web server for interaction ? (ex: use user-generated drawings/content)
//			handle one shape or several shapes ? --> if only 1, automatically repeat for all shapes
//			animation preferences ?
//			#shapeAssociation[nb,size,aspect_ratio,etc]
//			#transitionPreferences[in/out]
//			Should an effect also be able to query available shapes? To ask to be moved to another shape? (This shouldn't be forced by the animator) Can an effect ask to be replaced (progressively?) and die? (ex: create organic lifeforms, create interactive games...), Ask for more effects of a certain type and set their initial params? can they ask not to be destroyed yet? etc? Temporarily forbid the use of certain effects on certain shape types ? Etc.
//			Effects preferences ?
//			transitions? make a transition system with custom parenting ? Effects handle their own transitions ?
//				Make effects "connect" protocol to them to each other. They can chose to let other appear/die and notify events to eachother. Two connected effects make a transition
//			How does everything remain connected with eachother ? Maybe 1 main animation class can control the main things and effects can freely move on shapes.
// maybe try { cast class }catch(){ignore}
// maybe compiler can handle this ?
// or force effects to handle more animation modes ? (but can just do nothing)
// how do the effect and the animator interact with each other ?
// the animator calls an effect and handles it's animation state
// the animator can also provide a color palette for a more uniform result.
// how could an effect ask another effects not to erase the bg every frame?
// idea: if an effect has not yet been bound with a shape on initialise(), let it request shapes. Otherwise use given ones.


// todo: isLoading should be isLoaded (or maybe another name) (also for hasError, initialised, etc.. they are unclear.
// todo: shapeType should be a vector so it can contain all shape types it is

//#define KM_NULL_SHAPE(NULL)

// forward declaration
struct animationParams;

class basicEffect {
	
public:
	// constructors
	basicEffect();
	virtual ~basicEffect();
	
	// global effect functions
	virtual bool initialise(const animationParams& params);
	virtual bool initialise();
	virtual bool render(karmaFboLayer& renderLayer, const animationParams& params);
	virtual void update(karmaFboLayer& renderLayer, const animationParams& params);
	//virtual void update();
	virtual void reset();
	void enable();
	void disable();
	
	// #########
	// GUI stuff
	void setShowGuiWindow(const bool& _b);
	bool showGuiWindow(const shapesDB& _scene);
	void toggleGuiWindow();
        virtual bool printCustomEffectGui(){return true;};
	
	
	// #########
	// LOAD & SAVE FUNCTIONS
	virtual bool saveToXML(ofxXmlSettings& xml ) const;
	virtual bool loadFromXML(ofxXmlSettings& xml);
	
	// effect properties
	bool isReady() const;
	bool isLoading() const;
	string getName() const;
	bool isType(const string _type) const;
	string getType() const;
	virtual const string getShortStatus()const;
	bool setIndex(const int& _index);
	const int& getIndex() const;
	
	// controller functions
	virtual bool randomizePresets();
	//virtual void transitionIn();
	//virtual void transitionOut();
	//void updateTransitionState();
	string effectFolder(const string _file, const string _effectFolder="") const;
	bool usesPingPong() const;
	virtual bool setUsePingPong(const bool& _usePingpong);
	
	//void setShader(ofShader& _shader);
	void updateBoundingBox();
	
	// shape binding tools
	bool bindWithShape(basicShape* _shape);
	bool bindWithShapes(vector<basicShape*> _shapes);
	bool bindWithShapes(list<basicShape*>& _shapes);
	bool isBoundWithShape(basicShape* _shape) const;
	/*virtual void bindWithShapes(vector<basicShape*>& _shapesRef);*/
	bool detachFromAllShapes();
	bool detachFromShape(basicShape* _shape);
	int getNumShapes() const;
	
	// todo
	// overallBoundingBox getter
	
	// todo: make this read-only
	unsigned int aliveSince;
	unsigned long long startTime; // to compare against ofGetSystemTime();
	
	static bool orderByIndex (const basicEffect* first, const basicEffect* second){
		return ( first->getIndex() < second->getIndex() );
	}
	
protected:
	string effectType;
	string effectUID;
	int effectIndex; // for ordering
	//effectParams params;
	
	bool bHasError;
	bool bEnabled;
	bool bInitialised;
	bool bIsLoading;
	bool bShowGuiWindow;
	bool bUsePingpong;
	string effectName; // must stay unique
	string shortStatus;
	
	float mainColor[4] {1.0,1.0,1.0,1.0};
	
	// todo:
	// animation preferences class
	
	vector<basicShape*> shapes;
	
	ofRectangle overallBoundingBox; // computes boundingbox containing all shapes
	//ofPlanePrimitive
	ofMutex effectMutex;
	
//private:
	// todo: implement something to make threads safer
	// maybe a class like this to hold any shared data ?
	// http://stackoverflow.com/a/13300836/58565
	// or rather use ofThreadChannel ?
};


namespace effect
{
	basicEffect* create(const std::string& name);
	void destroy(const basicEffect* comp);
	vector< std::string > getAllEffectTypes();
}



#define GUIBoundShapesTitle "Bound Shapes"

// allow shape registration
#define EFFECT_REGISTER(TYPE, NAME)                                        \
namespace effect {                                                         \
namespace factory {                                                       \
namespace                                                                 \
{                                                                         \
template<class T>                                                         \
class effectRegistration;                                                  \
\
template<>                                                                \
class effectRegistration<TYPE>                                             \
{                                                                         \
static const ::effect::factory::RegistryEntry<TYPE>& reg;                  \
};                                                                        \
\
const ::effect::factory::RegistryEntry<TYPE>&                              \
effectRegistration<TYPE>::reg =                                            \
::effect::factory	::RegistryEntry<TYPE>::Instance(NAME);                \
}}}
