#include <QGroupBox>

#include "glyphselectordock.h"

#include "unicodemetadatamanager.h"

#include "utils.h"

GlyphSelectorDock::GlyphSelectorDock(QWidget *parent) :
    QDockWidget("Glyph Selector", parent)
    , m_mainWidget(nullptr)
    , m_mainLayout(nullptr)
    , m_mainSplitter(nullptr)
    , m_fontCharacterModel(nullptr)
    , m_decompositionsModel(nullptr)
    , m_categoriesModel(nullptr)
    , m_scriptsModel(nullptr)
    , m_glyphSplitter(nullptr)
    , m_fontCategoryList(nullptr)
    , m_fontScriptList(nullptr)
    , m_fontMSB(nullptr)
    , m_fontCharsTable(nullptr)
{
    setObjectName("glyphSelectorDock");
    setupUI();

    restoreWindowState();
}

void GlyphSelectorDock::setupUI() {
    m_fontCharacterModel = new FontCharactersModel(this);
    m_decompositionsModel = new FontMetadataModel(this);
    m_categoriesModel = new FontMetadataModel(this);
    m_scriptsModel = new FontMetadataModel(this);

    QObject::connect(m_fontCharacterModel, &FontCharactersModel::categoriesChanged, this, &GlyphSelectorDock::fillCategories);
    QObject::connect(m_fontCharacterModel, &FontCharactersModel::scriptsChanged, this, &GlyphSelectorDock::fillScripts);

    m_mainWidget = new QWidget(this);
    m_mainLayout = new QVBoxLayout(m_mainWidget);
    m_mainSplitter = new QSplitter(Qt::Orientation::Vertical, m_mainWidget);

    setupFontCategoryList ();
    setupFontScriptList   ();
    setupFontMSBSelector  ();
    setupGlyphTable ();
    m_mainWidget->setLayout(m_mainLayout);
    m_mainLayout->addWidget(m_mainSplitter);
    setWidget(m_mainWidget);
}

void GlyphSelectorDock::setupFontCategoryList () {
    QGroupBox *categoryGroup = new QGroupBox("Категории", m_mainWidget);
    QVBoxLayout *categoryBoxLayout = new QVBoxLayout(categoryGroup);
    m_fontCategoryList = new QListView(categoryGroup);
    m_fontCategoryList->setModel(m_categoriesModel);
    // m_fontCategoryList->addItems({"Punctuation", "Numbers", "Currency"});
    categoryBoxLayout->addWidget(m_fontCategoryList);
    // categoryGroup->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    QObject::connect(m_fontCategoryList->selectionModel(), &QItemSelectionModel::selectionChanged, m_fontCharacterModel, &FontCharactersModel::handleSelectionCategoriesChanged);
    m_fontCategoryList->setSelectionMode(QAbstractItemView::MultiSelection);
    m_mainSplitter->addWidget(categoryGroup);
}

