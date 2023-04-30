#version 330 core
out vec4 FragColor;

// Adding multiple voxelShapes should be a priority
uniform sampler3D voxelShape;
uniform sampler3D normalShape;

uniform vec3 voxelShapeSize;
uniform vec3 voxelPosition;

uniform vec2 cameraPosition;

uniform float time;

const vec4 blank = vec4(0.f,0.f,0.f,0.f);
vec4 color = blank;

vec4 getColor(vec3 texCoords)
{
    return texture(voxelShape, (texCoords + vec3(0,0,0.5))/voxelShapeSize);
}

bool anything(vec3 pos)
{
    return getColor(pos) != blank;
}

vec3 getNormal(vec3 pos)
{
    vec3 normal = texture(normalShape, (pos + vec3(0,0,0.5))/voxelShapeSize).xyz;
    return normalize(normal);
}

vec3 rayPosition;

void main()
{
    vec2 coord = gl_FragCoord.xy - cameraPosition;

    rayPosition = vec3(coord,200);
    rayPosition = vec3(rayPosition.x - int(coord/2), rayPosition.y, rayPosition.z- int(coord/2));

    float radius = 1;
    vec3 noLight = vec3(-(radius * cos(time)), -1, -(radius * sin(time)));
    vec3 lightDir = normalize(noLight);

    int steps = 0;
    while (rayPosition.y > 0)
    {
        rayPosition.z --;
        rayPosition.y --;
        rayPosition.x --;

        color = getColor(rayPosition);

        if (color != blank)
        {
            vec3 normal = getNormal(rayPosition);

            float lowestLight = 0.5f;
            float lightIntensity = max(dot(normal, -lightDir), lowestLight);

            while (rayPosition.y < voxelShapeSize.y)
            {
                rayPosition -= noLight;
                if (anything(rayPosition))
                {
                    lightIntensity = lowestLight;
                    break;
                }
            }

            FragColor = color * lightIntensity;
            return;
        }
    }

    if (steps > 48)
        discard;
    if (color == blank)
        discard;
}