#version 330 core
out vec4 FragColor;

// Adding multiple voxelShapes should be a priority
uniform sampler3D voxelShape;
uniform vec3 voxelShapeSize;
uniform vec3 voxelPosition;

uniform vec2 cameraPosition;

const vec4 blank = vec4(0.f,0.f,0.f,0.f);
vec4 color = blank;
vec4 getColor(vec3 texCoords)
{
    if (texCoords.x > voxelShapeSize.x || texCoords.y > voxelShapeSize.y || texCoords.z > voxelShapeSize.z)
    {
        if (texCoords.x < 0 || texCoords.y < 0 || texCoords.z < 0)
        {
            return blank;
        }
    }

    return texture(voxelShape, (texCoords + vec3(0,0,0.5))/voxelShapeSize);
}

bool nothing(vec3 pos)
{
    return getColor(pos) == blank;
}
bool anything(vec3 pos)
{
    return !nothing(pos);
}

vec3 getNormal(vec3 pos)
{
    int dist = 1;
    vec3 otherBlocks = vec3(0.f,0.f,0.f);

    otherBlocks.x = 1.f * float(int(nothing(pos+vec3(1.f,0.f,0.f)))) + -1.f * float(int(nothing(pos+vec3(-1.f,0.f,0.f))));
    otherBlocks.y = 1.f * float(int(nothing(pos+vec3(0.f,1.f,0.f)))) + -1.f * float(int(nothing(pos+vec3(0.f,-1.f,0.f))));
    otherBlocks.z = 1.f * float(int(nothing(pos+vec3(0.f,0.f,1.f)))) + -1.f * float(int(nothing(pos+vec3(0.f,0.f,-1.f))));

    vec3 away = otherBlocks;
    return normalize(away);
}

vec3 rayPosition;
void main()
{
    vec2 coord = gl_FragCoord.xy - cameraPosition;

    rayPosition = vec3(coord,200);
    rayPosition = vec3(rayPosition.x - int(coord/2), rayPosition.y, rayPosition.z- int(coord/2));

    int steps = 0;
    while (rayPosition.z > 0)
    {
        rayPosition.z --;
        rayPosition.y --;
        rayPosition.x --;

        color = getColor(rayPosition);
        if (color != blank)
        {
            vec3 normal = getNormal(rayPosition);

            vec3 lightDir = normalize(vec3(-0.2, -1, -1));

            float lightIntensity = max(dot(normal, -lightDir), 0.5f);

            float heightModifier = rayPosition.y / voxelShapeSize.y;

            FragColor = color * lightIntensity;
            return;
        }
    }

    if (steps > 48)
    discard;
    if (color == blank)
    discard;
}