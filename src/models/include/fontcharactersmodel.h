#ifndef FONTCHARACTERSMODEL_H
#define FONTCHARACTERSMODEL_H

#include <QAbstractTableModel>
#include <QAbstractItemModel>
#include <QFont>
#include <QFontMetrics>
#include <QChar>
#include <QString>
#include <QPair>

class FontCharactersModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit FontCharactersModel(QObject *parent = nullptr);

    // Header:
    QVariant headerData(int section,
                        Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

    // Basic functionality:
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    void setFont(const QFont &font);

    QChar characterAt(const QModelIndex &index) const;

    void setCategoryFilter(quint32 filter) {
        m_categoryFilter = filter;
        filterCharList();
    }

    void setScriptFilter(quint32 filter) {
        m_scriptFilter = filter;
        filterCharList();
    }

signals:
    void categoriesChanged (QVector<quint32>);
    void scriptsChanged (QVector<quint32> ids);
    void decompositionTagsChanged (QVector<quint32> ids);

private:
    void filterCharList ();
    void fillCharList ();

    QFont m_font;
    QVector<QPair<QChar, bool>> m_characters;
    QVector<QChar> m_fontCharacters;
    QVector<quint32> m_categories;
    QVector<quint32> m_decompositions;
    QVector<quint32> m_scripts;

    quint32 m_scriptFilter;
    quint32 m_categoryFilter;
};

#endif // FONTCHARACTERSMODEL_H
