//
// Created by Matthew Periut on 4/8/23.
//

#include <iostream>
#include "normal.h"
#include "voxelshape.h"

uint8_t packTwoBitValues(uint8_t value1, uint8_t value2, uint8_t value3)
{
    uint8_t packedValues = 0;

    // Pack the values into the bits of the variable
    packedValues |= (value1 & 0x3) << 6; // Put the first 2-bit value in the upper 2 bits
    packedValues |= (value2 & 0x3) << 4; // Put the second 2-bit value in the middle 2 bits
    packedValues |= (value3 & 0x3) << 2; // Put the third 2-bit value in the lower 2 bits

    return packedValues;
}

/*
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

    otherBlocks.x = 1.f * float(int(nothing(1.f,0.f,0.f)))) + -1.f * float(int(nothing(-1.f,0.f,0.f))));
    otherBlocks.y = 1.f * float(int(nothing(0.f,1.f,0.f)))) + -1.f * float(int(nothing(0.f,-1.f,0.f))));
    otherBlocks.z = 1.f * float(int(nothing(0.f,0.f,1.f)))) + -1.f * float(int(nothing(0.f,0.f,-1.f))));

    vec3 away = otherBlocks;
    return normalize(away);
}
 */

VoxelShape calculateNormals(VoxelShape& vox)
{
    auto start = std::chrono::high_resolution_clock::now();

    VoxelShape result = VoxelShape(vox.xSize, vox.ySize, vox.zSize);
    uint8_t obx;
    uint8_t oby;
    uint8_t obz;
    for (int x = 0; x < vox.xSize; x++)
    {
        for (int y = 0; y < vox.ySize; y++)
        {
            for (int z = 0; z < vox.zSize; z++)
            {
                if (vox.anyPixel(x,y,z))
                {
                    obx = 1.f * !vox.anyPixel(x+1.f,y+0.f,z+0.f);// + -1.f * vox.anyPixel(x+1.f,y+0.f,z+0.f);
                    oby = 1.f * !vox.anyPixel(x+0.f,y+1.f,z+0.f);// + -1.f * vox.anyPixel(x+0.f,y+1.f,z+0.f);
                    obz = 1.f * !vox.anyPixel(x+0.f,y+0.f,z+1.f);// + -1.f * vox.anyPixel(x+0.f,y+0.f,z+1.f);

                    if (obx + oby + obz == 0)
                    {
                        if (!vox.anyPixel(x+1.f,y+1.f,z+0.f))
                        {
                            //otherBlocks = vec3(1.f,1.f,0.f);
                            obx = 1.f;
                            oby = 1.f;
                        }
                        else if (!vox.anyPixel(x+0.f,y+1.f,z+1.f))
                        {
                            //otherBlocks = vec3(0.f,1.f,1.f);
                            oby = 1.f;
                            obz = 1.f;
                        }
                        else if (!vox.anyPixel(x+1.f,y+0.f,z+1.f))
                        {
                            obx = 1.f;
                        }
                    }

                    result.setPixel(x,y,z,Pixel((obx*127.5)+127.5,(oby*127.5)+127.5,(obz*127.5)+127.5));
                }

            }
        }
    }

    auto end = std::chrono::high_resolution_clock::now(); // Get the current time again

    // Calculate the time difference
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Normal generated in " << duration.count() << " milliseconds" << std::endl;

    return result;
}