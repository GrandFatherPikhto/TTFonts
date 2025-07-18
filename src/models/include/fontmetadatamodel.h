#ifndef SCRIPTMODEL_H
#define SCRIPTMODEL_H

#include <QAbstractListModel>
#include <QObject>

#include "fontmetadataitem.h"

class FontMetadataModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit FontMetadataModel(QObject *parent = nullptr);

    // Переопределение методов модели
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    // Добавление данных
    void addItem(quint32 id, const QString& name);
    void clearItems ();

private:
    void sortItems();
    void addDefaultItem ();

    // QVector<QPair<quint32, QString>> m_items; // Хранит id и название
    QVector<FontMetadataItem> m_items;
};

#endif // SCRIPTMODEL_H
