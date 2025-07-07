#ifndef FONTMETADATAITEM_H
#define FONTMETADATAITEM_H

#include <QDebug>
#include <QString>
#include <QHash>

class FontMetadataItem
{
public:
    FontMetadataItem(quint32 id = 0, const QString& name = "");
    FontMetadataItem(const FontMetadataItem& item) = default;

    quint32 id() const { return m_id; }
    QString name() const { return m_name; }

    bool operator == (const FontMetadataItem & item) const {
        return (this->m_name == item.m_name && this->m_id == item.m_id);
    }


    bool contains(const FontMetadataItem &item) {
        return this->m_id == item.id() && this->name() == item.name();
    }

    friend QDebug operator<<(QDebug debug, const FontMetadataItem& item) {
        QDebugStateSaver saver(debug);
        debug.nospace();

        if (item.id() == 0 && item.name().isEmpty()) {
            debug << "FontMetadataItem(invalid)";
        } else {
            debug << "FontMetadataItem(id=" << item.id()
            << ", name=" << item.name();
        }

        return debug;
    }

private:
    quint32 m_id;
    QString m_name;
};

inline uint qHash(const FontMetadataItem &item, uint seed = 0) noexcept
{
    return qHashMulti(seed, item.id(), item.name());
}

#endif // FONTMETADATAITEM_H
