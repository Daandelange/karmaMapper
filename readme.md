# karmaMapper
________

karmaMapper is a collection of OpenSource 2D mapping software made for real-time interactions and digital image generation. Built upon the amazing [OpenFrameworks](http://www.openframeworks.cc/) framework, it gives an interface between the physical and the virtual and some other useful features for facilitating generative visuals. If you're at ease with code, it's a perfect tool to start coding on physical surfaces using a video projector.

Runs on Mac and Linux and Windows (alpha). 

__karmaMapper is a work in progress.__  
_Curently (25-03-2016), the latest build is stable on Mac & Linux while the Windows version is not stable yet. There's a scene editor allowing you to configure a set of "mappable" shapes, then load them within the animator app and instantiate some effects and modules on them. There's a GUI overlay giving control over the animations and video can be outPut via Syphon for projection. Configurations can be saved and loaded. There's a music analyser that streams MIR data over the network and a module that lets you sync effects with LiveGrabber. In other words, karmaMapper is ready to be used in interactive installations & performances! :)_

![Old screenshot](https://raw.githubusercontent.com/Karma-Kusala/karmaMapper/master/karmaMapper.png)

## SETUP
#### Required Dependencies:  
- [OpenFrameworks 9.0](http://www.openframeworks.cc/) with the following addons:  
	- [ofxImUI](https://github.com/jvcleave/ofxImGui)
	- [ofxAbletonLiveSet](https://github.com/satoruhiga/ofxAbletonLiveSet)	
	- Etc. _(see `clone_addons.sh`)_
	- __Note__: _Some effects may have other dependencies._
 
#### Install
- [Install OpenFrameworks](http://openframeworks.cc/setup/) then:
- `cd OF/apps/karmaApps/`
- `git clone git@github.com:Karma-Kusala/karmaMapper.git`
- `cd ./karmaMapper`
- `./clone_addons.sh` _(Mac & Linux only)_

#### Compile
- **Makefiles**:  
`cd OF/apps/karmaApps/karmaMapper`  
`make Release`  
`make RunRelease`   
Select the animator or the editor in `config.make`  
	- `PROJECT_DEFINES = KM_EDITOR_APP`
	- `PROJECT_DEFINES = KM_ANIMATOR_APP`
  
- **Qt Creator**:  
Open `karmaMapper.qbs` and compile.  
See also the [OF Qt setup guide](http://openframeworks.cc/setup/qtcreator/).  
On Linux I also had to [update gcc to version 4.9](http://askubuntu.com/questions/428198/getting-installing-gcc-g-4-9-on-ubuntu).  
To select a **target**, edit `karmaMapper.qbs` and look for `of.defines: ['KM_ANIMATOR_APP']`  

- **Xcode**:  
Open `karmaMapper.xcodeproj` and select either the `karmaMapper Editor` or `karmaMapper Animator` target.

- **Visual Studio**:  
Open `karmaMapper.vcxproj` and compile. If not compiling, it may help to re-generate the project using OpenFramework's Project Generator.

#### Optional:
- a RaspberryPi + Raspbian _(for real-time sound analysis with Pd Vanilla)_
Note: I successfully compiled and ran the karmaMapper animator on a RaspberryPi2.

## Details
It lets you configure a series of virtual 2D shapes on a 3D environment. This simplifies the whole image output generation process not needing complex transform matrices and so on. These shapes are then passed to an animator class which applies effects on them. The idea is to build a big animator app for creative coders that works together with a bunch of side-apps and tools.  

## Architecture
It's made for extensibility and there are several components:

- Vectorial shapes provide a virtual interface with a physial space. A shapes server serves them; there's also an editor to configure a space.
- An animator lets you control most settings manually or automatically.
- Effects are applied to shapes by the animator component which can be controlled manually or automatically.


## People
#### Contributors
- [Daan de Lange](http://daandelange.com/) ([Karma Kusala](http://karma-kusala.com/) / [ADN](http://artdistorsionsnumeriques.com/))
- Kévin Pouget / [Vienin](https://soundcloud.com/vienin) ([Karma Kusala](http://karma-kusala.com/))  

#### Thanks
 - Ricardo Garcia
 - Douglas Edric Stanley ([Abstractmachine](http://www.abstractmachine.net/))
 - Gregory Pignot ([Servovalve](http://www.servovalve.org/))
 - [Guillaume Stagnaro](http://stagnaro.net/)
 - Julien Rancoeur / [Fx23](https://soundcloud.com/fx23) ([ADN](http://artdistorsionsnumeriques.com/))
 - [Tetalab](http://tetalab.org/)
 - [Esaaix](http://ecole-art-aix.fr/)
 - [François Parra](http://www.autotune.eu/)
 - [Bruce Lane // VideoDromm](https://github.com/brucelane)

##  
Feel free to comment, contibute, fork, pull, etc. :)