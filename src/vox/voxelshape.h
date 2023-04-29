#pragma once

#include <memory>
#include <vector>

struct Pixel {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;

    Pixel() : r(0), g(0), b(0), a(255) {}
    Pixel(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : r(r), g(g), b(b), a(a) {}
    Pixel(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b), a(255) {}
};

class VoxelShape {
private:
    // gpu texture handle
    unsigned int textureID = -1;
public:
    VoxelShape() = default;
    VoxelShape(int xSize, int ySize, int zSize);
    VoxelShape(const char* filepath);

    void create(int xSize, int ySize, int zSize);

    void setPixel(int x, int y, int z, Pixel pixel) const;
    void setAlpha(int x, int y, int z, uint8_t alpha) const;
    Pixel getPixel(int x, int y, int z) const;
    bool anyPixel(int x, int y, int z) const;
    bool colorPixelComp(int x, int y, int z, int value) const;

public:
    int xSize{};
    int ySize{};
    int zSize{};
    std::unique_ptr<Pixel[]> data;

    int index(int x, int y, int z) const;

    void loadMagica(const char* filepath);
    void send(unsigned int program, const char* name = "voxelShape");

    // png
    void save(const char* filepath);
    void load(const char* filepath);
};