# karmaMapper
________

karmaMapper is a collection of OpenSource 2D mapping software made for real-time interactions and digital image generation. Built upon the amazing [OpenFrameworks](http://www.openframeworks.cc/) framework, it gives an interface between the physical and the digital. It comes with an SDK allowing you to easyly start coding effects and modules then intstanciate and parametrize them within a same virtual space.
If you're at ease with code, it's a perfect tool to start coding on physical surfaces using a video projector.

Runs on Mac and Linux and Windows (alpha). 

__Update 11-12-2016__  
I have broken some parts intensely developing it for my exams. There have also been some openFrameworks updates leading to compilation issues and have had no time to maintain the Linux and Windows compilation.  
Over the next months I'm planing to restructure some code, and integrate a "shared variables ecosystem" with [VideoDromm](https://github.com/videodromm/). I'll also do some documentation and publish a binary to try it quickly.

__karmaMapper is a work in progress.__  
_Curently (18-06-2016), the latest build is stable on Mac while the Windows version is not stable yet. The Linux version should not be hard to get to compile.  
There's a scene editor allowing you to configure a set of "mappable" shapes, then load them within the animator app which lets you instantiate some effects and modules and bind them with these shapes.
There's a GUI overlay giving control over the animations and video can be output via Syphon for projection. Configurations can be saved and loaded. There's a music analyser that streams MIR data over a LAN network. There is also a module for doing Serial communication with Arduino and OSC communication with LiveGrabber, PureData and Duration. In other words, karmaMapper is ready to be used in interactive art installations & live performances, or to do simple video mapping! :)_

_In the future, karmaMapper will aim to facilitate the work of creative coders with a bunch of redundant code. It will also provide automatic variable assignment and animation to create a vast interconnected virtual and analog environment. Some of the code might be shared with videoDromm including a distant control interface._

![Screenshot](https://raw.githubusercontent.com/Karma-Kusala/karmaMapper/master/karmaMapper-cover-GIF.gif)

## SETUP
#### Install
- [Install OpenFrameworks](http://openframeworks.cc/setup/)  
- Clone the karmaMapper project:  

	```bash
	cd /path/to/openFrmeworks/apps/  
	mkdir karmaApps && cd ./karmaApps  
	git clone https://github.com/Karma-Kusala/karmaMapper.git --recursive   
	```
- Install dependencies: `cd ./karmaMapper && ./clone_addons.sh`  
_Note: On Windows, run the commands in the git command prompt._

#### Compile
_(Choose one)_  

- **Makefiles**: ( _/!\ Not tested with current build_ )  
`cd OF/apps/karmaApps/karmaMapper`  
`make Release`  
`make RunRelease`   
Select the animator or the editor in `config.make`  
	- `PROJECT_DEFINES = KM_EDITOR_APP`
	- `PROJECT_DEFINES = KM_ANIMATOR_APP`  
	
 _Note: Mac only. You may have to rename the `.mm`/`.m` file extensions in the addons' `ofxUVC/src/` to `.cpp`_
  
- **Qt Creator**: ( _/!\ Not tested with current build_ )  
Open `karmaMapper.qbs` and compile.  
See also the [OF Qt setup guide](http://openframeworks.cc/setup/qtcreator/).  
On Linux I also had to [update gcc to version 4.9](http://askubuntu.com/questions/428198/getting-installing-gcc-g-4-9-on-ubuntu).  
To select a **target**, edit `karmaMapper.qbs` and look for `of.defines: ['KM_ANIMATOR_APP']`  

- **Xcode**:  
Open `karmaMapper.xcodeproj` and select either the `karmaMapper Editor` or `karmaMapper Animator` target.  

	[__osx 10.12+ only__] : [Apple has kicked out the QTKit framework from 10.12+](https://forum.openframeworks.cc/t/macos-10-12-sierra-xcode-8-quicktime-deprecation-workaround/24179), needed by karmaMapper. For now, use `MacOSX10.9.sdk` (or 10.11).  
	- [Download an SDK](https://github.com/phracker/MacOSX-SDKs/releases/tag/MacOSX10.11.sdk)  
	- Install via symlink: (you can also copy the file into Xcode)  
`sudo ln -s /path/to/MacOSX10.11.sdk /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs`  
	- Relaunch Xcode then select the right SDKs to use to build and link karmaMapper and OF on each target in your project settings.

- **Visual Studio**: ( _/!\ Not tested with current build_ )  
Open `karmaMapper.vcxproj` and compile. If not compiling, it may help to re-generate the project using OpenFramework's Project Generator.

#### Optional:
Some [extra standalone apps](https://github.com/Karma-Kusala/karmaMapper/tree/master/utilities/) are available on rpi and other platforms, they can connect to and interact with karmaMapper.

- **RaspberryPi + Raspbian**: ( _/!\ Not tested with current build_ )  
Various scripts will be available, so an Rpi will be useful to have around.
[Pd sound analyser](https://github.com/Karma-Kusala/karmaMapper/tree/master/utilities/karmaSoundAnalyser) - [Pd Chladni plate generator](https://github.com/Karma-Kusala/karmaMapper/tree/master/utilities/chladni-plate-generator)
Note: I also got the karmaMapper (renderer) to compile and run on a RaspberryPi2 using Raspbian. Not sure the current version still compiles.
- **Arduino**  
A [serial control interface](https://github.com/Karma-Kusala/karmaMapper/tree/master/src/modules/serialController_v1) is available to communicate with Arduinos over USB / Serial.

____

## People
#### Contributors
- [Daan de Lange](http://daandelange.com/) ([Karma Kusala](http://karma-kusala.com/) / [ADN](http://artdistorsionsnumeriques.com/))
- Kévin Pouget / [Vienin](https://soundcloud.com/vienin) ([Karma Kusala](http://karma-kusala.com/))  
- Bruce Lane ([VideoDromm](https://github.com/brucelane))  

#### Thanks
 - Ricardo Garcia
 - Douglas Edric Stanley ([Abstractmachine](http://www.abstractmachine.net/))
 - Gregory Pignot ([Servovalve](http://www.servovalve.org/))
 - [Guillaume Stagnaro](http://stagnaro.net/)
 - [Tetalab](http://tetalab.org/)
 - [Esaaix](http://ecole-art-aix.fr/)
 - Locust

##  
Feel free to comment, contibute, fork, pull, etc. :)
