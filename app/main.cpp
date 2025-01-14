#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

#include "txtratl/atlas.hpp"

namespace fs = std::filesystem;

std::vector<fs::path> getDirectoryFiles(const fs::path& directory, const std::vector<std::string>& extensions)
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
        return 0;
    }

    const auto extensions = std::vector<std::string>{".jpg", ".png"};
    auto imageFiles = getDirectoryFiles(argv[1], extensions);

    if (!imageFiles.size())
    {
        std::cerr << "Failed to find image files from " << argv[1] << std::endl;
        return -1;
    }

    txtratl::Atlas atlas;

    try
    {
        for (const auto& filepath : imageFiles)
        {
            std::cout << "Adding image: " << filepath.relative_path().string() << std::endl;
            atlas.addImage(filepath.relative_path());
        }

        if (!atlas.packImages())
        {
            std::cerr << "Failed to pack images." << std::endl;
            return -1;
        }

        if (!atlas.createAtlas("atlas.png", "atlas.txt"))
        {
            std::cerr << "Failed to create the atlas image and metadata" << std::endl;
            return -1;
        }
    }
    catch (std::exception const& e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
        return -1;
    }

    return 0;
}
