#version 150

#ifdef GL_ES
precision highp float;
#endif

// incoming variables
// no need to declare these, use this as a list
layout(origin_upper_left) in vec4 gl_FragCoord;// <-- pixel position (layout fixes inverted y)
// in bool gl_FrontFacing;
// in vec2 gl_PointCoord;// <-- position of the vertex that created this pixel
// in float gl_FragDepth;// <-- gl_FragCoord.z, if DEPTH enabled

// these are our textures from OF
// depreciated for now #toberemoved
uniform vec4 globalColor;
uniform sampler2DRect tex0;
uniform vec2 tex0Resolution; // w,h

uniform float opacity;
uniform float timeValX;

// the line below asks karmaMapper::animator to send shaderToy variables
// ### karmaMapper request shaderToyValues

// set by karmaMapper
uniform vec2 	fboCanvas; // w,h
uniform vec4 	shapeBoundingBox; // x,y,w,h
uniform vec2 	shapeCenter; // x,y
uniform int 	textureMode; // 0=scale to shape, 1=cover, 2=fill (clamed edge), 3=fill (repeat)
uniform vec4 	globalTextureTransform; // position(x,y) scale(z, w)

// the following line requests mir Data from karmaMapper::animator
// ### karmaMapper request mirValues
uniform float mirZeroCrossings;
uniform float mirOnSetCalls;
uniform float mirZcr; // [0.0]
uniform float mirPitch; // [0.0 to 100.0+]
uniform float mirBpm; // [ 0 to 200+ ]
uniform float mirBalance; // [-1.0 to +1.0]
uniform float mirVolume; // [0.0 to 1.0+] // not yet
uniform int mirSilence; // 0 or 1

//uniform vec2 manualZoom; //*km slider(0.1,3.3,0.5679)

// this comes from the vertex shader
in vec2 texCoordVarying;

// this is the output of the fragment shader
out vec4 outputColor;

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main(){

	// NICE ONE - Mindfuck patterns
	// outputColor = globalColor;//vec4(texture(tex0, pos).rgb, texColor.a*opacity);
	// float offset = 0.1 + timeValX / 120.0 + mirZeroCrossings/100.0 + mirOnSetCalls/20.0;
	// vec2 xy = 500+gl_FragCoord.xy/5.0 + .5*timeValX;///fboCanvas.xy;
	// outputColor.rg = outputColor.rg * mod(cos(xy.x/5.0)*100.0+ dot( (((xy-timeValX*2.0)*sin(xy.yx))/(xy.yx+timeValX)) / vec2(2000.0*1.0, 1800.0+500.0*sin(timeValX*10)), xy ), 1.0);
	// END NICE ONE

	// NICE ONE - Flickering geomerty
	// vec2 positionFloat = (gl_FragCoord.xy/fboCanvas.xy);
	// vec2 flickeringAmount = vec2(texCoordVarying/fboCanvas.xy);
	// outputColor.rg = ((texCoordVarying-gl_FragCoord.xy)/50.0);
	// outputColor.b = 0;
	// outputColor.a = 1;
	// END NICE ONE

	
    // get rgb from tex0
    //vec2 pos;
    //if(fitMode==1) pos = ((gl_FragCoord.xy-shapeBoundingBox.xy) / floor(shapeBoundingBox.zw) ) * tex0Resolution; // fit tex to shape
    //else pos = ( (gl_FragCoord.xy-shapeBoundingBox.xy) / shapeBoundingBox.zw) * tex0Resolution; // fit tex to screen

	outputColor = globalColor;//vec4(texture(tex0, pos).rgb, texColor.a*opacity);
    
    //outputColor = vec4(offset.x, offset.y,0,1);

//	float offset = 0.1 + timeValX / 120.0 + mirZeroCrossings/100.0 + mirOnSetCalls/20.0;

	vec2 positionFloat = (gl_FragCoord.xy/fboCanvas.xy); // from 0 to 1, relative to fbo
	vec2 cornerDistance = ((texCoordVarying)/shapeBoundingBox.ba); // goes from -1 to 1, relative to shape
	

	//outputColor *= 0.1*mod( dot(gl_FragCoord.xy*2*( offset/2 ),gl_FragCoord.xy*offset),16.1+offset/3*2);
	vec2 zoom = vec2(10,10);

	vec2 xy = 100+ (gl_FragCoord.xy/zoom) + .5*timeValX + mirBpm/2.0;///fboCanvas.xy;

	// (xy.yx*vec2(timeValX/80.0+xy.y,timeValX*90.0)*10.0
	float expression = 
		mod(
			(cos( xy.x/(5.0+cornerDistance.x))*1
				+ dot(
					timeValX*0.1+((xy)*(sin(xy.yx+cornerDistance*cornerDistance*cornerDistance*positionFloat))+cos(xy/(50.0*cornerDistance))/(xy.yx*(cornerDistance*timeValX*60.0))) / vec2(1000.0+600.0*positionFloat.x+300.0*sin(timeValX), 1200.0+300.0*positionFloat.y+500.0*sin(timeValX)),
					xy
			))
			,
			3.0
		);
	//vec2 expression2 = vec2(
	//	mod( sin(xy.x+tan(xy.y-xy.x)*50)+1, sin(timeValX+xy.y)+1),
	//	mod( cos(xy.y+sin(xy.x)*100)+1, cos(timeValX+xy.x)+1)
	//);
	float expression3 = sin( dot(tan(xy.x/1000)-(xy.y), mod(sin(xy.y)+cos((xy.x+xy.y/1000)), tan(xy.x/1000)) ) ) +1;
	//outputColor.rg = (cornerDistance+1)/2.0;//cornerDistance;

	// gl_FragCoord.xy is in pixels
	// texCoordVarying ranges from -1 to 1 ???
	//outputColor.b = outputColor.b * mod( ,1.0);
	//outputColor.b = 0;

	outputColor.rgb = vec3( expression3 )/vec3(2.0);
	//outputColor.r = mod(expression+1,3.0)/3.0;
	//outputColor.g = mod(expression+2,3.0)/3.0;
	//outputColor.b = mod(expression+0,3.0)/3.0;
	outputColor.a = 1;

	//outputColor.b = outputColor.b * mod( dot(xy+xy.yx/(1000.0+xy+timeValX)+offset, xy), 1.0);
	//outputColor.b = mod(offset, 100.0);
	//outputColor.rgba = vec4(outputColor.r*outputColor.b,outputColor.r*outputColor.b,outputColor.r*outputColor.b,1);

	//outputColor.rgb*=-1.0;//-outputColor;
	//outputColor = vec4(offset, offset,0,1);
}
