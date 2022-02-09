#version 330 core

uniform View {
    vec2 view_scale;
    vec2 view_offset;
    int resolution;
};

out vec2 coords;

void main() {
    vec2 uv = vec2((gl_VertexID << 1) & 2, gl_VertexID & 2);
    vec4 out_uv = vec4(uv * vec2(2, -2) + vec2(-1, 1), 0, 1);
	
    coords = (out_uv.xy * view_scale) + view_offset;
    gl_Position = out_uv;
}