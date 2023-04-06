//
// Created by Matthew Periut on 3/31/23.
//
#include "voxelshape.h"

#define OGT_VOX_IMPLEMENTATION
#include "ogt_vox.h"

#include "../png/fpng.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../png/stb_image.h"

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

void VoxelShape::save(const char* filepath) // to gpu
{
    int horizontal_layers = floor(sqrt(ySize));
    int vertical_layers = ceil(sqrt(ySize)) + 1;

    int horizontal_pixels = horizontal_layers * xSize;
    int vertical_pixels = vertical_layers * zSize;
    int num_chans = 4;
    std::vector<uint8_t> pixels(horizontal_pixels * vertical_pixels * num_chans);

    for (int layer = 0; layer < ySize; layer++)
    {
        int horizontal_layer_offset = layer % horizontal_layers;
        int horizontal_pixel_offset = horizontal_layer_offset * xSize;
        int vertical_layer_offset = floor(layer / horizontal_layers);
        int vertical_pixel_offset = vertical_layer_offset * zSize;

        int& hpo = horizontal_pixel_offset;
        int& vpo = vertical_pixel_offset;

        for (int hp = 0; hp < xSize; hp++)
        {
            for (int vp = 0; vp < zSize; vp++)
            {
                Pixel pixel = getPixel(hp,layer,vp);
                uint32_t i = ((vp + vpo) * (horizontal_pixels) + (hp + hpo)) * num_chans;

                pixels[i] = pixel.r;        // red channel
                pixels[i + 1] = pixel.g;   // green channel
                pixels[i + 2] = pixel.b;   // blue channel
                pixels[i + 3] = pixel.a;   // alpha channel
            }
        }
    }

    bool success = fpng::fpng_encode_image_to_file(filepath, pixels.data(), horizontal_pixels, vertical_pixels, num_chans);
    if (!success)
    {
        std::cout << "encode fail" << std::endl;
    }
}

void VoxelShape::load(const char* filepath)
{
    std::vector<uint8_t> pixels;
    uint32_t width, height, num_chans;

    // Read the input PNG file
    int result = fpng::fpng_decode_file(filepath, pixels, width, height, num_chans, 4);
    if (result == fpng::FPNG_DECODE_NOT_FPNG)
    {
        int x, y, n;
        unsigned char* data = stbi_load(filepath, &x, &y, &n, 4);
        if (data == nullptr) {
            std::cout << "stb_image - failure to read " << filepath << std::endl;
        }

        // Copy the pixel data to the output vector
        pixels.assign(data, data + x * y * 4);
        width = x;
        height = y;
        num_chans = 4;

        // Free the stb_image memory
        stbi_image_free(data);
    }
    else if (result != fpng::FPNG_DECODE_SUCCESS)
    {
        std::cout << "fpng - failure to read " << filepath << std::endl;
        return;
    }

    // put resulting image into vox (todo)
}