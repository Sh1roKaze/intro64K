#pragma once

#include <glad\glad.h>
#include <gl\gl.h> 

/* Functions */
GLuint shaderCreate(const char *shaderSource, GLenum mode);

/* Library shaders */
const extern char library_shader[];

/* Vertex shaders */
const extern char vert_shader_texture_rendering[];
const extern char vert_shader_normals[];
const extern char vert_shader_planet[];
const extern char vert_shader_skybox[];

/* Fragment shaders */
const extern char frag_shader_sun[];
const extern char frag_shader_milky_way[];
const extern char frag_shader_phong[];
const extern char frag_sh_clouds[];
const extern char frag_shader_earth[];
const extern char frag_shader_perlin[];
const extern char frag_shader_dot_noise[];
const extern char frag_shader_skybox[];
const extern char frag_shader_spaceship[];

