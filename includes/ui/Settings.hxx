//
// Created by plouvel on 12/2/25.
//

#ifndef GBEMU_SETTINGS_HXX
#define GBEMU_SETTINGS_HXX
#include <qkeysequence.h>

#include <QColor>
#include <cstdint>

#include "Common.hxx"

namespace Settings
{
    Q_NAMESPACE

    namespace Palette
    {
        Q_NAMESPACE

        enum class Type : uint8_t
        {
            Color0,
            Color1,
            Color2,
            Color3
        };

        constexpr QColor DEFAULT_COLORS[] = {
            QColor{0xE0, 0xF8, 0xD8},  // White
            QColor{0x88, 0xC0, 0x70},  // Light Grey
            QColor{0x34, 0x68, 0x56},  // Dark Grey
            QColor{0x08, 0x18, 0x20}   // Black
        };

        void   set(Type type, const QColor& color);
        QColor get(Type type);
    }  // namespace Palette

    namespace Keys
    {
        Q_NAMESPACE

        inline QKeySequence DEFAULT_KEY_SEQUENCES[] = {
            QKeySequence{"Z"},          // A
            QKeySequence{"X"},          // B
            QKeySequence{"Backspace"},  // Select
            QKeySequence{"Enter"},      // Start
            QKeySequence{"Right"},      // Right
            QKeySequence{"Left"},       // Left
            QKeySequence{"Up"},         // Up
            QKeySequence{"Down"},       // Down
        };

        void         set(Key key, const QKeySequence& sequence);
        QKeySequence get(Key key);
    }  // namespace Keys

    bool isBootRomEnabled();
    void setBootRomEnabled(bool enabled);

    QString getBootRomPath();
    void    setBootRomPath(const QString& path);

    namespace Graphics
    {
        bool isWindowEnabled();
        bool isBackgroundEnabled();
        bool areObjectsEnabled();
        bool useFastTransformation();
        bool keepAspectRatio();

        void setWindowEnabled(bool enabled);
        void setBackgroundEnabled(bool enabled);
        void setObjectsEnabled(bool enabled);
        void setFastTransformation(bool enabled);
        void setKeepAspectRatio(bool enabled);

    }  // namespace Graphics

}  // namespace Settings

#endif  // GBEMU_SETTINGS_HXX
