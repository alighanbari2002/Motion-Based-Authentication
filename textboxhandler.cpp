#include "textboxhandler.h"

TextBoxHandler::TextBoxHandler() {}

void TextBoxHandler::clicked(int current_pixelsz)
{
    TextBoxHandler::setpixelsz(++current_pixelsz);
}

int TextBoxHandler::pixelsz() const
{
    return m_pixelsz;
}

void TextBoxHandler::setpixelsz(int newPixelsz)
{
    if (m_pixelsz == newPixelsz)
        return;
    m_pixelsz = newPixelsz;
    emit pixelszChanged();
}
