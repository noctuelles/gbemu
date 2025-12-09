//
// Created by plouvel on 12/8/25.
//

#ifndef GBEMU_QTRENDERER_HXX
#define GBEMU_QTRENDERER_HXX
#include <QObject>

#include "IRenderer.hxx"
#include "graphics/Framebuffer.hxx"

class QtRenderer final : public QObject, public IRenderer
{
    Q_OBJECT

  public:
    explicit QtRenderer(QObject* parent = nullptr);

    void setPixel(uint8_t x, uint8_t y, uint8_t pixel) noexcept override;
    void render() override;

    [[nodiscard]] const Graphics::Framebuffer& getFramebuffer() const;

  signals:
    void onRender(const Graphics::Framebuffer& framebuffer);

  private:
    Graphics::Framebuffer _framebuffer{};
};

#endif  // GBEMU_QTRENDERER_HXX
