#include "shaders.h"

/* Functions */
GLuint shaderCreate(const char *shaderSource, GLenum mode) 
{
	
	GLint err;
	GLuint shader = glCreateShader(mode);
	glShaderSource(shader, 1, &shaderSource, NULL);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &err);
	if (err == GL_FALSE) 
	{
		return 0;
	}
	return shader;
}

/* Libray shader */
const extern char library_shader[] = R"glsl(
	#version 330 core

	float rand3D(in vec3 co){
		return fract(sin(dot(co.xyz ,vec3(12.9898,78.233,144.7272))) * 43758.5453);
	}

	float simple_interpolate(in float a, in float b, in float x)
	{
		return a + smoothstep(0.0,1.0,x) * (b-a);
	}

	float interpolatedNoise3D(in float x, in float y, in float z)
	{
		float integer_x = x - fract(x);
		float fractional_x = x - integer_x;

	    float integer_y = y - fract(y);
	    float fractional_y = y - integer_y;

	    float integer_z = z - fract(z);
	    float fractional_z = z - integer_z;

	    float v1 = rand3D(vec3(integer_x, integer_y, integer_z));
	    float v2 = rand3D(vec3(integer_x+1.0, integer_y, integer_z));
	    float v3 = rand3D(vec3(integer_x, integer_y+1.0, integer_z));
	    float v4 = rand3D(vec3(integer_x+1.0, integer_y +1.0, integer_z));

	    float v5 = rand3D(vec3(integer_x, integer_y, integer_z+1.0));
	    float v6 = rand3D(vec3(integer_x+1.0, integer_y, integer_z+1.0));
	    float v7 = rand3D(vec3(integer_x, integer_y+1.0, integer_z+1.0));
	    float v8 = rand3D(vec3(integer_x+1.0, integer_y +1.0, integer_z+1.0));

	    float i1 = simple_interpolate(v1,v5, fractional_z);
	    float i2 = simple_interpolate(v2,v6, fractional_z);
	    float i3 = simple_interpolate(v3,v7, fractional_z);
	    float i4 = simple_interpolate(v4,v8, fractional_z);
	
	    float ii1 = simple_interpolate(i1,i2,fractional_x);
	    float ii2 = simple_interpolate(i3,i4,fractional_x);

	    return simple_interpolate(ii1 , ii2 , fractional_y);
	}

	float Noise3D(in vec3 coord, in float wavelength)
	{
		return interpolatedNoise3D(coord.x/wavelength, coord.y/wavelength, coord.z/wavelength);
	}

	float circle(in vec2 _st, in float _radius){
		vec2 dist = _st-vec2(0.5);
		return 1.-smoothstep(_radius-(_radius*0.01),
                         _radius+(_radius*0.01),
                         dot(dist,dist)*4.0);
	}

	float rand2D(in vec2 co){
		return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
	}

	float dotNoise2D(in float x, in float y, in float fractionalMaxDotSize, in float dDensity)
	{
		float integer_x = x - fract(x);
		float fractional_x = x - integer_x;

		float integer_y = y - fract(y);
		float fractional_y = y - integer_y;

		if (rand2D(vec2(integer_x+1.0, integer_y +1.0)) > dDensity)
		   {return 0.0;}

		float xoffset = (rand2D(vec2(integer_x, integer_y)) -0.5);
		float yoffset = (rand2D(vec2(integer_x+1.0, integer_y)) - 0.5);
		float dotSize = 0.5 * fractionalMaxDotSize * max(0.25,rand2D(vec2(integer_x, integer_y+1.0)));

		vec2 truePos = vec2 (0.5 + xoffset * (1.0 - 2.0 * dotSize) , 0.5 + yoffset * (1.0 -2.0 * dotSize));

		float distance = length(truePos - vec2(fractional_x, fractional_y));

		return 1.0 - smoothstep (0.3 * dotSize, 1.0* dotSize, distance);

	}

	float DotNoise2D(in vec2 coord, in float wavelength, in float fractionalMaxDotSize, in float dDensity)
	{
	   return dotNoise2D(coord.x/wavelength, coord.y/wavelength, fractionalMaxDotSize, dDensity);
	}
	
)glsl"; //library_shader


