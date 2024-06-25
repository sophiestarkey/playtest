#version 330 core

in vec3 v_position;
in vec3 v_normal;

out vec4 f_color;

void main()
{
	vec3 V = normalize(-v_position);
	vec3 N = normalize(v_normal);
	float brightness = clamp(dot(V, N), 0.0, 1.0);
	f_color = vec4(vec3(brightness), 1.0);
	f_color.rgb = pow(f_color.rgb, vec3(1.0 / 2.2));
}