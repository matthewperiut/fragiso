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

    return texture(voxelShape, (texCoords)/voxelShapeSize);
}

vec3 rayPosition;
void main()
{
    vec2 coord = gl_FragCoord.xy - cameraPosition;

    rayPosition = vec3(coord,voxelShapeSize.z*2);
    rayPosition = vec3(rayPosition.x - int(coord/2), rayPosition.y, rayPosition.z- int(coord/2));

    int steps = 0;
    while (rayPosition.z > 0)
    {
        rayPosition.z -= 0.4;
        rayPosition.y -= 0.4;
        rayPosition.x -= 0.4;
        color = getColor(rayPosition);
        if (color != blank)
        {
            FragColor = color;
            return;
        }
    }

    if (steps > 48)
            discard;
    if (color == blank)
            discard;
}