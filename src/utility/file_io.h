#pragma once

#include <glm/vec2.hpp>
#include <stb/stb_image.h>
#include <string>
#include <vector>

enum class ImageFormat : int {
    USE_DEFAULT = 0,
    GREYSCALE = 1,
    GREYSCALE_ALPHA = 2,
    RGB = 3,
    RGBA = 4
};

class Image {
public:
    Image(const std::string& path, ImageFormat format = ImageFormat::USE_DEFAULT);
    ~Image();

    Image(const Image&) = delete;
    Image& operator=(const Image&) = delete;

    Image(Image&& rhs) noexcept;
    Image& operator=(Image&& rhs) noexcept;

    int width() const;
    int height() const;
    ImageFormat format() const;
    stbi_uc* data() const;
private:
    int m_width;
    int m_height;
    ImageFormat m_format;
    stbi_uc* m_data;
};

std::string ReadFile(const std::string& path);
