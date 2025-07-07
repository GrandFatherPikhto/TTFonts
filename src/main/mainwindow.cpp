#include <QRegularExpressionValidator>
#include <QRegularExpression>
#include <QSplitter>
#include <QGroupBox>
#include <QSettings>
#include <QVBoxLayout>
#include <QSizePolicy>

#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_glyphSelectorDock(nullptr)
    , m_glyphWidget(nullptr)
{
    ui->setupUi(this);
    setupGlyphDock();
    setupGlyphWidget();
    // Восстановление геометрии при создании окна
    restoreGeometryAndState();

    QObject::connect(this, &MainWindow::saveCurrentState, m_glyphSelectorDock, &GlyphSelectorDock::saveDockState);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event) {
    // Сохранение геометрии перед закрытием
    saveGeometryAndState();
    emit saveCurrentState();
    QMainWindow::closeEvent(event);
}

void MainWindow::setupGlyphDock() {
    m_glyphSelectorDock = new GlyphSelectorDock(this);
    addDockWidget(Qt::LeftDockWidgetArea, m_glyphSelectorDock);
}

void MainWindow::setupGlyphWidget() {
    m_glyphWidget = new GlyphWidget(this);
    m_glyphWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QVBoxLayout *layout = new QVBoxLayout(ui->centralwidget);
    layout->addWidget(m_glyphWidget);
    ui->centralwidget->setLayout(layout);
    QObject::connect(m_glyphSelectorDock, &GlyphSelectorDock::changedCharacter,m_glyphWidget, &GlyphWidget::setCharacter);
}

void MainWindow::on_actionImport_Font_triggered()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok, nullptr);
    if (m_glyphSelectorDock && ok) {
        m_glyphWidget->setFont(font);
        m_glyphSelectorDock->setFont(font);
    }
}


void MainWindow::on_action_Quit_triggered()
{
    qApp->quit();
}

void MainWindow::saveGeometryAndState() {
    QSettings settings("DAE", "TTFonts");
    settings.setValue("mainWindowGeometry", saveGeometry());
    settings.setValue("mainWindowState", saveState());
}

void MainWindow::restoreGeometryAndState() {
    QSettings settings("DAE", "TTFonts");
    restoreGeometry(settings.value("mainWindowGeometry").toByteArray());
    restoreState(settings.value("mainWindowState").toByteArray());
}


