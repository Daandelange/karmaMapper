karmaSoundAnalyser
==================

A Puredata real-time music analysis patch using Lib Aubio and OSC communication.  
_dependency_: Aubio pd plugin (v0.3.3)

![Preview](https://secure-b.vimeocdn.com/ts/461/411/461411483_960.jpg)  
[Click here to watch a demo video](https://vimeo.com/84516862)

Details
=======

It uses lib Aubio for onset and tempo detection.
I’ve had difficulties compiling the Pd plugin and I got it working using Pd-Extended compiled in 64 bit with Aubio 0.3.3 also in 64 bit.
Other features include an alternative onSet detection, pitch detection, silence detection and a 5-band equaliser.  

Note: not all collected data is yet set to be forwarded trough OSC.  

Note: Configure Pd to load aubio on launch or you'll have to open the pd patch twice.  

- - - -

Lib Aubio: http://aubio.org/  
Puredata: http://puredata.info/  
Made for a [Karma Kusala](http://karma-kusala.com/) project.  
Preview video: http://vimeo.com/84516862  

Installing (linux)
==================
`sudo apt-get install puredata pd-aubio pd-cyclone`  
add /usr/lib/pg/extra/cyclone to your pb library search paths (from pd preferences)  
Start Pd with `/usr/bin/pd -rt -oss -audiooutdev "2,3,4,5,6" -outchannels "2,2,2,2,2"`.  

Installing Jack on linux can also be useful.  
`sudo-apt-get install jackd qjackctl`  


