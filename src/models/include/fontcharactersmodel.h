#ifndef FONTCHARACTERSMODEL_H
#define FONTCHARACTERSMODEL_H

#include <QAbstractTableModel>
#include <QAbstractItemModel>
#include <QItemSelection>
#include <QFont>
#include <QFontMetrics>
#include <QChar>
#include <QString>
#include <QPair>

#include "fontcharitem.h"

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

    // void setFont(const QFont &font);

    QChar characterAt(const QModelIndex &index) const;

    void setCategoryFilter(const QVector<quint32> &categories) {
        QVector<quint32> temp = categories; // Создаем копию
        m_categoryFilter.swap(temp);
        fillCharList();
    }

    void setScriptFilter(const QVector<quint32> &scripts) {
        QVector<quint32> temp = scripts; // Создаем копию
        m_scriptFilter.swap(temp);
        fillCharList();
    }

    // Q_PROPERTY(QColor font MEMBER m_font NOTIFY fontChanged)

    QFont font() const;
    void setFont(const QFont &newFont);
    void resetFont();

public slots:
    void handleSelectionCategoriesChanged(const QItemSelection &selected, const QItemSelection &deselected);
    void handleSelectionScriptsChanged(const QItemSelection &selected, const QItemSelection &deselected);

signals:
    void categoriesChanged (QVector<quint32>);
    void scriptsChanged (QVector<quint32> ids);
    void decompositionTagsChanged (QVector<quint32> ids);
    void setUnicodeMSB(qint16 msb);


    void fontChanged();

private:
    void setUnicodeMSBFilter(qint16 msb);
    void fillCharList ();
    void filterCharList();

    QFont m_font;

    QVector<FontCharItem> m_characters;
    QVector<FontCharItem> m_fontCharacters;
    QVector<quint32> m_categories;
    QVector<quint32> m_decompositions;
    QVector<quint32> m_scripts;

    QVector<quint32> m_scriptFilter;
    QVector<quint32> m_categoryFilter;
    qint16  m_MSBFilter;
    Q_PROPERTY(QFont font READ font WRITE setFont RESET resetFont NOTIFY fontChanged FINAL)
};

#endif // FONTCHARACTERSMODEL_H
