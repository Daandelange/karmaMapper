#!/bin/bash

# Dependencies for karmaMapper
# inspired by ObviousJim and ofxTimeline "clone_addons" script

cd ../../../addons/

if [ -z $1 ]; then
    PREFIX="git clone https://github.com/"
else
    PREFIX="git clone git@github.com:"
fi

#${PREFIX}satoruhiga/ofxAbletonLiveSet.git
${PREFIX}daandelange/ofxAbletonLiveSet.git

${PREFIX}Flightphase/ofxTextInputField.git
echo "If you're using linux, please make sure you checkout the develop branch of ofxTextInputField"

${PREFIX}frauzufall/ofxGuiExtended.git

${PREFIX}toruurakawa/ofxTextBox.git

${PREFIX}hironishihara/ofxTrueTypeFontUC.git

${PREFIX}runemadsen/ofxVLCRemote.git

${PREFIX}timscaffidi/ofxVideoRecorder.git -b 0.9.0

# will probably become: ${PREFIX}astellato/ofxSyphon.git -b OF-9.0
${PREFIX}asus4/ofxSyphon.git -b of_v0.9.0_OpenGL3

${PREFIX}Flightphase/ofxTextInputField.git -b develop
