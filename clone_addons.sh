#!/bin/bash

# Clones dependencies for karmaMapper
# Inspired by ObviousJim and ofxTimeline "clone_addons.sh" script

cd ../../../addons/

if [ -z $1 ]; then
    PREFIX="git clone https://github.com/"
else
    PREFIX="git clone git@github.com:"
fi

#${PREFIX}satoruhiga/ofxAbletonLiveSet.git
${PREFIX}daandelange/ofxAbletonLiveSet.git

${PREFIX}Flightphase/ofxTextInputField.git
#${PREFIX}Flightphase/ofxTextInputField.git -b develop
echo "IMPORTANT WARNING: If you're using Linux, please make sure you checkout the develop branch of ofxTextInputField"

${PREFIX}frauzufall/ofxGuiExtended.git

${PREFIX}jvcleave/ofxImGui.git

#${PREFIX}toruurakawa/ofxTextBox.git

# Not needed anymore ?
#${PREFIX}hironishihara/ofxTrueTypeFontUC.git
#${PREFIX}ofxTrueTypeFontUC.git

${PREFIX}runemadsen/ofxVLCRemote.git

unamestr=`uname`
if [[ $OSTYPE == darwin* ]] || [[ "$unamestr" == 'Linux' ]]; then
	echo "Installing ofxVideoRecorder for OS X / Linux..."
	#${PREFIX}timscaffidi/ofxVideoRecorder.git -b 0.9.0
	${PREFIX}timscaffidi/ofxVideoRecorder.git
else
	# for windows::::
	echo "Installing ofxVideoRecorder for windows..."
	${PREFIX}DomAmato/ofxVideoRecorder.git
fi

# will probably become: ${PREFIX}astellato/ofxSyphon.git -b OF-9.0
# note: Only required on Mac
${PREFIX}asus4/ofxSyphon.git -b of_v0.9.0_OpenGL3

${PREFIX}obviousjim/ofxMSATimer.git

# note: Only required on Mac
${PREFIX}atduskgreg/ofxUVC.git

${PREFIX}tobiasebsen/ofxFps.git

echo "Done cloning addons! :)"
