//
// Created by plouvel on 12/2/25.
//

#include "ui/Settings.hxx"

#include <QSettings>

namespace Settings
{
    bool isBootRomEnabled()
    {
        return QSettings{}.value("preference/system/enableBootRom", false).toBool();
    }

    void setBootRomEnabled(const bool enabled)
    {
        QSettings{}.setValue("preference/system/enableBootRom", enabled);
    }

    QString getBootRomPath()
    {
        return QSettings{}.value("preference/system/bootRomPath").toString();
    }

    void setBootRomPath(const QString& path)
    {
        QSettings{}.setValue("preference/system/bootRomPath", path);
    }

    bool Graphics::isWindowEnabled()
    {
        return QSettings{}.value("preference/graphics/enableWindow", true).toBool();
    }

    bool Graphics::isBackgroundEnabled()
    {
        return QSettings{}.value("preference/graphics/enableBackground", true).toBool();
    }

    bool Graphics::areObjectsEnabled()
    {
        return QSettings{}.value("preference/graphics/enableObjects", true).toBool();
    }
    bool Graphics::useFastTransformation()
    {
        return QSettings{}.value("preference/graphics/useFastTransformation", true).toBool();
    }

    bool Graphics::keepAspectRatio()
    {
        return QSettings{}.value("preference/graphics/keepAspectRatio", false).toBool();
    }

    void Graphics::setWindowEnabled(const bool enabled)
    {
        QSettings{}.setValue("preference/graphics/enableWindow", enabled);
    }

    void Graphics::setBackgroundEnabled(const bool enabled)
    {
        QSettings{}.setValue("preference/graphics/enableBackground", enabled);
    }

    void Graphics::setObjectsEnabled(const bool enabled)
    {
        QSettings{}.setValue("preference/graphics/enableObjects", enabled);
    }

    void Graphics::setFastTransformation(const bool enabled)
    {
        QSettings{}.setValue("preference/graphics/useFastTransformation", enabled);
    }

    void Graphics::setKeepAspectRatio(const bool enabled)
    {
        QSettings{}.setValue("preference/graphics/keepAspectRatio", enabled);
    }

}  // namespace Settings

namespace Settings::Palette
{
    void set(const Type type, const QColor& color)
    {
        QSettings{}.setValue(QString{"preference/palette/color%1"}.arg(std::to_underlying(type)), color);
    }

    QColor get(const Type type)
    {
        const auto value{std::to_underlying(type)};

        return QSettings{}
            .value(QString{"preference/palette/color%1"}.arg(value), DEFAULT_COLORS[value])
            .value<QColor>();
    }
}  // namespace Settings::Palette

namespace Settings::Keys
{
    static QString keyToString(const Key key)
    {
        switch (key)
        {
            case Key::A:
                return QString{"a"};
            case Key::B:
                return QString{"b"};
            case Key::Select:
                return QString{"select"};
            case Key::Start:
                return QString{"start"};
            case Key::Right:
                return QString{"right"};
            case Key::Left:
                return QString{"left"};
            case Key::Up:
                return QString{"up"};
            case Key::Down:
                return QString{"down"};
            [[unlikely]] default:
                return QString{};
        }
    }

    void set(const Key key, const QKeySequence& sequence)
    {
        QSettings{}.setValue(QString{"preference/keys/%1"}.arg(keyToString(key)), sequence);
    }

    QKeySequence get(const Key key)
    {
        return QSettings{}
            .value(QString{"preference/keys/%1"}.arg(keyToString(key)), DEFAULT_KEY_SEQUENCES[std::to_underlying(key)])
            .value<QKeySequence>();
    }

}  // namespace Settings::Keys
