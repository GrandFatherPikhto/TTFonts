#include "fontcharactersmodel.h"
#include "unicodemetadatamanager.h"
#include "common.h"

FontCharactersModel::FontCharactersModel(QObject *parent)
    : QAbstractItemModel(parent)
    , m_scriptFilter(FONT_METADATA_DEFAULT_ID)
    , m_categoryFilter(FONT_METADATA_DEFAULT_ID)
{}

QVariant FontCharactersModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        switch (section) {
            case 0: return "Selected";
            case 1: return "Unicode";
            case 2: return "Character";
            case 3: return "Script";
            case 4: return "Category";
            case 5: return "Decomposition";
            case 6: return "HTML";
        }
    }

    return QVariant();
}

QModelIndex FontCharactersModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    // Для плоских таблиц (без иерархии) просто создаем индекс
    return createIndex(row, column);
}


QModelIndex FontCharactersModel::parent(const QModelIndex &child) const
{
    Q_UNUSED(child);
    return QModelIndex(); // Плоская модель без родительских элементов
}

int FontCharactersModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_characters.size();
}

int FontCharactersModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return 6;
}

bool FontCharactersModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (index.column() == 0 && role == Qt::CheckStateRole) {
        m_characters[index.row()].second = value.toBool();
        emit dataChanged(index, index, {Qt::CheckStateRole});
        return true;
    }
    return false;
}

QVariant FontCharactersModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_characters.size())
        return QVariant();

    const QChar ch = m_characters.at(index.row()).first;
    bool selected = m_characters.at(index.row()).second;

    switch (index.column()) {
        case 0: // Выбрать символ для обработки
            if (role == Qt::DisplayRole || role == Qt::EditRole)
                return QVariant(); // Пустое значение для чекбокса
            if (role == Qt::CheckStateRole)
                return  selected ? Qt::Checked :  Qt::Unchecked;
            break;
        case 1: // Unicode код
            if (role == Qt::DisplayRole)
            {
                return QString("U+%1").arg(int(ch.unicode()), 4, 16, QLatin1Char('0')).toUpper();
            }
            break;

        case 2: // Символ
            if (role == Qt::DisplayRole || role == Qt::EditRole)
                return QString(ch);
            if (role == Qt::FontRole)
            {
                return m_font;
            }
            if (role == Qt::TextAlignmentRole)
                return Qt::AlignCenter;
            break;

        case 3: // Язык
            if (role == Qt::DisplayRole)
            {
                return ch.script() == QChar::Script_Unknown ? "Unknown" : UnicodeMetadataManager::instance().scriptName(ch.script());
            }
            break;
        case 4: // Категория символа
            if (role == Qt::DisplayRole)
                return QChar::category(ch.unicode()) == QChar::Other_NotAssigned ? "N/A" : UnicodeMetadataManager::instance().categoryName(ch.category());
            break;

        case 5:
            if (role == Qt::DisplayRole)
            {
                return QVariant(ch.decomposition());
            }
            break;

        case 6: // HTML-код
            if (role == Qt::DisplayRole)
                return QString("&#%1;").arg(ch.unicode());
            break;
    }

    return QVariant();
}

// Добавим вспомогательный метод для получения символа по индексу
QChar FontCharactersModel::characterAt(const QModelIndex &index) const {
    if (!index.isValid() || index.row() >= m_characters.size())
        return QChar();

    return m_characters.at(index.row()).first;
}

Qt::ItemFlags FontCharactersModel::flags(const QModelIndex &index) const {
    Qt::ItemFlags flags = QAbstractItemModel::flags(index);

    if (index.column() == 0) {
        flags |= Qt::ItemIsUserCheckable | Qt::ItemIsEnabled;
    }

    return flags;
}

void FontCharactersModel::setFont(const QFont &font) {
    m_font = font;
    fillCharList();
    filterCharList();
}

void FontCharactersModel::filterCharList()
{
    beginResetModel();
    m_characters.clear();
    for (QVector<QChar>::Iterator it = m_fontCharacters.begin(); it != m_fontCharacters.end(); it ++) {
        if ((m_scriptFilter == FONT_METADATA_DEFAULT_ID || m_scriptFilter == it->script()) &&
            (m_categoryFilter == FONT_METADATA_DEFAULT_ID || m_categoryFilter == it->category()))
        {
            m_characters.append({*it, false});
        }
    }
    endResetModel();
}

void FontCharactersModel::fillCharList ()
{
    m_characters.clear();

    QFontMetrics fm(m_font);
    for (uint32_t code = 32; code < 65535; ++code) { // Пропускаем управляющие символы
        QChar ch(code);
        if (fm.inFont(ch) && ch.category() != QChar::Other_NotAssigned)
        {
            QRect bbox = fm.boundingRect(ch);
            if (bbox.width() > 1 && bbox.height() > 1) {
                m_fontCharacters.append(ch);
                if (!m_categories.contains(ch.category()))
                {
                    m_categories.append(ch.category());
                }

                if (!m_scripts.contains(ch.script()))
                {
                    m_scripts.append(ch.script());
                }

                // if (!m_decompositions.contains(ch.decomposition()))
                // {
                //     m_decompositions.append(ch.decompositionTag());
                // }

            }
        }
        // QChar::Other_NotAssigned
        if (fm.inFont(ch)) {
            // qDebug() << ch;

        }
    }

    qDebug() << "Added " << m_fontCharacters.length() << " chars";

    emit categoriesChanged(m_categories);
    emit scriptsChanged(m_scripts);
    emit decompositionTagsChanged(m_decompositions);
}
