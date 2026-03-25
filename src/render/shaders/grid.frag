#version 330 core
in vec3 vPos;
out vec4 FragColor;
uniform float u_time;
uniform vec3 u_color;
void main() {
    float base = 0.9;
    float pulse = 0.1 * sin(u_time * 2.0 + vPos.x * 0.5 + vPos.z * 0.5);
    float brightness = clamp(base + pulse, 0.0, 1.0);
    vec3 col = u_color * brightness;
    FragColor = vec4(col, 1.0);
}
