#include "atlas.hpp"
#include "zupply.hpp"

using namespace zz;

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " [directory]" << std::endl;
        return -1;
    }

    // Filter image files from the source directory
    const std::vector<const char*> patterns{"*.jpg", "*.jpeg", "*.png"};
    auto imageFiles = fs::Directory(argv[1], patterns, false);

    if (!imageFiles.size())
    {
        std::cerr << "Failed to find image files from " << argv[1] << std::endl;
        return -2;
    }

    Atlas a;

    for (auto file : imageFiles)
    {
        std::cout << "Adding image: " << file.relative_path() << std::endl;
        if (!a.addImage(file.relative_path()))
        {
            std::cerr << "Failed to add image: " << file.relative_path() << std::endl;
            return -3;
        }
    }

    if (!a.packImages())
    {
        std::cerr << "Failed to pack images." << std::endl;
        return -4;
    }

    if (!a.createAtlas("atlas.jpg", "atlas.txt"))
    {
        std::cerr << "Failed to create atlas image and metadata" << std::endl;
        return -5;
    }

    return 0;
}
