#version 330 core
in vec3 vPos;
out vec4 FragColor;
uniform float u_time;
void main() {
    float base = 0.25;
    float pulse = 0.15 * sin(u_time * 2.0 + vPos.x * 0.5 + vPos.z * 0.5);
    float brightness = base + pulse;
    vec3 col = vec3(0.2, 0.5, 0.8) * brightness;
    FragColor = vec4(col, 1.0);
}
