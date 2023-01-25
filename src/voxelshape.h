class Pixel {
public:
    char r, g, b;
    Pixel(char r, char g, char b) : r(r), g(g), b(b) {}
    Pixel() : r(0), g(0), b(0) {}
};

class VoxelShape {
public:
    int xSize, ySize, zSize;
    Pixel* data;
    VoxelShape(int x, int y, int z) : xSize(x), ySize(y), zSize(z)
    {
        data = new Pixel[x * y * z];
        for (int i = 0; i < x; i++) {
            for (int j = 0; j < y; j++) {
                for (int k = 0; k < z; k++) {
                    char r = (i * 255 / x);
                    char g = (j * 255 / y);
                    char b = (k * 255 / z);
                    setPixel(i, j, k, Pixel(r, g, b));
                }
            }
        }
    }
    void setPixel(int x, int y, int z, Pixel pixel) const
    {
        data[x + y * this->xSize + z * this->xSize * this->ySize] = pixel;
    }

    Pixel getPixel(int x, int y, int z) const
    {
        return data[x + y * this->xSize + z * this->xSize * this->ySize];
    }
};