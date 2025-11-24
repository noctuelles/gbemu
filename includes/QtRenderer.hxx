//
// Created by plouvel on 23/11/2025.
//

#ifndef GBEMU_QTRENDERER_HXX
#define GBEMU_QTRENDERER_HXX
#include <QObject>
#include <QImage>

#include "IRenderer.hxx"

class QtRenderer final : public QObject, public IRenderer
{
    Q_OBJECT

  public:
    explicit QtRenderer(QObject* parent = nullptr);

    void setPixel(const Graphics::Tile::ColorIndex& colorIndex, size_t x, size_t y) noexcept override;

    [[nodiscard]] QImage render() const;

  signals:
    void frameReady() override;
private:
    std::array<uint32_t, 160 * 144> _framebuffer{};
};

#endif  // GBEMU_QTRENDERER_HXX
