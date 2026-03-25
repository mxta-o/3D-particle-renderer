#version 330 core
layout(location = 0) in vec3 aPos;
uniform mat4 u_MVP;
out vec3 vPos;
void main() {
    vPos = aPos;
    gl_Position = u_MVP * vec4(aPos, 1.0);
}
