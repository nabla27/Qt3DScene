#ifndef UTILITY_H
#define UTILITY_H
#include <QString>
#include <QStringList>
#include <QMetaEnum>
#include <QSize>
#include <QRect>

template <typename QEnum>
QString enumToString(const QEnum value) { return QString(QMetaEnum::fromType<QEnum>().valueToKey((int)value)); }

template <class QEnum>
QStringList enumToStrings(const QEnum)
{
    QStringList enumStrList;
    const int enumCount = QMetaEnum::fromType<QEnum>().keyCount();
    int count = 0;

    for(int i = 0; count < enumCount; ++i)
    {
        const QString name(QMetaEnum::fromType<QEnum>().valueToKey(i));
        if(!name.isEmpty())
        {
            enumStrList << name;
            ++count;
        }
    }

    return enumStrList;
}

template <class QEnum>
int getEnumCount(const QEnum){
    return QMetaEnum::fromType<QEnum>().keyCount();
}

template <class QEnum>
int getEnumIndex(const QString enumStr)
{
    const int enumCount = getEnumCount(QEnum(0));
    int count = 0;

    for(int i = 0; count < enumCount; ++i)
    {
        const QString name(QMetaEnum::fromType<QEnum>().valueToKey(i));
        if(!name.isEmpty())
        {
            if(name == enumStr) return i;
            ++count;
        }
    }

    return -1;
}

inline const QRect getRectFromScreenRatio(const QSize& screenSize, const float& rw, const float& rh){
    const float px = (1.0f - rw) * screenSize.width() / 2.0f;
    const float py = (1.0f - rh) * screenSize.height() / 2.0f;
    return QRect(px, py, screenSize.width() * rw, screenSize.height() * rh);
}










#include <QIcon>
struct IconSet
{
public:
    static void load();

    static QIcon mesh3d;
    static QIcon object3d;
    static QIcon surface3d;
    static QIcon transform3d;
    static QIcon animationgroup;

    static QIcon clone;
    static QIcon gear;
    static QIcon import;
    static QIcon play;
    static QIcon plus;
    static QIcon sort;
    static QIcon trashcan;
};









#endif // UTILITY_H
