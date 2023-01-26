#include <vector>

class Pixel {
public:
    uint8_t r, g, b;
    Pixel(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b) {}
    Pixel() : r(0), g(0), b(0) {}
};

class VoxelShape {
public:
    int xSize, ySize, zSize;
    std::vector<Pixel> data;
    VoxelShape(int x, int y, int z) : xSize(x), ySize(y), zSize(z), data(x*y*z)
    {
        for (int i = 0; i < x; i++) {
            for (int j = 0; j < y; j++) {
                for (int k = 0; k < z; k++) {
                    uint8_t r = 255;
                    uint8_t g = 0;
                    uint8_t b = 0;
                    setPixel(i, j, k, Pixel(r, g, b));
                }
            }
        }
    }
    void setPixel(int x, int y, int z, Pixel pixel)
    {
        data[x + y * this->xSize + z * this->xSize * this->ySize] = pixel;
    }

    Pixel getPixel(int x, int y, int z) const
    {
        return data[x + y * this->xSize + z * this->xSize * this->ySize];
    }
};