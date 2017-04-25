#!/bin/bash

# Clones dependencies for karmaMapper
# Inspired by ObviousJim and ofxTimeline "clone_addons.sh" script

cd ../../../addons/

if [ -z $1 ]; then
    PREFIX="git clone https://github.com/"
else
    PREFIX="git clone git@github.com:"
fi

# variable for testing OS
unamestr=`uname`

echo "karmaMapper"
echo " - - - - - "
echo "Cloning addons starting."


echo "Cloning ofxAbletonLiveSet..."
#${PREFIX}satoruhiga/ofxAbletonLiveSet.git
${PREFIX}daandelange/ofxAbletonLiveSet.git

if [[ "$unamestr" == 'Linux' ]]; then
	echo "Cloning ofxTextInputField for Linux..."
	echo "IMPORTANT WARNING: Please make sure you checkout the develop branch of ofxTextInputField"
	${PREFIX}Flightphase/ofxTextInputField.git -b develop
else
	${PREFIX}Flightphase/ofxTextInputField.git
fi

echo "Cloning ofxGuiExtended..."
${PREFIX}frauzufall/ofxGuiExtended.git

echo "Cloning ofxImGui..."
${PREFIX}jvcleave/ofxImGui.git

#${PREFIX}toruurakawa/ofxTextBox.git

# Not needed anymore ?
#${PREFIX}hironishihara/ofxTrueTypeFontUC.git
#${PREFIX}ofxTrueTypeFontUC.git

echo "Cloning ofxMSATimer..."
${PREFIX}runemadsen/ofxVLCRemote.git

if [[ $OSTYPE == darwin* ]] || [[ "$unamestr" == 'Linux' ]]; then
	echo "Installing ofxVideoRecorder for mac or linux"
	#${PREFIX}timscaffidi/ofxVideoRecorder.git -b 0.9.0
	${PREFIX}timscaffidi/ofxVideoRecorder.git
else
	# for windows::::
	echo "Installing ofxVideoRecorder for windows..."
	${PREFIX}DomAmato/ofxVideoRecorder.git
fi

# will probably become: ${PREFIX}astellato/ofxSyphon.git -b OF-9.0
echo "Cloning ofxSyphon... (only needed on osx, make sure to checkout the openGL3 branch)"
${PREFIX}daandelange/ofxSyphon.git -b openGL3

echo "Cloning ofxMSATimer..."
${PREFIX}obviousjim/ofxMSATimer.git

# note: Only required on Mac
echo "Cloning ofxUVC... (osx only)"
${PREFIX}atduskgreg/ofxUVC.git

echo "Cloning ofxFps..."
${PREFIX}tobiasebsen/ofxFps.git

echo "Cloning ofxDelaunay..."
${PREFIX}obviousjim/ofxDelaunay.git

echo "Cloning ofxTurboJpeg..."
${PREFIX}armadillu/ofxTurboJpeg.git
echo "IMPORTANT WARNING: To link with ofxTurboJpeg, you have to install turboJpeg. See ofxTurboJpeg's readme for more details."

echo "Cloning ofxFaceTracker..."
${PREFIX}kylemcdonald/ofxFaceTracker.git
# copy needed files to karmaMapper
cp -R ./ofxFaceTracker/libs/FaceTracker/model/ ../apps/karmaApps/karmaMapper/bin/data/model/
echo "IMPORTANT WARNING: Copied ofxFaceTracker model data files. See the addon's license for appropriate usage."

# face tracker dependency
echo "Cloning ofxCv... (ofxFaceTracker dependency)"
${PREFIX}kylemcdonald/ofxCv.git

echo "Cloning ofxSerial..."
${PREFIX}bakercp/ofxSerial.git
# dependency of ofxSerial
echo "Cloning ofxIO... (ofxSerial dependency)"
${PREFIX}bakercp/ofxIO.git

echo "Cloning ofxLiquidFun..."
${PREFIX}tado/ofxLiquidFun.git

echo "Done cloning addons! :)"
