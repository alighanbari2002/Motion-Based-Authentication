#ifndef TEXTBOXHANDLER_H
#define TEXTBOXHANDLER_H

#include <QObject>

class TextBoxHandler : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int pixelsz READ pixelsz WRITE setpixelsz NOTIFY pixelszChanged FINAL)


signals:

    void pixelszChanged();

public:
    TextBoxHandler();
    Q_INVOKABLE void clicked(int current_pixelsz);

    int pixelsz() const;
    void setpixelsz(int newPixelsz);

private:
    int m_pixelsz;
};

#endif // TEXTBOXHANDLER_H
