#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFontDialog>

#include "fontcharactersmodel.h"
#include "fontmetadatamodel.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionImport_Font_triggered();

    void on_action_Quit_triggered();

    void onScriptsListChanged (QVector<quint32> scripts);
    void onCategoriesListChanged (QVector<quint32> categories);


private:
    void initScriptComboBox ();
    void initDecompositionsComboBox ();
    void initCategoriesComboBox ();
    void initMSBFilter();

    Ui::MainWindow *ui;
    FontCharactersModel *m_fontCharacterModel;

    FontMetadataModel *m_scriptsModel;
    FontMetadataModel *m_categoriesModel;
    FontMetadataModel *m_decompositionsModel;
};
#endif // MAINWINDOW_H
