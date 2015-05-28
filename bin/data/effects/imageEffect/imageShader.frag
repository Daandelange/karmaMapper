#version 120
 
#ifdef GL_ES
precision highp float;
#endif

// this is how we receive the texture
uniform sampler2DRect tex0;
uniform vec2 imageResolution;
uniform vec2 canvasResolution;

varying vec2 varyingtexcoord;
//varying vec4 outputColor;
 
void main()
{
	//vec2 innerPos = 
	//gl_FragCoord = coordinates of current pixel
    //gl_FragColor = vec4(varyingtexcoord.x,0,0,1);
    //gl_FragColor = texture2DRect(tex0, gl_FragCoord.xy);//gl_TexCoord[0].st);//varyingtexcoord);
    gl_FragColor = texture2DRect(tex0, gl_FragCoord.xy);
}