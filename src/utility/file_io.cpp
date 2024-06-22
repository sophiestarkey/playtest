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

    m_num_channels = (format == ImageFormat::USE_DEFAULT) ? channels_in_file : static_cast<int>(format);
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
    m_num_channels(std::exchange(rhs.m_num_channels, 0)),
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
        m_num_channels = std::exchange(rhs.m_num_channels, 0);
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

int Image::num_channels() const
{
    return m_num_channels;
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
