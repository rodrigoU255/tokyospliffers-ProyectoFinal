#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;
layout (location = 3) in ivec4 boneIds;
layout (location = 4) in vec4 weights;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 finalBonesMatrices[MAX_BONES];
uniform bool isAnimated;

void main()
{
    vec4 totalPosition = vec4(position, 1.0f);
    vec3 totalNormal = normal;

    if (isAnimated)
    {
        vec4 animPos = vec4(0.0f);
        vec3 animNorm = vec3(0.0f);
        bool hasInfluence = false;

        for (int i = 0; i < MAX_BONE_INFLUENCE; i++)
        {
            if (boneIds[i] == -1 || weights[i] <= 0.0f)
                continue;

            if (boneIds[i] >= MAX_BONES)
            {
                animPos = vec4(position, 1.0f);
                animNorm = normal;
                hasInfluence = true;
                break;
            }

            hasInfluence = true;
            vec4 localPosition = finalBonesMatrices[boneIds[i]] * vec4(position, 1.0f);
            animPos += localPosition * weights[i];

            vec3 localNormal = mat3(finalBonesMatrices[boneIds[i]]) * normal;
            animNorm += localNormal * weights[i];
        }

        // Respaldo: si una submalla dentro del .glb no tiene huesos asignados, no colapsa al origen
        if (hasInfluence)
        {
            totalPosition = animPos;
            totalNormal = animNorm;
        }
    }

    gl_Position = projection * view * model * totalPosition;
    FragPos = vec3(model * totalPosition);
    Normal = mat3(transpose(inverse(model))) * totalNormal;
    TexCoords = texCoords;
}