/* Vertex shaders */
const extern char vert_shader_texture_rendering[] = R"glsl(
    #version 330 core

	in vec3 position;
    in vec2 texture_coord; 

	out vec2 vTexCoord;

	void main()
    {
		gl_Position = vec4(position, 1.f);
		vTexCoord = texture_coord;
    }
)glsl"; //vert_shader_texture_rendering

const extern char vert_shader_normals[] = R"glsl(
	#version 330 core

	float Noise3D(in vec3 coord, in float wavelength);

    in vec3 position;
    in vec3 normal;
	in vec2 TexCoord;
    
	out vec3 positionWS, normalWS;
	out vec2 vTexCoord;

	uniform mat4 modelMatrix;
	uniform mat4 projectionMatrix;
	uniform mat4 viewMatrix;

	uniform float scale;

	void main()
    {
		gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(position * scale, 1.0);
		positionWS = vec3(modelMatrix * vec4(position, 1.0));
		normalWS = vec3(modelMatrix * vec4(normal, 0.0));
		vTexCoord = TexCoord;
    }
)glsl"; //vert_shader_normals

const extern char vert_shader_planet[] = R"glsl(
    #version 330 core

	float Noise3D(in vec3 coord, in float wavelength);

    in vec3 position;
    in vec2 TexCoord;
    
	out vec3 positionWS, normalWS;
	out vec2 vTexCoord;

	uniform mat4 modelMatrix;
	uniform mat4 projectionMatrix;
	uniform mat4 viewMatrix;

	uniform float scale;

    void main()
    {
		float noise = (Noise3D(position, 0.1) - 0.5) *0.05;

		float a = Noise3D(position, 128);
		a += Noise3D(position, 0.5);
		a += Noise3D(position, 0.25);
		a += Noise3D(position, 0.125);
		noise = a/4 * 0.15;

		vec3 offset = position * noise;
        gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4((position + offset) * scale, 1.0);
		positionWS = vec3(modelMatrix * vec4(position + offset, 1.0));
		normalWS = vec3(modelMatrix * vec4(position + offset, 0.0));
		vTexCoord = TexCoord;
    }
)glsl"; //vert_shader_planet

const char vert_shader_skybox[] = R"glsl(
	#version 330 core

	in vec3 position;
	in vec2 TexCoord;
    
	out vec3 positionWS, normalWS;
	out vec2 vTexCoord;

	uniform mat4 modelMatrix;
	uniform mat4 projectionMatrix;
	uniform mat4 viewMatrix;

	uniform float scale;

	void main()
    {
		gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(position * scale, 1.0);
		positionWS = vec3(modelMatrix * vec4(position, 1.0));
		normalWS = vec3(modelMatrix * vec4(position, 0.0));
		vTexCoord = TexCoord;
	}
)glsl"; //vert_shader_skybox



/* Fragment shaders */
const char frag_shader_sun[] = R"glsl(
	#version 330 core

	#define TWO_PI 6.28318530718

	in vec4 gl_FragCoord;
	out vec4 outColor;

	void main() {

	vec2 st = gl_FragCoord.xy/2048;

	st = st *2.-1.;

	float a = atan(st.x,st.y) + 3.14159265359;
	float r = TWO_PI/float(13);

	float d = cos(floor(.5+a/r)*r-a)*length(st);
	d = 1. - d;

	outColor = vec4(vec3(d,d,d * 0.89),d);
	}
)glsl"; //frag_shader_sun

