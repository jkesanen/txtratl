#pragma once

#include "zupply.hpp"

class Image
{
public:
    explicit Image(const std::string& filename)
        : mImage(filename.c_str())
    {}

    explicit Image(size_t rows, size_t cols, size_t channels)
        : mImage(static_cast<int>(rows), static_cast<int>(cols), static_cast<int>(channels))
    {}

    ~Image();

    unsigned char* ptr(size_t row, size_t col, size_t channel) const;

    size_t channels() const;

    size_t cols() const;

    size_t rows() const;

    void save(const std::string filename, size_t quality) const;

    void release();

private:
    zz::Image mImage;
};
