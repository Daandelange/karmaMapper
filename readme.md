# karmaMapper
________

karmaMapper is an OpenSource mapping software made for real-time interactions and image generation. Built upon the amazing [OpenFrameworks](http://www.openframeworks.cc/) framework, it runs on Mac and should run on Windows and Linux.

__karmaMapper is a work in progress.__

![Project Cover](https://raw.githubusercontent.com/Karma-Kusala/karmaMapper/master/karmaMapper.png)

## Dependencies
#### Required:  
- [OpenFrameworks](http://www.openframeworks.cc/) with the following addons:
	- [ofxUI](https://github.com/rezaali/ofxUI)
	- OSC
	- [ofxAbletonLiveSet](https://github.com/satoruhiga/ofxAbletonLiveSet)	
	- etc.
- _Some effects may have other dependencies._

#### Optional:
- a RaspberryPi + Raspbian _(for real-time sound analysis and video streaming)_

## Details
It lets you configure a series of virtual 2D shapes on a 3D environment. This simplifies the whole image output generation process not needing complex transform matrices and so on. These shapes are then passed to an animator class which applies effects on them. Devices can be used for more interaction.

## Features
- User input: a webserver can collect images or messages or drawings from people's smart phone to be used within specific effects. (or a photomaton)
- User interaction: possibility to put a totem-like-object allowing people to interact with the visuals.
- Physical effect generators can be used. (ex: an independent rPi with webcam?)
- Automatic animation mode or manual with seamless switching.
- Real-time sound analysis streamed over local network using OSC.

_(most of these are not yet implemented)_

## Architecture
It's made for extensibility and there are several components:

- Vectorial shapes provide a virtual interface with a physial space. A shapes server serves them; there's also an editor to configure a space.
- An animator lets you control most settings manually or automatically.
- Effects are applyed to shapes by the animator component which can be controlled manually or automatically.

For more details, please refer to the software diagram below:  
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