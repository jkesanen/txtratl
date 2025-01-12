#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

#include "txtratl/atlas.hpp"

namespace fs = std::filesystem;

std::vector<fs::path> getDirectoryFiles(fs::path directory, const std::vector<std::string> extensions)
{
    auto files = std::vector<fs::path>{};

    if (!fs::exists(directory) || !fs::is_directory(directory))
    {
        return files;
    }

    for (const auto& entry : fs::directory_iterator(directory))
    {
        if (fs::is_regular_file(entry.status()))
        {
            for (const auto& extension : extensions)
            {
                if (entry.path().extension() == extension)
                {
                    files.push_back(entry.path());
                    continue;
                }
            }
        }
    }

    return files;
}

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " [directory]" << std::endl;
        return -1;
    }

    const auto extensions = std::vector<std::string>{".jpg", ".png"};
    auto imageFiles = getDirectoryFiles(argv[1], extensions);

    if (!imageFiles.size())
    {
        std::cerr << "Failed to find image files from " << argv[1] << std::endl;
        return -2;
    }

    Atlas a;

    for (const auto& file : imageFiles)
    {
        std::cout << "Adding image: " << file.relative_path().string() << std::endl;
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

    if (!a.createAtlas("atlas.png", "atlas.txt"))
    {
        std::cerr << "Failed to create atlas image and metadata" << std::endl;
        return -5;
    }

    return 0;
}
