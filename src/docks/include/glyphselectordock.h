#ifndef GLYPHSELECTORDOCK_H
#define GLYPHSELECTORDOCK_H

#include <QObject>
#include <QWidget>

#include <QVBoxLayout>

#include <QDockWidget>
#include <QTableView>

#include <QLineEdit>
#include <QComboBox>
#include <QDockWidget>
#include <QSplitter>
#include <QListWidget>
#include <QSpinBox>
#include <QListView>

#include <QHideEvent>

#include <QVector>

#include <QSettings>

#include "fontcharactersmodel.h"
// #include "categoriesmodel.h"
#include "fontcharactersmodel.h"
#include "fontmetadatamodel.h"
//#include "unicodemetadatamanager.h"


class GlyphSelectorDock : public QDockWidget {
    Q_OBJECT
public:
    explicit GlyphSelectorDock(QWidget *parent = nullptr);
    
    // void setFont(const Font& font); // Установка текущего шрифта
    
signals:
    // void glyphSelected(uint glyphIndex); // Сигнал о выборе символа
    // void filterChanged(const QString& filter); // Сигнал об изменении фильтра
    void changedCharacter (const QChar &character);

public slots:
    void setFont(const QFont &font);
    void saveDockState();

    //void applyFilters(); // Применение фильтров
private slots:
    //void applyFilters(); // Применение фильтров

    // QWidget interface
protected:
    void closeEvent(QCloseEvent *event) override;
    void hideEvent(QHideEvent *event) override;

private:
    void saveWindowState ();
    void restoreWindowState ();

    void setupUI();
    void setupFontCategoryList ();
    void setupFontScriptList ();
    void setupFontMSBSelector ();
    void setupGlyphTable ();

    void fillCategories (const QVector<quint32> &categories);
    void fillScripts(const QVector<quint32> &scripts);
    
    QWidget     *m_mainWidget;
    QVBoxLayout *m_mainLayout;
    QSplitter   *m_mainSplitter;

    QSplitter   *m_glyphSplitter;
    QListView   *m_fontCategoryList;
    QListView   *m_fontScriptList;
    // QSpinBox    *m_fontMSB;
    QLineEdit   *m_fontMSB;
    QTableView  *m_fontCharsTable;


    FontCharactersModel *m_fontCharacterModel;

    FontMetadataModel *m_scriptsModel;
    FontMetadataModel *m_categoriesModel;
    FontMetadataModel *m_decompositionsModel;

    QString m_fontPath;
};

#endif // GLYPHSELECTORDOCK_H
