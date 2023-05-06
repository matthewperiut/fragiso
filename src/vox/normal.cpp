//
// Created by Matthew Periut on 4/8/23.
//

#include <iostream>
#include <chrono>
#include <glm/glm.hpp>
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
                    glm::vec3 pos{ x,y,z };
                    glm::vec3 res{ 0, 0, 0 };

                    for (int i = -1; i < 2; i++)
                    {
                        for (int j = -1; j < 2; j++)
                        {
                            for (int k = -1; k < 2; k++)
                            {
                                if (!vox.anyPixel(pos.x+i, pos.y+j, pos.z+k))
                                {
                                    if (i == 0 || j == 0 || k == 0)
                                        res += glm::vec3(i,j,k);
                                }
                            }
                        }
                    }

                    res = glm::normalize(res);

                    result.setPixel(pos.x,pos.y,pos.z,Pixel((res.x*127.5)+127.5,(res.y*127.5)+127.5,(res.z*127.5)+127.5));

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