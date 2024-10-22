#version 410 core

layout(points) in;
layout(triangle_strip, max_vertices = 20) out;

uniform int m,n;
uniform float blocksize;
const semiboundaryWidth = 5.0f;
void main() {
    vec4 center =  gl_in[0].gl_Position;
    float semiWidth = blocksize * m / 2;
    float semiHeight = blocksize * n / 2;
    //top
    gl_Position = center + vec4(-semiWidth - semiboundaryWidth, semiHeight + semiboundaryWidth, 0.0, 0.0);
    EmitVertex();
    gl_Position = center + vec4(-semiWidth + semiboundaryWidth, semiHeight - semiboundaryWidth, 0.0, 0.0);
    EmitVertex();
    gl_Position = center + vec4( semiWidth - semiboundaryWidth, semiHeight - semiboundaryWidth, 0.0, 0.0);
    EmitVertex();
    gl_Position = center + vec4( semiWidth + semiboundaryWidth, semiHeight + semiboundaryWidth, 0.0, 0.0);
    EmitVertex();
    //right
    gl_Position = center + vec4( semiWidth - semiboundaryWidth,-semiHeight + semiboundaryWidth, 0.0, 0.0);
    EmitVertex();
    gl_Position = center + vec4( semiWidth + semiboundaryWidth,-semiHeight - semiboundaryWidth, 0.0, 0.0);
    EmitVertex();
    //bottom
    gl_Position = center + vec4(-semiWidth + semiboundaryWidth,-semiHeight + semiboundaryWidth, 0.0, 0.0);
    EmitVertex();
    gl_Position = center + vec4(-semiWidth - semiboundaryWidth,-semiHeight - semiboundaryWidth, 0.0, 0.0);
    //left
    gl_Position = center + vec4(-semiWidth + semiboundaryWidth, semiHeight - semiboundaryWidth, 0.0, 0.0);
    EmitVertex();
    gl_Position = center + vec4(-semiWidth - semiboundaryWidth, semiHeight + semiboundaryWidth, 0.0, 0.0);
    EmitVertex();
    EndPrimitive();
}


