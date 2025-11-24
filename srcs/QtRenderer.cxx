//
// Created by plouvel on 24/11/2025.
//

#include "QtRenderer.hxx"

QtRenderer::QtRenderer(QObject* parent) : QObject(parent) {}

void QtRenderer::setPixel(const Graphics::Tile::ColorIndex& colorIndex, const size_t x, const size_t y) noexcept
{
    const auto color{colorIndex.to_ulong()};
    uint32_t   pixelColor{};

    if (color == 0b11)
    {
        pixelColor = qRgb(0x00, 0x00, 0x00);
    }
    else if (color == 0b10)
    {
        pixelColor = qRgb(0x55, 0x55, 0x55);
    }
    else if (color == 0b01)
    {
        pixelColor = qRgb(0xAA, 0xAA, 0xAA);
    }
    else if (color == 0b00)
    {
        pixelColor = qRgb(0xFF, 0xFF, 0xFF);
    }

    _framebuffer[y * 160 + x] = pixelColor;
}

QImage QtRenderer::render() const
{
    QImage image{reinterpret_cast<const uchar*>(_framebuffer.data()), 160, 144, QImage::Format_RGB32};

    return image;
}