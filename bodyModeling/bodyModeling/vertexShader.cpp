#include "vertexShader.h"

string generateVertexShader()
{
	string src = 

"#ifdef GL_ES\n"
	  	"precision highp float;\n"
"#endif\n"
		"\n"
        "attribute vec3 aPosition;\n"
        "attribute vec3 aNormal;\n"
        "attribute vec2 aTexture;\n"

		"attribute vec3 aPositionOffsets1;\n"
		"attribute vec3 aNormalOffsets1;\n"
		"uniform float scaleFactor1;\n"

		"attribute vec3 aPositionOffsets2;\n"
		"attribute vec3 aNormalOffsets2;\n"
		"uniform float scaleFactor2;\n"
		
		"attribute vec3 aPositionOffsets3;\n"
		"attribute vec3 aNormalOffsets3;\n"
		"uniform float scaleFactor3;\n"
		
		"attribute vec3 aPositionOffsets4;\n"
		"attribute vec3 aNormalOffsets4;\n"
		"uniform float scaleFactor4;\n"
		
		"attribute vec3 aPositionOffsets5;\n"
		"attribute vec3 aNormalOffsets5;\n"
		"uniform float scaleFactor5;\n"
		
		"attribute vec3 aPositionOffsets6;\n"
		"attribute vec3 aNormalOffsets6;\n"
		"uniform float scaleFactor6;\n"

		"attribute vec3 aPositionOffsets7;\n"
		"attribute vec3 aNormalOffsets7;\n"
		"uniform float scaleFactor7;\n"

		"uniform float meanScaleFactor;\n"
        "uniform mat4 world;\n"
        "uniform mat4 worldInverseTranspose;\n"
        "uniform mat4 worldViewProj;\n"
        "uniform mat4 viewInverse;\n"
        "uniform mat4 normalMatrix;\n"
        "\n"
        "varying vec3 vLighting;\n"
        "varying vec2 vTexture;\n"
        "\n"
        "void main() {\n"
		"  gl_Position = worldViewProj * vec4( meanScaleFactor*aPosition.xyz + scaleFactor1*aPositionOffsets1.xyz+ scaleFactor2*aPositionOffsets2.xyz+ scaleFactor3*aPositionOffsets3.xyz+ scaleFactor4*aPositionOffsets4.xyz+ scaleFactor5*aPositionOffsets5.xyz+ scaleFactor6*aPositionOffsets6.xyz+ scaleFactor7*aPositionOffsets7.xyz, 1.0);\n"
        "  vec3 ambientLight = vec3(0.2, 0.2, 0.2);\n"
        "  vec3 directionalLightColor = vec3(0.9, 0.9, 0.75);\n"
        "  vec3 directionalVector = vec3(0.0, 0.0, 1.0);\n"
        "  vec3 directionalVector2 = 0.41*vec3(1.0, 2.0, 1.0);\n"
		"  vec3 specDirection = 0.667*vec3(0.5, 1.0, 1.0);\n"
		
		"  vec3 normalVector = normalize(vec3( aNormal.xyz+ scaleFactor1*aNormalOffsets1.xyz+ scaleFactor2*aNormalOffsets2.xyz+ scaleFactor3*aNormalOffsets3.xyz+ scaleFactor4*aNormalOffsets4.xyz+ scaleFactor5*aNormalOffsets5.xyz+ scaleFactor6*aNormalOffsets6.xyz+ scaleFactor7*aNormalOffsets7.xyz ));\n"

        "  vec4 transformedNormal = normalMatrix * vec4(normalVector, 1.0);\n"
//      "  float directional = max(dot(transformedNormal.xyz, directionalVector), 0.0);"
//      "  vec4 transformedNormal = normalMatrix * vec4(normalVector, 0.0);"
        "  float directional = abs(dot(transformedNormal.xyz, directionalVector));\n"
        "  float directional2 = abs(dot(transformedNormal.xyz, directionalVector2));\n"
		"  float directional3 = pow(max(dot(transformedNormal.xyz, specDirection),0.0), 10.0);\n"
        "  vLighting = ambientLight + (directionalLightColor * directional) + 0.3*(directionalLightColor * directional2) + 0.6*(directionalLightColor * directional3);\n"
//      "  vLighting = ambientLight + (directionalLightColor * directional);"
//      "  vTexture = aTexture;"
        "}\n";

	return src;
}


string generatefragmentShader(bool texture)
{
	String tempString= "";

	if(texture)
     tempString =  "    vec4 col = texture2D(textureSampler, vTexture);\n";
	else
     tempString =  "    vec4 col = color;\n";

	string src = 
"#ifdef GL_ES\n"
	"precision highp float;\n"
"#endif\n"
	"\n"
    "varying vec3 vLighting;\n"
    "varying vec2 vTexture;\n"
    "\n"
    "uniform vec4 color;\n"
    "uniform sampler2D textureSampler;\n"
    "void main() {\n"+
	tempString+
    "    gl_FragColor = vec4(col.rgb*vLighting,col.a);\n"
    "}";
	return src;
}

string phongVertexShader()
{
	string vertex = 
				"varying vec3 N;"
				"varying vec3 v;"
				"void main(void)"
				"{ "
				"   v = vec3(gl_ModelViewMatrix * gl_Vertex);"
				"   N = normalize(gl_NormalMatrix * gl_Normal);"
				"   gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;"
				"}";

	return vertex;
}


string phongFragShader()
{
	string frag = 
				"varying vec3 N;"
				"varying vec3 v;"
				"void main(void)"
				"{ "
				"  vec3 L = normalize(gl_LightSource[0].position.xyz - v);"
				"  vec4 Idiff = gl_FrontLightProduct[0].diffuse * max(dot(N,L), 0.0);"
				"  Idiff = clamp(Idiff, 0.0, 1.0); "
				"  gl_FragColor = Idiff;"
				"}";

	return frag;
}
