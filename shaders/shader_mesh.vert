#version 120

uniform mat4 camera_projection;
uniform mat4 camera_modelview;
uniform mat4 normal_matrix;
uniform mat4 transformation_matrix;

uniform mat4 cam_rotation;
uniform vec4 cam_rot_center;
uniform vec4 cam_translation;

varying vec4 position_3d_original;
varying vec4 position_3d_modelview;

varying vec3 normal;
varying vec4 color;


void main (void)
{
    gl_Position = camera_projection/**camera_modelview*/*  (transformation_matrix * gl_Vertex + cam_translation);
    //gl_Position = cam_rotation * ( transformation_matrix * gl_Vertex - cam_rot_center) + cam_rot_center + cam_translation;
    position_3d_original = transformation_matrix*gl_Vertex;
    position_3d_modelview = camera_modelview*transformation_matrix*gl_Vertex;
    color = gl_Color;

    vec4 normal4d = normal_matrix*vec4(normalize(gl_Normal),0);
    normal = normal4d.xyz;

    gl_TexCoord[0]=gl_MultiTexCoord0;
}
