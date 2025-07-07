#include "fontmetadatamodel.h"
#include "common.h"

FontMetadataModel::FontMetadataModel(QObject *parent)
    : QAbstractListModel{parent}
{
#ifdef FONT_METADATA_DEFAULT_ITEM
    addDefaultItem();
#endif
}

int FontMetadataModel::rowCount(const QModelIndex &parent) const
{
    return m_items.size();
}

QVariant FontMetadataModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_items.size())
        return QVariant();

    const auto& item = m_items[index.row()];

    switch(role) {
        case Qt::DisplayRole:
            return item.name(); // Название
        case Qt::EditRole:  // Добавьте эту строку
            return item.name(); // Название
        case Qt::UserRole:
            return item.id();  // ID
        default:
            return QVariant();
    }
}

void FontMetadataModel::addItem(quint32 id, const QString &name)
{
    if (!m_items.contains(FontMetadataItem(id, name))) {
#ifdef FONT_METADATA_DEFAULT_ITEM
        addDefaultItem ();
#endif
        beginInsertRows(QModelIndex(), m_items.size(), m_items.size());
        m_items.append({id, name});
        endInsertRows ();
        sortItems ();
    }
}

void FontMetadataModel::sortItems() {
    // Сортировка: "All" всегда первая, остальные по алфавиту
    std::stable_sort(m_items.begin(), m_items.end(),
                [](const FontMetadataItem & a, const FontMetadataItem & b) {
#ifdef FONT_METADATA_DEFAULT_ITEM
                    // if (a.second == FONT_METADATA_DEFAULT_NAME)
                    //  return true; // a наверху
                    // if (b.second == FONT_METADATA_DEFAULT_NAME)
                    //  return false; // b наверху
#endif
                    return a.name().compare(b.name(), Qt::CaseInsensitive) < 0;
        }
    );
}

void FontMetadataModel::addDefaultItem () 
{
    if (!m_items.contains({FONT_METADATA_DEFAULT_ID, FONT_METADATA_DEFAULT_NAME})) {
        m_items.append({FONT_METADATA_DEFAULT_ID, FONT_METADATA_DEFAULT_NAME});
    }
}


void FontMetadataModel::clearItems()
{
    beginResetModel();
    m_items.clear();
    endResetModel ();
}
