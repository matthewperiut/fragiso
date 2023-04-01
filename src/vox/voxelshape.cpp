//
// Created by Matthew Periut on 3/31/23.
//
#include "voxelshape.h"

#define OGT_VOX_IMPLEMENTATION
#include "ogt_vox.h"

#include <GL/glew.h>
#include <iostream>
#include <fstream>
#include <string>

VoxelShape::VoxelShape(int xSize, int ySize, int zSize)
{
    create(xSize,ySize,zSize);
}

VoxelShape::VoxelShape(const char* filepath)
{
    loadMagica(filepath);
}

void VoxelShape::create(int xSize, int ySize, int zSize)
{
    this->xSize = xSize;
    this->ySize = ySize;
    this->zSize = zSize;

    data = std::make_unique<Pixel[]>(xSize * ySize * zSize);
    for (int i = 0; i < xSize * ySize * zSize; ++i) {
        data[i] = Pixel{255, 0, 0, 255 };
    }
}

int VoxelShape::index(int x, int y, int z) const
{
    return x + y * xSize + z * xSize * ySize;
}

void VoxelShape::setPixel(int x, int y, int z, Pixel pixel) const
{
    data[index(x, y, z)] = pixel;
}

Pixel VoxelShape::getPixel(int x, int y, int z) const
{
    return data[index(x, y, z)];
}

// external needed
void sendUniform3fSafely(GLuint program, std::string name, float x, float y, float z);
void sendUniform1fSafely(GLuint program, std::string name, float t);

void VoxelShape::send(unsigned int program) // to gpu
{
    sendUniform3fSafely(program, "voxelShapeSize", xSize, ySize, zSize);

    // Create the 3D texture
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_3D, textureID);

    glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA, xSize, ySize, zSize, 0, GL_RGBA, GL_UNSIGNED_BYTE, data.get());

    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);

    GLuint textureUnit = 0;
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(GL_TEXTURE_3D, textureID);

    // Bind the texture to a uniform sampler in the fragment shader
    GLint voxelShapeUniform = glGetUniformLocation(program, "voxelShape");
    glUniform1i(voxelShapeUniform, textureUnit);
}

void VoxelShape::loadMagica(const char* filepath)
{
    auto start = std::chrono::high_resolution_clock::now(); // Get the current time

    std::ifstream file(filepath, std::ios::binary | std::ios::ate);
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<char> buffer(size);
    if (file.read(buffer.data(), size))
    {
        /* worked! */
        const ogt_vox_scene* scene = ogt_vox_read_scene(reinterpret_cast<const uint8_t *>(buffer.data()), buffer.size());
        const ogt_vox_model* model = scene->models[0];

        create(model->size_y,model->size_z,model->size_x);

        for (int x = 0; x < model->size_x; x++)
        {
            for (int y = 0; y < model->size_y; y++)
            {
                for (int z = 0; z < model->size_z; z++)
                {
                    int voxel_index = x + (y * model->size_x) + (z * model->size_x * model->size_y);
                    uint8_t color_index = model->voxel_data[voxel_index];
                    ogt_vox_rgba color = scene->palette.color[color_index];
                    setPixel(y, z, x, Pixel(color.r, color.g, color.b, color.a));
                }
            }
        }

        auto end = std::chrono::high_resolution_clock::now(); // Get the current time again

        // Calculate the time difference
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "\"" << filepath << "\" loaded in " << duration.count() << " milliseconds" << std::endl;
    }
    else
    {
        std::cout << "Failed to load vox" << std::endl;
        exit(-1);
    }
}