const char frag_sh_clouds[] = R"glsl(
#version 330 core

	float Noise3D(in vec3 coord, in float wavelength);

	in vec4 gl_FragCoord;

	out vec4 outColor;

	void main()
    {
		float n1 = Noise3D(gl_FragCoord.xyz, 128);
		float n2 = Noise3D(gl_FragCoord.xyz, 64);
		float n3 = Noise3D(gl_FragCoord.xyz, 32);
		float n4 = Noise3D(gl_FragCoord.xyz, 16);
		float n5 = Noise3D(gl_FragCoord.xyz, 0.625);
		float n = (n1 + n2 + n3 +n4 +n5)/5;
		n = 1 - smoothstep(0.455 - 0.1, 0.455 + 0.1, n);

		outColor = vec4(1.0, 1.0, 1.0, n);
    }
)glsl"; //frag_sh_clouds

const char frag_shader_earth[] = R"glsl(
    #version 330 core

	float Noise3D(in vec3 coord, in float wavelength);

	float circle(in vec2 _st, in float _radius);	

	in vec4 gl_FragCoord;
	out vec4 outColor;

	void main()
    {
		float n1 = Noise3D(gl_FragCoord.xyz, 50);
		float n2 = Noise3D(gl_FragCoord.xyz, 40);
		float n3 = Noise3D(gl_FragCoord.xyz, 30);
		float n4 = Noise3D(gl_FragCoord.xyz, 20);
		float n5 = Noise3D(gl_FragCoord.xyz, 5);
		float n = (n1 + n2 + n3 +n4 +n5)/5;
		n = smoothstep(0.55 - 0.05, 0.55 + 0.05, n); 

		float a = Noise3D(gl_FragCoord.xyz, 15);
		float c = a * 0.15;
		a = Noise3D(gl_FragCoord.yxz, 41.62);
	
		vec2 st = gl_FragCoord.xy/2048;
		float b = circle(st, 0.05 + c);

		vec3 c1 = vec3(0.25, 0.39, 0.81);
		vec3 c2 = vec3(0.f, 0.8, 0.f);

		vec3 c3	= mix(c1, c2, n);

		vec3 c4 = vec3(b, b, b);

		outColor = vec4(max(c3, c4), 1.0);
    }
)glsl"; //frag_shader_earth

const char frag_shader_perlin[] = R"glsl(
    #version 330 core

	float Noise3D(in vec3 coord, in float wavelength);

	float circle(in vec2 _st, in float _radius);

	in vec4 gl_FragCoord;
	out vec4 outColor;

	void main()
    {
		float n1 = Noise3D(gl_FragCoord.xyz, 20);
		float n2 = Noise3D(gl_FragCoord.xyz, 10);
		float n3 = Noise3D(gl_FragCoord.xyz, 5);
		float n4 = Noise3D(gl_FragCoord.xyz, 2.5);
		float n5 = Noise3D(gl_FragCoord.xyz, 1.25);
		float n = (n1 + n2 + n3 +n4 +n5)/5;


		float a = Noise3D(gl_FragCoord.xyz, 2) + Noise3D(gl_FragCoord.xyz, 1) + Noise3D(gl_FragCoord.xyz, 0.5);
		a = (a / 3) * 0.15;

		vec2 st = gl_FragCoord.xy/2048;
		float b = circle(st, 0.01 + a);
		
		vec3 t1 = vec3(0.99 * n, 0.35 * n , 0.21 * n);
		vec3 t2 = vec3(b , b, b);

		outColor = vec4(max(t1, t2), 1.0);
    }
)glsl"; //frag_shader_perlin

const char frag_shader_dot_noise[] = R"glsl(
    #version 330 core

	float DotNoise2D(in vec2 coord, in float wavelength, in float fractionalMaxDotSize, in float dDensity);

	in vec4 gl_FragCoord;
	out vec4 outColor;

	void main()
    {
		float a = DotNoise2D(gl_FragCoord.xy - 4, 3.26, 0.51, 0.015);
		float b = DotNoise2D(gl_FragCoord.yx + 2 + 0.5, 0.65, 0.21, 0.04);
		float c = DotNoise2D(-gl_FragCoord.xy, 2.006, 0.1, 0.02);
		float d = DotNoise2D(-gl_FragCoord.yx + 17.36, 1, 0.7, 0.03);
		a = max(max(a, b), c);
		outColor = vec4(a, a, a, 1.0);
    }
)glsl"; //frag_shader_dot_noise

