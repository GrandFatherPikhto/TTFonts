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

void MainWindow::on_scripts_List_Changed(QVector<quint32> scripts)
{
    ui->comboBoxFontScript->clear();
    ui->comboBoxFontScript->setEnabled(false);
    for (QVector<quint32>::Iterator it = scripts.begin(); it != scripts.end(); it ++) {
        m_scriptsModel->addItem(*it, UnicodeMetadataManager::instance().scriptName(static_cast<QChar::Script>(*it)));
    }
    ui->comboBoxFontScript->setEnabled(true);
}

void MainWindow::on_categories_List_Changed(QVector<quint32> scripts)
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
    QObject::connect(m_fontCharacterModel, &FontCharactersModel::categoriesChanged, this, &MainWindow::on_categories_List_Changed);
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

void MainWindow::initScriptComboBox ()
{
    ui->comboBoxFontScript->setModel(m_scriptsModel);
    QObject::connect(m_fontCharacterModel, &FontCharactersModel::scriptsChanged, this, &MainWindow::on_scripts_List_Changed);

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

