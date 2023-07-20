#include "Raytracer.hpp"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

//#include <SDL.h>

#include <iostream>
#include <iomanip>

using std::cout;
using std::endl;
using std::left;
using std::setw;

#define PRINT_CONFIG(name, val) cout << setw(10) << left << name << ':' << setw(10) << val << endl;

int main() {
    const char* filename = "output.bmp";
    const size_t width = 400;
    const size_t height = 225;

    cout << "Raytracer running with the following configuration" << endl;
    PRINT_CONFIG("Width", width);
    PRINT_CONFIG("Height", height);
    PRINT_CONFIG("Filename", filename);

    RayTracer raytracer(width, height);
    raytracer.Run();
    cout << "Raytracer successfully run, beginning to write to file..." << endl;

    // Write to file.
    auto bitmap = raytracer.GetBitmap();
    stbi_flip_vertically_on_write(true); // Bugs
    stbi_write_bmp(filename, width, height, 3, bitmap);
    cout << "Image written to file " << filename << '.' << endl;

    return EXIT_SUCCESS;
}