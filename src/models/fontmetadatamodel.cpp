#include "fontmetadatamodel.h"
#include "common.h"

FontMetadataModel::FontMetadataModel(QObject *parent)
    : QAbstractListModel{parent}
{
    addDefaultItem();
}

int FontMetadataModel::rowCount(const QModelIndex &parent) const
{
    return m_items.size();
}

QVariant FontMetadataModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_items.size())
        return QVariant();

    const auto& script = m_items[index.row()];

    // qDebug() << script.first << script.second;

    switch(role) {
        case Qt::DisplayRole:
            return script.second; // Название
        case Qt::EditRole:  // Добавьте эту строку
            return script.second; // Название
        case Qt::UserRole:
            return script.first;  // ID
        default:
            return QVariant();
    }
}

void FontMetadataModel::addItem(quint32 id, const QString &name)
{
    if (!m_items.contains({id, name})) {
        addDefaultItem ();
        beginInsertRows(QModelIndex(), m_items.size(), m_items.size());
        m_items.append({id, name});
        endInsertRows ();
        sortItems ();
    }
}

void FontMetadataModel::sortItems() {
    // Сортировка: "All" всегда первая, остальные по алфавиту
    std::stable_sort(m_items.begin(), m_items.end(),
                [](const QPair<quint32, QString>& a, const QPair<quint32, QString>& b) {
                    if (a.second == FONT_METADATA_DEFAULT_NAME)
                     return true; // a наверху
                    if (b.second == FONT_METADATA_DEFAULT_NAME)
                     return false; // b наверху
            return a.second.compare(b.second, Qt::CaseInsensitive) < 0;
        }
    );
}


void FontMetadataModel::addDefaultItem () 
{
    if (!m_items.contains({FONT_METADATA_DEFAULT_ID, FONT_METADATA_DEFAULT_NAME})) {
        m_items.append({FONT_METADATA_DEFAULT_ID, FONT_METADATA_DEFAULT_NAME});
    }
}


void FontMetadataModel::clearItemList()
{
    beginResetModel();
    m_items.clear();
    endResetModel ();
}
