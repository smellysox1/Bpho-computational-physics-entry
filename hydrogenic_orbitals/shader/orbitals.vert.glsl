#version 330 core
layout (location = 0) in vec2 pos;

uniform mat4 projection;
uniform int numPlanes;
uniform int cameraPlaneID;
uniform float size;

out vec3 fragPos;

void main() {
    vec3 transformedPos = vec3(pos, 2.0 * float(gl_InstanceID) / float(numPlanes) - 1.0);

    if (gl_InstanceID > cameraPlaneID) {
        int diff = gl_InstanceID - cameraPlaneID;

        transformedPos = vec3(pos, 2.0 * float(numPlanes - diff) / float(numPlanes) - 1.0);
    }

    fragPos = transformedPos * size;

    gl_Position = projection * vec4(transformedPos, 1.0);
}