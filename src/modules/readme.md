# Components
This folder cntains all your aditional modules.  
Like the effects folder, all these have to be compiled / hardcoded in the main app while they are instanciated on demand.

----

The [OSC router module](https://github.com/Karma-Kusala/karmaMapper/tree/master/src/modules/oscRouter) facilitates handling and distributing OSC messages and is mainly needed by other modules.
It also lets you handle the OSC connections from code or GUI.


#### C++  --(OSC)--> Pd  

 - `karmaSoundAnalyser` is a pd patch that analyses sound streams and sends MIR data. [C++ code](https://github.com/Karma-Kusala/karmaMapper/tree/master/src/modules/mirOSC) - [Pd Patch](https://github.com/Karma-Kusala/karmaMapper/tree/master/utilities/karmaSoundAnalyser)  
 - `chladni-plate-generator` is a pd patch that generates an audio signal for obtaining chladni figures / symatics. [C++ code](https://github.com/Karma-Kusala/karmaMapper/tree/master/src/modules/chladniOSC) - [Pd patch]( https://github.com/Karma-Kusala/karmaMapper/tree/master/utilities/chladni-plate-generator)

#### C++ --(serial)--> Ardiuno  

- `karmaMapperSerial_v1` is an Arduino program that communicates with a c++ serial module used for controlling electrovalves, lights and other electric signals. Clone this one to start Arduino controlled projects.
[C++ code](https://github.com/Karma-Kusala/karmaMapper/tree/master/src/modules/serialController_v1) - [Ardiuno sketch ](https://github.com/Karma-Kusala/karmaMapper/tree/master/src/modules/serialController_v1/karmaMapperSerial_v1) - [Photo](https://www.flickr.com/photos/108268248@N06/28651597005/in/album-72157670932284742/)