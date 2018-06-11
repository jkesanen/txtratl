#include <iostream>

#include "zupply.hpp"
using namespace zz;

int main(int argc, char** argv)
{
    /*
    Image img("demo.jpg");

    if (!img.empty())
    {
        for (int i = 0; i < img.cols(); i++)
        {
            img(i, 50, 0) = 160;
            std::cout << i << std::endl;
        }

        img.save("demo.png");
    }
	*/

    //Image canvas(720, 720, 4);
    /*
    std::vector<uint32_t> bigarray;
    bigarray.reserve(720 * 720);
    const uint32_t grey = 0x303030ff;
    for (unsigned int k = 0; k < (720 * 720); ++k)
    {
        bigarray.push_back(grey);
    }

    Image canvas;
    */
    //canvas.import(reinterpret_cast<unsigned char*>(bigarray.data()), 720, 720, 4);

    Image canvas(8192, 8192, 4);

    for (int y = 0; y < canvas.rows(); y++) {
        for (int x = 0; x < canvas.cols(); x++) {
            for (int c = 0; c < 4; c++) {
                canvas(x, y, c) = c < 3 ? 0x30 : 0xff;
            }
        }
    }

    int i = 0;
    while (true)
    {
        float angle = 0.1f * i;
        int x = canvas.cols() / 2 + (1 + angle) * cos(angle);
        int y = canvas.rows() / 2 + (1 + angle) * sin(angle);
        //std::cout << "x: " << x << ", y: " << y << std::endl;
        if (x < 1 || x >= canvas.cols() ||
            y < 1 || y >= canvas.rows())
            break;
        canvas(x, y, 0) = 0xcc;
        i++;
    }

    canvas.save("spiral.png");
    
    return 0;
}