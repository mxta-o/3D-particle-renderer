#version 330 core
in vec4 vColor;
out vec4 FragColor;

void main() {
    // soft circle using gl_PointCoord
    vec2 uv = gl_PointCoord * 2.0 - 1.0;
    float d = dot(uv, uv);
    float alpha = smoothstep(1.0, 0.6, d);
    FragColor = vec4(vColor.rgb, vColor.a * alpha);
}