void GlyphSelectorDock::setupFontScriptList () {
    QGroupBox *scriptGroup = new QGroupBox("Языки", m_mainWidget);
    QVBoxLayout *scriptBoxLayout = new QVBoxLayout(scriptGroup);
    m_fontScriptList = new QListView(scriptGroup);
    m_fontScriptList->setModel(m_scriptsModel);
    // m_fontScriptList->addItems({"Латиница", "Кириллица", "Арабский", "Греческий", "Зулу"});
    QObject::connect(m_fontScriptList->selectionModel(), &QItemSelectionModel::selectionChanged, m_fontCharacterModel, &FontCharactersModel::handleSelectionScriptsChanged);
    m_fontScriptList->setSelectionMode(QAbstractItemView::MultiSelection);
    scriptBoxLayout->addWidget(m_fontScriptList);
#if 0
    connect(m_fontScriptList, QOverload<int>::of(&QListView::sele),
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
#endif
    m_mainSplitter->addWidget(scriptGroup);
}

void GlyphSelectorDock::setupFontMSBSelector () {
    QGroupBox *msbGroup = new QGroupBox("MSB", m_mainWidget);
    QVBoxLayout *msbBoxLayout = new QVBoxLayout(msbGroup);
    // m_fontMSB = new QSpinBox(msbGroup);
    // m_fontMSB->setRange(0, 255);
    m_fontMSB = new QLineEdit(msbGroup);
    QRegularExpressionValidator *v = new QRegularExpressionValidator(QRegularExpression("[\\da-fA-F]{1,2}"), m_fontMSB);
    m_fontMSB->setValidator(v);
    msbBoxLayout->addWidget(m_fontMSB);
    QObject::connect(m_fontMSB, &QLineEdit::textChanged, this, [=](){
        bool ok;
        QString text = m_fontMSB->text();
        int hexValue = m_fontMSB->text().toInt(&ok, 16);  // toInt с основанием 16
        m_fontMSB->setText(text.toUpper());
        if (ok) {
            m_fontMSB->setStyleSheet("background-color: white;");
            if (text.isEmpty()) {
                emit m_fontCharacterModel->setUnicodeMSB(-1);
            } else {
                emit m_fontCharacterModel->setUnicodeMSB(hexValue);
            }
        } else if (m_fontMSB->text().isEmpty()) {
            m_fontMSB->setStyleSheet("background-color: #333333;");
            emit m_fontCharacterModel->setUnicodeMSB(-1);
        } else {
            m_fontMSB->setStyleSheet("background-color: #FFDDDD;");
        }
    });

    m_mainSplitter->addWidget(msbGroup);
}

void GlyphSelectorDock::setupGlyphTable () {
    m_fontCharsTable = new QTableView(m_mainWidget);
    m_fontCharsTable->setModel(m_fontCharacterModel);
    m_mainSplitter->addWidget(m_fontCharsTable);
}

void GlyphSelectorDock::setFont(const QFont &font) {
    if (GetSystemFontFilePath(font.family(), m_fontPath)) {
         qDebug() << font.family() << m_fontPath;
    }
    // display->setFont(font);

    m_fontCharacterModel->setFont(font);

    // Автоматически подгоняем ширину столбцов
    m_fontCharsTable->resizeColumnsToContents();

    // Прокручиваем к началу
    m_fontCharsTable->scrollToTop();

    // В классе MainWindow
    connect(m_fontCharsTable, &QTableView::clicked, [this](const QModelIndex &index) {
        QChar ch = m_fontCharacterModel->characterAt(index);
        emit changedCharacter (ch);
        qDebug() << "Character " << ch;
    });
}

void GlyphSelectorDock::saveWindowState()
{
    QSettings settings("DAE", "TTFonts");
    // qDebug() << __FILE__ << __LINE__ << __FUNCTION__;

    // Сохраняем состояние QSplitter
    if (m_mainSplitter) {
        settings.setValue("splitter/state", m_mainSplitter->saveState());
        // qDebug() << "Save splitter state";
    }

    // Сохраняем состояние всех QDockWidget
    foreach (QDockWidget* dock, findChildren<QDockWidget*>()) {
        settings.setValue(QString("dockWidgets/%1/geometry").arg(dock->objectName()),
                          dock->saveGeometry());
        settings.setValue(QString("dockWidgets/%1/floating").arg(dock->objectName()),
                          dock->isFloating());
        settings.setValue(QString("dockWidgets/%1/visible").arg(dock->objectName()),
                          dock->isVisible());
    }
}

void GlyphSelectorDock::restoreWindowState()
{
    QSettings settings("DAE", "TTFonts");

    // Восстанавливаем QSplitter
    if (m_mainSplitter) {
        m_mainSplitter->restoreState(
            settings.value("splitter/state").toByteArray());
        qDebug() << "Restore splitter state";
    }

    // Восстанавливаем QDockWidget
    foreach (QDockWidget* dock, findChildren<QDockWidget*>()) {
        dock->restoreGeometry(
            settings.value(QString("dockWidgets/%1/geometry").arg(dock->objectName()))
                .toByteArray());

        bool floating = settings.value(
                                    QString("dockWidgets/%1/floating").arg(dock->objectName()), false).toBool();
        dock->setFloating(floating);

        bool visible = settings.value(
                                   QString("dockWidgets/%1/visible").arg(dock->objectName()), true).toBool();
        dock->setVisible(visible);
    }
}

void GlyphSelectorDock::fillCategories(const QVector<quint32> &categories) {
    m_categoriesModel->clearItems();
    m_fontCategoryList->setEnabled(false);
    for (QVector<quint32>::const_iterator it = categories.begin(); it != categories.end(); it ++) {
        m_categoriesModel->addItem(*it, UnicodeMetadataManager::instance().categoryName(static_cast<QChar::Category>(*it)));
        // qDebug() << *it << ", Name: " << UnicodeMetadataManager::instance().categoryName(static_cast<QChar::Category>(*it));
    }
    m_fontCategoryList->setEnabled(true);
}

void GlyphSelectorDock::fillScripts(const QVector<quint32> &scripts) {
    m_scriptsModel->clearItems();
    m_fontScriptList->setEnabled(false);
    for (QVector<quint32>::const_iterator it = scripts.begin(); it != scripts.end(); it ++) {
        m_scriptsModel->addItem(*it, UnicodeMetadataManager::instance().scriptName(static_cast<QChar::Script>(*it)));
        // qDebug() << *it << ", Name: " << UnicodeMetadataManager::instance().scriptName(static_cast<QChar::Script>(*it));
    }
    m_fontScriptList->setEnabled(true);
}

void GlyphSelectorDock::closeEvent(QCloseEvent *event)
{
    // qDebug() << __FILE__ << ":" << __LINE__ << __FUNCTION__;
    saveWindowState();
    QDockWidget::closeEvent(event);
}

void GlyphSelectorDock::hideEvent(QHideEvent *event)
{
    // qDebug() << __FILE__ << ":" << __LINE__ << __FUNCTION__;
    saveWindowState();
    QDockWidget::hideEvent(event);
}

void GlyphSelectorDock::saveDockState() {
    // qDebug() << __FILE__ << __LINE__ << __FUNCTION__;
    saveWindowState();
}