const char frag_shader_spaceship[] = R"glsl(
	#version 330 core


	in vec4 gl_FragCoord;
	out vec4 outColor;

	void main()
	{
		float a = 128.f/255.f;

		outColor = vec4(a, a, a, 1.0);
	}

)glsl"; //frag_shader_spaceship

const char frag_shader_milky_way[] = R"glsl(
	#version 330 core

	#define TWO_PI 6.28318530718

	float Noise3D(in vec3 coord, in float wavelength);

	float circle(in vec2 _st, in float _radius);

	in vec4 gl_FragCoord;
	out vec4 outColor;	

	void main()
	{
		vec2 st = gl_FragCoord.xy/2048;
		st = st *2.-1.;
		float a = atan(st.x,st.y) + 3.14159265359;
		float r = TWO_PI/float(32);
		float d = cos(floor(.5+a/r)*r-a)*length(st);

		a = Noise3D(gl_FragCoord.xyz, 20) + Noise3D(gl_FragCoord.xyz, 10) + Noise3D(gl_FragCoord.xyz, 5); 
		a = (a/3) * 0.03;
		float b = 1 - circle(gl_FragCoord.xy/2048, 0.975 + a);
		
		a = Noise3D(gl_FragCoord.xyz, 18) + Noise3D(gl_FragCoord.xyz, 9) + Noise3D(gl_FragCoord.xyz, 4.5); 
		a = (a/3) * 0.03;
		float c = 1 - circle(gl_FragCoord.xy/2048, 0.99 + a);
		b = b - c;
		b = max(b, 0);

		float g = Noise3D(gl_FragCoord.xyz, 35) + Noise3D(gl_FragCoord.xyz, 17.5);
		g += Noise3D(gl_FragCoord.xyz, 8.75) + Noise3D(gl_FragCoord.xyz, 4.385);
		g += Noise3D(gl_FragCoord.xyz, 2.1925);

		g = g / 5;

		vec4 res = max(vec4(g * 0.65, 0.65 * g, 0.65 * g, d - 0.68), vec4(b * 0.570,b * 0.48,b * 0.394, b * 0.75));
		
		outColor = res;
	}
)glsl"; //frag_shader_milky_way

const char frag_shader_phong[] = R"glsl(
    #version 330 core

	in vec3 positionWS, normalWS;
	in vec2 vTexCoord;
    
	out vec4 outColor;

	uniform sampler2D texture_sampler;
	uniform vec3 cameraPosition;
	uniform vec3 lightPosition;
    
	void main()
    {
		float a = texture2D(texture_sampler, vTexCoord).a;
		if (a < 0.15)
			discard; 
		vec3 diffuse = vec3(texture2D(texture_sampler, vTexCoord));
		vec3 specular = vec3(0.1f, 0.1f, 0.1f);
		float shininess = 10.f;
		vec3 N = normalize(normalWS);
		vec3 V = normalize(cameraPosition - positionWS);
		vec3 R = reflect(-V, N);
		vec3 L = normalize(lightPosition - positionWS);
		float sF = pow((clamp(dot(R, L), 0.f, 1.f)), shininess);
		float dF = clamp(dot(N, L), 0.f, 1.f);
		vec3 dM = diffuse;
		vec3 sM = specular;
		vec3 dL = vec3(1.f, 1.f, 1.f);
		vec3 sL = dL;
		outColor = vec4(dF*dM*dL + sF*sM*sL, a);
    }
)glsl"; //frag_shader_phong

const char frag_shader_skybox[] = R"glsl(
    #version 330 core

	in vec2 vTexCoord;
    
	layout(location = 0) out vec4 outColor;

	uniform sampler2D texture_sampler;
    
	void main()
    {
		float a = texture2D(texture_sampler, vTexCoord).a;
		if (a < 0.1)
			discard; 

		outColor = texture2D(texture_sampler, vTexCoord);
    }
)glsl"; //frag_shader_skybox
