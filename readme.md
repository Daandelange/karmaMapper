# karmaMapper
________

karmaMapper is an OpenSource 2D mapping software made for real-time interactions and image generation involving a lot of interconnected madness. Built upon the amazing [OpenFrameworks](http://www.openframeworks.cc/) framework, it runs on Mac and should run on Windows and Linux.

__karmaMapper is a work in progress.__  
_Curently (19-11-2015), there's a scene editor allowing you to configure a set of shapes, a music analyser that streams over the network and the very bare basics of the animator app allowing to instantiate the first effects on shapes._

![Project Cover](https://raw.githubusercontent.com/Karma-Kusala/karmaMapper/master/karmaMapper.png)

## Dependencies
#### Required:  
- [OpenFrameworks 9.0](http://www.openframeworks.cc/) with the following addons:  
	- [ofxGui](https://github.com/rezaali/ofxGui)
	- [ofxAbletonLiveSet](https://github.com/satoruhiga/ofxAbletonLiveSet)	
	- etc.
	- __Note__: _Some effects may have other dependencies._
 
#### Install the dependencies
- Run `./clone_addons.sh` _(Mac & Linux only)_



#### Optional:
- a RaspberryPi + Raspbian _(for real-time sound analysis and video streaming #notyet )_

## Details
It lets you configure a series of virtual 2D shapes on a 3D environment. This simplifies the whole image output generation process not needing complex transform matrices and so on. These shapes are then passed to an animator class which applies effects on them. The idea is to build a big animator app that works together with a bunch of side-apps and tools. Imagine connecting devices such as a camera, gamepads, a raspberryPi music analyser, a control interface on a tablet, etc.

_(most of these are not yet implemented)_

## Architecture
It's made for extensibility and there are several components:

- Vectorial shapes provide a virtual interface with a physial space. A shapes server serves them; there's also an editor to configure a space.
- An animator lets you control most settings manually or automatically.
- Effects are applied to shapes by the animator component which can be controlled manually or automatically.

For more details, please refer to the software diagram below:  
Note: (__Depreciated__)  
![Software Diagram](https://raw.githubusercontent.com/Karma-Kusala/karmaMapper/master/software-diagram.png)



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

##  
Feel free to comment, contibute, fork, pull, etc. :)