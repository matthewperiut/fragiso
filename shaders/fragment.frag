#version 330 core
out vec4 FragColor;

in vec4 vertexColor; // the input variable from the vertex shader (same name and same type)

//uniform vec3 voxelShapeSize;
uniform sampler3D voxelShape;
uniform vec3 voxelShapeSize;
uniform vec3 voxelPosition;

uniform vec3 cameraPosition;

vec3 rayPosition;
vec2 displacement = -vec2(256.0,144.0)/2;
const vec3 fixedStep = vec3(2,-1,2);
vec3 stepProcess = fixedStep;

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

    return vec4(texture(voxelShape, (texCoords)/voxelShapeSize).rgb, 1.f);
}

void main()
{
    /*
    rayPosition = vec3(displacement.x+gl_FragCoord.x, 50.0, displacement.y+gl_FragCoord.y);

    for (float i = 0.0; i < 50.0; i++)
    {
        color = getColor(vec3(rayPosition) - vec3(0.0,i,0.0));
        if (color != blank)
                break;
    }

    if (color == blank)
            discard;

    FragColor = color;

    //FragColor = getColor(vec3(gl_FragCoord.x, 0, gl_FragCoord.y));
    */

    color = getColor(vec3(mod(gl_FragCoord.x,voxelShapeSize.x),mod(gl_FragCoord.y,voxelShapeSize.y),trunc(gl_FragCoord.x/voxelShapeSize.z)));
    if (color == blank)
            discard;
    FragColor = color;
}

/*
vec4 getColor(vec3 texCoords)
{
    if (texCoords.xyz > 0)
            return vec4(0.f,1.f,0.f,1.f);
    return texture(voxelShape, texCoords/voxelShapeSize);
}

void main()
{
    vec4 pos = gl_FragCoord;
    if (pos.x > 256 || pos.y > 144)
            discard;

    pos = vec4(displacement.xy,0.f,0.f);
    rayPosition = vec3(pos.x, pos.y, pos.x);
    for (int i = 0; i < 20; i++)
    {
        stepProcess = fixedStep;
        while (stepProcess.y < 0)
        {
            color = getColor(rayPosition);
            if (color.xyz != blank.xyz)
            {
                FragColor = color;
                return;
            }
            stepProcess.y++;
            rayPosition.y--;
        }

        while (stepProcess.x > 0 && stepProcess.y > 0)
        {
            color = getColor(rayPosition);
            if (color.xyz != blank.xyz)
            {
                FragColor = color;
                return;
            }
            stepProcess.x--;
            stepProcess.z--;
            rayPosition.x++;
            rayPosition.z++;
        }
    }
    discard;

    //FragColor = vec4(0.f,0.f,1.f,1.f);


     = getColor(vec3(1,0,0));
    FragColor = vec4(color.xyz, 1.f);


    if (pos.x >= 254 && pos.x < 255 && pos.y >= 142 && pos.y < 143)
    {
        FragColor = vec4(1.f,1.f,1.f,1.f);
    }
}
*/