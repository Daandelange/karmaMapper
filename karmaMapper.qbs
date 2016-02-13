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
            'src/core/karmaConsole.cpp',
            'src/core/karmaConsole.h',

            // MODULES (CORE)
            'src/modules/karmaModule.cpp',
            'src/modules/karmaModule.h',
            'src/modules/singletonModule.cpp',
            'src/modules/singletonModule.h',
            'src/modules/moduleFactory.cpp',
            'src/modules/moduleFactory.h',

            // MODULES
            'src/modules/fboRecorder/fboRecorder.cpp',
            'src/modules/fboRecorder/fboRecorder.h',
            'src/modules/soundAnalyser/karmaSoundAnalyser.cpp',
            'src/modules/soundAnalyser/karmaSoundAnalyser.h',
            'src/modules/mirOSC/mirEvents.h',
            'src/modules/mirOSC/mirReceiver.cpp',
            'src/modules/mirOSC/mirReceiver.h',
            'src/modules/durationOSC/durationEvents.h',
            'src/modules/durationOSC/durationReceiver.cpp',
            'src/modules/durationOSC/durationReceiver.h',
            'src/modules/durationOSC/durationRC.cpp',
            'src/modules/durationOSC/durationRC.h',
            'src/modules/oscRouter/OSCNode.cpp',
            'src/modules/oscRouter/OSCNode.h',
            'src/modules/oscRouter/OSCRouter.cpp',
            'src/modules/oscRouter/OSCRouter.h',

            // PARAMS
            'src/parameters/animationParams.h',
            'src/parameters/animationParamsServer.cpp',
            'src/parameters/animationParamsServer.h',

            // EFFECTS
            'src/effects/effects.h',
            'src/effects/effectFactory.cpp',
            'src/effects/effectFactory.h',

            'src/effects/basicEffect.cpp',
            'src/effects/basicEffect.h',
            'src/effects/distortEffect/distortEffect.cpp',
            'src/effects/distortEffect/distortEffect.h',
            'src/effects/imageShader/imageShader.cpp',
            'src/effects/imageShader/imageShader.h',
            'src/effects/lineEffect/lineEffect.cpp',
            'src/effects/lineEffect/lineEffect.h',
            'src/effects/lineEffect/lineEffectLine.cpp',
            'src/effects/lineEffect/lineEffectLine.h',
            'src/effects/lineDrawEffect/lineDrawEffect.cpp',
            'src/effects/lineDrawEffect/lineDrawEffect.h',
            'src/effects/lineDrawEffect/lineDrawEffectLine.cpp',
            'src/effects/lineDrawEffect/lineDrawEffectLine.h',
            'src/effects/shaderEffect/shaderEffect.cpp',
            'src/effects/shaderEffect/shaderEffect.h',
            'src/effects/videoShader/videoShader.cpp',
            'src/effects/videoShader/videoShader.h',


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
            'src/shapes/shapes/fullScreenShape.cpp',
            'src/shapes/shapes/fullScreenShape.h',
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
             //'ofxOpenCv',
             //'ofxAbletonLiveSet',
             //'ofxAssimpModelLoader',
             'ofxGuiExtended',
             'ofxOsc',
             'ofxTextBox',
             //'ofxTrueTypeFontUC',
             'ofxVLCRemote',
             'ofxVideoRecorder',
             'ofxXmlSettings',
             'ofxImGui',
             'ofxSyphon'
        ]

        // additional flags for the project. the of module sets some
        // flags by default to add the core libraries, search paths...
        // this flags can be augmented through the following properties:
        of.pkgConfigs: []       // list of additional system pkgs to include

        // unfortunately you have to enter all these manually... :'(
        of.includePaths: [
            'src/shapes/shapes/',
            'src/shapes/',
            'src',
            'src/core',
            'src/effects',
                'src/effects/distortEffect',
                'src/effects/imageShader',
                'src/effects/lineDrawEffect',
                'src/effects/lineEffect',
                'src/effects/shaderEffect',
                'src/effects/videoShader',
            'src/parameters',
            'src/modules',
                'src/modules/mirOSC',
                'src/modules/oscRouter',
                'src/modules/fboRecorder',
                'src/modules/soundAnalyser',
                'src/modules/mirOSC',
                'src/modules/durationOSC',
//            '../../../addons/ofxSyphon/libs',
//            '../../../addons/ofxSyphon/libs/Syphon',
//            '../../../addons/ofxSyphon/libs/Syphon/lib',
//            '../../../addons/ofxSyphon/libs/Syphon/lib/osx',
//            '../../../addons/ofxSyphon/libs/Syphon/src',
//            '../../../addons/ofxSyphon/src',
//            '/Library/Frameworks/'
        ] // include search paths
        of.cFlags: ['-fpermissive']  // flags passed to the c compiler
        of.cxxFlags: []         // flags passed to the c++ compiler
        of.linkerFlags: ['-v']//, '-F/Developer/openFrameworks/addons/ofxSyphon/libs/Syphon/lib/osx']      // flags passed to the linker
        //of.defines: ['KM_EDITOR_APP', 'KM_QT_CREATOR'] // defines are passed as -D to the compiler
        of.defines: ['KM_ANIMATOR_APP', 'KM_QT_CREATOR']
        // and can be checked with #ifdef or #if in the code
        //of.frameworks: ['Syphon']  // osx only, additional frameworks to link with the project
        //of.frameworks: ['Qtkit', 'Syphon']

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

//        Depends{
//            name: "Syphon.framework"
//        }
    }

    references: [FileInfo.joinPaths(of_root, "/libs/openFrameworksCompiled/project/qtcreator/openFrameworks.qbs")]
}

