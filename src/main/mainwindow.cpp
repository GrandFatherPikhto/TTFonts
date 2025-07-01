#include <QRegularExpressionValidator>
#include <QRegularExpression>

#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "unicodemetadatamanager.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_fontCharacterModel(new FontCharactersModel(this))
    , m_decompositionsModel(new FontMetadataModel(this))
    , m_categoriesModel(new FontMetadataModel(this))
    , m_scriptsModel(new FontMetadataModel(this))
{
    ui->setupUi(this);
    ui->tableCharView->setModel(m_fontCharacterModel);
    // Дополнительные настройки таблицы
    ui->tableCharView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableCharView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableCharView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    initScriptComboBox();
    initCategoriesComboBox();
    initDecompositionsComboBox();
    initMSBFilter();

    // Включаем выделение строк
    ui->tableCharView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableCharView->setSelectionMode(QAbstractItemView::SingleSelection);

    // В классе MainWindow
    connect(ui->tableCharView, &QTableView::clicked, [this](const QModelIndex &index) {
        QChar ch = m_fontCharacterModel->characterAt(index);
        ui->glyphWidget->setGlyph(ch, m_fontCharacterModel->font());
        qDebug() << "Character " << ch;
    });
}

MainWindow::~MainWindow()
{
    delete m_decompositionsModel;
    delete m_categoriesModel;
    delete m_scriptsModel;
    delete m_fontCharacterModel;
    delete ui;
}

void MainWindow::on_actionImport_Font_triggered()
{
    bool ok;
    // QFont font = QFontDialog::getFont(&ok, nullptr, nullptr, "Выберите шрифт");
    QFont font = QFontDialog::getFont(&ok, nullptr);
    if (ok) {
        qDebug() << "Font :" << font.family();
        // display->setFont(font);
        m_fontCharacterModel->setFont(font);

        // Автоматически подгоняем ширину столбцов
        ui->tableCharView->resizeColumnsToContents();

        // Прокручиваем к началу
        ui->tableCharView->scrollToTop();
    }
}


void MainWindow::on_action_Quit_triggered()
{
    qApp->quit();
}

void MainWindow::onScriptsListChanged(QVector<quint32> scripts)
{
    ui->comboBoxFontScript->clear();
    ui->comboBoxFontScript->setEnabled(false);
    for (QVector<quint32>::Iterator it = scripts.begin(); it != scripts.end(); it ++) {
        m_scriptsModel->addItem(*it, UnicodeMetadataManager::instance().scriptName(static_cast<QChar::Script>(*it)));
    }
    ui->comboBoxFontScript->setEnabled(true);
}

void MainWindow::onCategoriesListChanged(QVector<quint32> scripts)
{
    ui->comboBoxFontCategory->clear();
    ui->comboBoxFontCategory->setEnabled(false);
    for (QVector<quint32>::Iterator it = scripts.begin(); it != scripts.end(); it ++) {
        m_categoriesModel->addItem(*it, UnicodeMetadataManager::instance().categoryName(static_cast<QChar::Category>(*it)));
    }
    ui->comboBoxFontCategory->setEnabled(true);
}


void MainWindow::initDecompositionsComboBox()
{

}

void MainWindow::initCategoriesComboBox()
{
    ui->comboBoxFontCategory->setModel(m_categoriesModel);
    QObject::connect(m_fontCharacterModel, &FontCharactersModel::categoriesChanged, this, &MainWindow::onCategoriesListChanged);
    // QObject::connect(this, &MainWindow::onCategoriesListChanged, this, &MainWindow::on_categories_List_Changed);
    connect(ui->comboBoxFontCategory, QOverload<int>::of(&QComboBox::currentIndexChanged),
        [=](int index) {
            if (index < 0) return;

            // qDebug() << index;
            // Получаем данные из модели
            QVariant data = ui->comboBoxFontCategory->itemData(index);  // UserRole по умолчанию
            QString text = ui->comboBoxFontCategory->itemText(index);

            m_fontCharacterModel->setCategoryFilter(data.toUInt());

            // Обработка выбора
            qDebug() << "Select:" << text << ", data:" << data;
    });
}

void MainWindow::initMSBFilter()
{
    QRegularExpressionValidator *v = new QRegularExpressionValidator(QRegularExpression("[\\da-fA-F]{1,2}"), ui->lineEditMSBFilter);
    ui->lineEditMSBFilter->setValidator(v);
    QObject::connect(ui->lineEditMSBFilter, &QLineEdit::textChanged, [=](){
        bool ok;
        QString text = ui->lineEditMSBFilter->text();
        int hexValue = ui->lineEditMSBFilter->text().toInt(&ok, 16);  // toInt с основанием 16
        ui->lineEditMSBFilter->setText(text.toUpper());
        if (ok) {
            ui->lineEditMSBFilter->setStyleSheet("background-color: white;");
            if (text.isEmpty()) {
                emit m_fontCharacterModel->setUnicodeMSB(-1);
            } else {
                emit m_fontCharacterModel->setUnicodeMSB(hexValue);
            }
        } else {
            ui->lineEditMSBFilter->setStyleSheet("background-color: #FFDDDD;");
        }
    });
}

void MainWindow::initScriptComboBox ()
{
    ui->comboBoxFontScript->setModel(m_scriptsModel);
    QObject::connect(m_fontCharacterModel, &FontCharactersModel::scriptsChanged, this, &MainWindow::onScriptsListChanged);

    connect(ui->comboBoxFontScript, QOverload<int>::of(&QComboBox::currentIndexChanged),
        [=](int index) {
            if (index < 0) return;

            // qDebug() << index;
            // Получаем данные из модели
            QVariant data = ui->comboBoxFontScript->itemData(index);  // UserRole по умолчанию
            QString text = ui->comboBoxFontScript->itemText(index);

            m_fontCharacterModel->setScriptFilter(data.toUInt());

            // Обработка выбора
            qDebug() << "Select char script: " << text << ", data:" << data;
    });
}

