#include <vector>

struct RGBA {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;

    RGBA() : r(0), g(0), b(0), a(255) {}
    RGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : r(r), g(g), b(b), a(a) {}
    RGBA(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b), a(255) {}
};

class VoxelShape {
public:
    VoxelShape(int xSize, int ySize, int zSize)
            : xSize(xSize), ySize(ySize), zSize(zSize) {
        data = new RGBA[xSize * ySize * zSize];
        for (int i = 0; i < xSize * ySize * zSize; ++i) {
            data[i] = RGBA{ 0, 0, 0, 0 };
        }
    }

    ~VoxelShape() {
        delete[] data;
    }

    void setPixel(int x, int y, int z, RGBA rgba) {
        data[index(x, y, z)] = rgba;
    }

    RGBA getPixel(int x, int y, int z) {
        return data[index(x, y, z)];
    }

public:
    int xSize;
    int ySize;
    int zSize;
    RGBA* data;

    int index(int x, int y, int z) {
        return x + y * xSize + z * xSize * ySize;
    }
};