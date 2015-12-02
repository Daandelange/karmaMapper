import qbs
import qbs.Process
import qbs.File
import qbs.FileInfo
import qbs.TextFile
import "../../../libs/openFrameworksCompiled/project/qtcreator/ofApp.qbs" as ofApp

Project{
    property string of_root: '../../..'

    ofApp {
        name: { return FileInfo.baseName(path) }

        files: [
            'src/main.cpp',
            'src/ofApp.cpp',
            'src/ofApp.h',
            'src/KMSettings.h',
            'src/ofAppEditor.cpp',
            'src/ofAppEditor.h',

            // CORE
            'src/core/animationController.cpp',
            'src/core/animationController.h',
            'src/core/animationParams.h',
            'src/core/animationParamsServer.cpp',
            'src/core/animationParamsServer.h',
            'src/core/durationEvents.h',
            'src/core/durationRC.cpp',
            'src/core/durationRC.h',
            'src/core/durationReceiver.cpp',
            'src/core/durationReceiver.h',
            'src/core/fboRecorder.cpp',
            'src/core/fboRecorder.h',
            'src/core/karmaSoundAnalyser.cpp',
            'src/core/karmaSoundAnalyser.h',
            'src/core/mirEvents.h',
            'src/core/mirReceiver.cpp',
            'src/core/mirReceiver.h',
            'src/core/OSCNode.cpp',
            'src/core/OSCNode.h',
            'src/core/OSCRouter.cpp',
            'src/core/OSCRouter.h',

            // EFFECTS
            'src/effects/effects.h',
            'src/effects/basicEffect.cpp',
            'src/effects/basicEffect.h',
            'src/effects/distortEffect.cpp',
            'src/effects/distortEffect.h',
            //'src/effects/emptyEffect.cpp',
            //'src/effects/emptyEffect.h',
            'src/effects/lineEffect/lineEffect.cpp',
            'src/effects/lineEffect/lineEffect.h',
            'src/effects/lineEffect/lineEffectLine.cpp',
            'src/effects/lineEffect/lineEffectLine.h',
            'src/effects/shaderEffect/shaderEffect.cpp',
            'src/effects/shaderEffect/shaderEffect.h',
           //'src/effects/musicEffect.cpp',
           //'src/effects/musicEffect.h',

           // SHAPES
            'src/shapes/shapeFactory.cpp',
            'src/shapes/shapeFactory.hpp',
            'src/shapes/shapes/basicPoint.cpp',
            'src/shapes/shapes/basicPoint.h',
            'src/shapes/shapes/basicShape.cpp',
            'src/shapes/shapes/basicShape.h',
            'src/shapes/shapes/shapes.h',
            'src/shapes/shapes/vertexShape.cpp',
            'src/shapes/shapes/vertexShape.h',
            'src/shapes/shapesDB.cpp',
            'src/shapes/shapesDB.h',
            'src/shapes/shapesEditor.cpp',
            'src/shapes/shapesEditor.h',
            'src/shapes/shapesScene.cpp',
            'src/shapes/shapesScene.h',
            'src/shapes/shapesTransformator.cpp',
            'src/shapes/shapesTransformator.h',
            'src/shapes/shapeUtils.h'
        ]

        // This project is using addons.make to include the addons
        // since it was imported from old code. To change it to include
        // the addons from the qbs file change the following lines to
        // the list of used addons in array format. eg:
        //
        of.addons: [
             'ofxGui',
             'ofxOpenCv',
             'ofxAbletonLiveSet',
             'ofxAssimpModelLoader',
             'ofxGuiExtended',
             'ofxOsc',
             'ofxTextBox',
             'ofxTrueTypeFontUC',
             'ofxVLCRemote',
             'ofxVideoRecorder',
             'ofxXmlSettings',
            //'ofxSyphon'
        ]

        // additional flags for the project. the of module sets some
        // flags by default to add the core libraries, search paths...
        // this flags can be augmented through the following properties:
        of.pkgConfigs: []       // list of additional system pkgs to include
        of.includePaths: ['src/shapes/shapes/', 'src/shapes/', 'src', 'src/core', 'src/effects']     // include search paths
        of.cFlags: ['-fpermissive']  // flags passed to the c compiler
        of.cxxFlags: []         // flags passed to the c++ compiler
        of.linkerFlags: []      // flags passed to the linker
        //of.defines: ['KM_EDITOR_APP']          // defines are passed as -D to the compiler
        of.defines: ['KM_ANIMATOR_APP']
        // and can be checked with #ifdef or #if in the code
        of.frameworks: []       // osx only, additional frameworks to link with the project

        // other flags can be set through the cpp module: http://doc.qt.io/qbs/cpp-module.html
        // eg: this will enable ccache when compiling
        //
        // cpp.compilerWrapper: 'ccache'

        Depends{
            name: "cpp"
        }

        // common rules that parse the include search paths, core libraries...
        Depends{
            name: "of"
        }

        // dependency with the OF library
        Depends{
            name: "openFrameworks"
        }
    }

    references: [FileInfo.joinPaths(of_root, "/libs/openFrameworksCompiled/project/qtcreator/openFrameworks.qbs")]
}

