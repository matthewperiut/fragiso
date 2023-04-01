#pragma once

struct framebuffer
{
    unsigned int width, height;

    unsigned int texture;
    unsigned int handle;

    framebuffer();
    framebuffer(int width, int height);
    void create(int width, int height);
    void regen();
};