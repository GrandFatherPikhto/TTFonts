#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFontDialog>

#include "glyphwidget.h"

#include "glyphselectordock.h"

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

signals:
    void saveCurrentState ();

private:
    void closeEvent(QCloseEvent* event) override;
    void setupGlyphDock ();
    void setupGlyphWidget();
    void saveGeometryAndState ();
    void restoreGeometryAndState ();

    Ui::MainWindow *ui;

    GlyphSelectorDock *m_glyphSelectorDock;
    GlyphWidget *m_glyphWidget;
};
#endif // MAINWINDOW_H
