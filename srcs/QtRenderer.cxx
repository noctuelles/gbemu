#include "QtRenderer.hxx"

QtRenderer::QtRenderer(QObject* parent) : QObject(parent) {}

void QtRenderer::setPixel(const uint8_t x, const uint8_t y, const uint8_t pixel) noexcept
{
    _framebuffer[y][x] = pixel;
}

void QtRenderer::render()
{
    emit onRender(_framebuffer);
}

const Graphics::Framebuffer& QtRenderer::getFramebuffer() const
{
    return _framebuffer;
}