// #version 150

// uniform sampler2D texMap1;
// uniform sampler2D texMap2;

// out vec4 fragColor;
// in vec2 texCoordVarying;

// void main() {
//     vec4 T1  = texture(texMap1,texCoordVarying);
//     vec4 T2  = texture(texMap2,texCoordVarying);

//     fragColor = T1*T2;
//     //fragColor = vec4(1,0,0,1);
// }

#version 150

// Note: source cloned from Openframeworks GL/BilboardExample

uniform sampler2D tex;

in vec4 colorVarying;

out vec4 fragColor;

void main (void) {
	//fragColor = vec4(1,0,0,1);
	if(true){
		fragColor = texture(tex, gl_PointCoord) * colorVarying;
	}
	else {
		fragColor = colorVarying;
	}
}
