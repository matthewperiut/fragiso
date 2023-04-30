//
// Created by Matthew Periut on 4/8/23.
//

#include <iostream>
#include <chrono>
#include "normal.h"
#include "voxelshape.h"

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
                        else if (!vox.anyPixel(x+1.f,y+1.f,z+1.f))
                        {
                            obx = 1.f;
                            oby = 1.f;
                            obz = 1.f;
                        }
                    }

                    result.setPixel(x,y,z,Pixel(obx*255,oby*255,obz*255));
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