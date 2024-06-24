#version 330 core

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;

out vec3 v_position;
out vec3 v_normal;

uniform mat4 u_projection_mat;
uniform mat4 u_view_mat;
uniform mat4 u_model_mat;

void main()
{
	gl_Position = u_projection_mat * u_view_mat * u_model_mat * vec4(a_position, 1.0);
	v_position = vec3(u_view_mat * u_model_mat * vec4(a_position, 1.0));
	v_normal = vec3(u_view_mat * u_model_mat * vec4(a_normal, 0.0));
}
