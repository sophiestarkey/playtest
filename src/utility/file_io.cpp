#include <fstream>
#include <sstream>
#include <stb/stb_image.h>
#include <utility>

#include "file_io.h"

Image::Image(const std::string& path, ImageFormat format)
{
    int channels_in_file;
    m_data = stbi_load(path.c_str(), &m_width, &m_height, &channels_in_file, static_cast<int>(format));

    if (!m_data) {
        throw std::runtime_error(std::string("Failed to load image '") + path + "': " + stbi_failure_reason());
    }

    if (format == ImageFormat::USE_DEFAULT) {
        m_format = static_cast<ImageFormat>(channels_in_file);
    } else {
        m_format = format;
    }
}

Image::~Image()
{
    if (m_data) {
        stbi_image_free(m_data);
    }
}

Image::Image(Image&& rhs) noexcept :
    m_width(std::exchange(rhs.m_width, 0)),
    m_height(std::exchange(rhs.m_height, 0)),
    m_format(std::exchange(rhs.m_format, ImageFormat::USE_DEFAULT)),
    m_data(std::exchange(rhs.m_data, nullptr))
{
}

Image& Image::operator=(Image&& rhs) noexcept
{
    if (this != &rhs) {
        if (m_data) {
            stbi_image_free(m_data);
        }

        m_width = std::exchange(rhs.m_width, 0);
        m_height = std::exchange(rhs.m_height, 0);
        m_format = std::exchange(rhs.m_format, ImageFormat::USE_DEFAULT);
        m_data = std::exchange(rhs.m_data, nullptr);
    }

    return *this;
}

int Image::width() const
{
    return m_width;
}

int Image::height() const
{
    return m_height;
}

ImageFormat Image::format() const
{
    return m_format;
}

stbi_uc* Image::data() const
{
    return m_data;
}

std::string ReadFile(const std::string& path)
{
    std::ifstream file(path);
    std::ostringstream stream;
    stream << file.rdbuf();
    return stream.str();
}
