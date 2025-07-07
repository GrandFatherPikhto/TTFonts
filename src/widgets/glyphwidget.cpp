#include <QPainter>
#include <QMouseEvent>

#include "glyphwidget.h"
#include "utils.h"

#define TRUNC(x) ((x) >> 6)

GlyphWidget::GlyphWidget(QWidget *parent) :
    QWidget(parent)
    , m_gridSize(16)
    , m_gridRows(8)
    , m_gridColumns(8)
    , m_cellSize(20)
    , m_charSize(12)
    , m_fontPath("C:\\Windows\\Fonts\\arial.ttf")
    , m_character(QChar('M'))
    , m_glyphCols(0)
    , m_glyphRows(0)
    , m_library(nullptr)
    , m_face(nullptr)
    , m_ftError(FT_Err_Ok)
    , m_glyphUpdated(false)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setMinimumSize(400, 400);
    initFT();
    m_glyphPixels.clear();
}

GlyphWidget::~GlyphWidget() {
    FT_Done_Face(m_face);
    FT_Done_FreeType(m_library);
}

#if 0
void GlyphWidget::setGlyph(QChar character)
{
    m_character = character;
    qDebug() << "setGlyph(" << m_character << ", " << m_font << ")";
    loadFromChar();
    update();
}
#endif

void GlyphWidget::initFT()
{
    if (m_library != nullptr) {
        FT_Done_FreeType(m_library);
    }

    // For simplicity, the error handling is very rudimentary.
    m_ftError = FT_Init_FreeType(&m_library);
    if (!m_ftError)
    {
        qDebug() << "FT Library loaded";
    } else {
        qDebug() << "FT Load Library error";
    }
}

void GlyphWidget::loadFTFont(const QFont &font) {
    m_font = font;
    FT_Error error = FT_Err_Ok;
    if (m_face == nullptr) {
        QString fontPath;
        if (GetSystemFontFilePath(m_font.family(), fontPath)) {
            m_fontPath = fontPath;
        }
    }
}

void GlyphWidget::clearFT() {
    FT_Done_Face(m_face);
    m_face = nullptr;
}

void GlyphWidget::loadCharFT(const QChar &character, quint32 charSize) {
}


void GlyphWidget::convertToGlyphPixels () {
    QString line = "";
    FT_Bitmap* bitmap = &m_face->glyph->bitmap;
    qDebug() << "Columns: " << bitmap->width << ", Rows: " << bitmap->rows;
    m_glyphRows = bitmap->rows;
    m_glyphCols = bitmap->width;
    m_glyphPixels.clear();
    if (bitmap->pixel_mode == FT_PIXEL_MODE_MONO) {
        for (int y = 0; y < bitmap->rows; y++) {
            line.clear();
            for (int x = 0; x < bitmap->width; x++) {
                // Получаем байт, содержащий пиксель
                unsigned char byte = bitmap->buffer[y * bitmap->pitch + (x / 8)];
                // Извлекаем бит (MSB → x=0, LSB → x=7)
                unsigned char bit = (byte >> (7 - (x % 8))) & 0x1;
                // printf("%c", bit ? '#' : ' ');
                line.append(bit ? '#' : ' ');
                m_glyphPixels.append(bit ? true : false);
            }
            qDebug() << line;
        }
    }
    qDebug() << m_glyphPixels;
}


void GlyphWidget::setBackgroundColor(const QColor &color)
{
    m_bgColor = color;
    update();
}

void GlyphWidget::setGlyphColor(const QColor &color)
{
    m_glyphColor = color;
    update();
}

void GlyphWidget::setScaleFactor(float scale)
{
    m_scale = qBound(0.5f, scale, 5.0f); // Ограничиваем масштаб
    update();
}

void GlyphWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
#if 0
    QPainter painter(this);
    painter.fillRect(rect(), Qt::white);

    // Рисуем пиксели
    const int cellSize = 16;  // Размер ячейки в пикселях (для удобства)
    for (int y = 0; y < m_gridSize; ++y) {
        for (int x = 0; x < m_gridSize; ++x) {
            QRect rect(x * cellSize, y * cellSize, cellSize, cellSize);
            painter.fillRect(rect, m_pixels[y][x] ? m_pixelColor : Qt::white);
            painter.setPen(Qt::lightGray);
            painter.drawRect(rect);
        }
    }
#endif
    qDebug() << __FUNCTION__ << "GlyphWidget Width: " << this->width() << ", Height: " << this->height();
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, false);

    // Очищаем фон
    painter.fillRect(rect(), Qt::white);

    // Проверяем, что данные валидны
    if(m_glyphPixels.isEmpty() || m_glyphCols == 0 || m_glyphRows == 0) {
        qDebug() << "Glyph Pixels is empty";
        return;
    }

    // if (!m_glyphUpdated)
    //     return;

    // Рассчитываем начальную позицию для центрирования
    int startX = (width() - m_glyphCols * m_cellSize) / 2;
    int startY = (height() - m_glyphRows * m_cellSize) / 2;
    qDebug() << __FUNCTION__ << "Rows: " << m_glyphRows << ", Cols: " << m_glyphCols << ", Size: " << m_glyphPixels.length() << ", Cell: " << m_cellSize;
    // Отрисовываем каждый пиксель
    for(quint32 y = 0; y < m_glyphRows; ++y) {
        for(quint32 x = 0; x < m_glyphCols; ++x) {
            // Получаем значение пикселя (true - закрашен, false - пустой)
            bool pixel = m_glyphPixels[y * m_glyphCols + x];

            // Рассчитываем координаты квадрата с учетом масштаба
            QRect pixelRect(
                startX + x * m_cellSize,
                startY + y * m_cellSize,
                m_cellSize,
                m_cellSize
                );

            // Отрисовываем квадрат
            if(pixel) {
                painter.fillRect(pixelRect, Qt::black);
            } else {
                painter.fillRect(pixelRect, Qt::white);
                painter.setPen(Qt::lightGray);
                painter.drawRect(pixelRect);
            }
        }
    }

    painter.end();
}

void GlyphWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        emit clicked();
    }
    QWidget::mousePressEvent(event);
}

void GlyphWidget::wheelEvent(QWheelEvent *event)
{
    float delta = event->angleDelta().y() > 0 ? 0.1f : -0.1f;
    setScaleFactor(m_scale + delta);
}

void GlyphWidget::clearGlyph()
{

}

void GlyphWidget::loadFromChar()
{
    int scaleFactor = 10;
    // Очищаем текущие пиксели
    clearGlyph();

    // Создаем временное изображение для растеризации QImage::Format_Mono QImage::Format_Grayscale8
    QImage image(m_gridSize * scaleFactor, m_gridSize * scaleFactor, QImage::Format_Mono);
    image.fill(0); // Черный фон

    m_font.setPixelSize(m_gridSize * scaleFactor); // Важно для точного соответствия сетке

    // Рисуем символ на изображении
    QPainter painter(&image);

    painter.setFont(m_font);
    painter.setPen(Qt::color1); // Белый цвет для символа


    // Центрируем символ
    // QRectF boundingRect = painter.boundingRect(image.rect(), Qt::AlignCenter, m_character);
    painter.drawText(image.rect(), Qt::AlignCenter, m_character);
    painter.end();
    // Масштабируем обратно до 16x16
    image = image.scaled(m_gridSize, m_gridSize, Qt::IgnoreAspectRatio, Qt::FastTransformation);

    // Переносим пиксели в нашу сетку
    for (int y = 0; y < m_gridSize; ++y) {
        for (int x = 0; x < m_gridSize; ++x) {
            m_pixels[y][x] = image.pixelIndex(x, y) == 1;
            qDebug() << x << ", " << y << ", " << image.pixelIndex(x, y);
        }
    }
    update();
}

QChar GlyphWidget::character() const
{
    return m_character;
}

void GlyphWidget::setCharacter(const QChar &newCharacter)
{
    if (m_character == newCharacter)
        return;
    // update ();

    qDebug() << __FUNCTION__ << newCharacter << m_charSize;

    // error = FT_Set_Char_Size(m_face, 0, pointSize * 64, physicalDpiX(), physicalDpiY());
    m_ftError = FT_Set_Pixel_Sizes(m_face, 0, m_charSize);

    FT_UInt glyph_index = 0;
    glyph_index = FT_Get_Char_Index(m_face, newCharacter.unicode());

    qDebug() << "Glyph Index: " << glyph_index;

    qDebug() << "GlyphWidget:" << __LINE__ << "Width " << width() << ", Height " << height();

    m_ftError = FT_Load_Glyph(m_face, glyph_index, FT_LOAD_RENDER | FT_LOAD_TARGET_MONO);
    if (!m_ftError)
    {
        FT_Pos left = m_face->glyph->metrics.horiBearingX;
        FT_Pos right = left + m_face->glyph->metrics.width;
        FT_Pos top = m_face->glyph->metrics.horiBearingY;
        FT_Pos bottom = top - m_face->glyph->metrics.height;

        m_glyphRect = QRect(QPoint(TRUNC(left),
                                   -TRUNC(top) + 1),
                            QSize(TRUNC(right - left) + 1,
                                  TRUNC(top - bottom) + 1));
        // setFixedSize(m_glyphRect.width(),
        //              m_glyphRect.height());
        qDebug() << "Loaded Glyph " << newCharacter << m_face->glyph->bitmap.width << m_face->glyph->bitmap.rows;
        m_character = newCharacter;

        FT_Bitmap* bitmap = &m_face->glyph->bitmap;
        qDebug() << "Columns: " << bitmap->width << ", Rows: " << bitmap->rows;
        m_glyphRows = bitmap->rows;
        m_glyphCols = bitmap->width;
        m_glyphPixels.clear();
        if (bitmap->pixel_mode == FT_PIXEL_MODE_MONO) {
            for (int y = 0; y < bitmap->rows; y++) {
                for (int x = 0; x < bitmap->width; x++) {
                    // Получаем байт, содержащий пиксель
                    unsigned char byte = bitmap->buffer[y * bitmap->pitch + (x / 8)];
                    // Извлекаем бит (MSB → x=0, LSB → x=7)
                    unsigned char bit = (byte >> (7 - (x % 8))) & 0x1;
                    // printf("%c", bit ? '#' : ' ');
                    m_glyphPixels.append(bit ? true : false);
                }
            }
        }
    } else {
        qDebug() << "Error!";
    }

    qDebug() << "GlyphWidget:" << __LINE__ << "Width " << width() << ", Height " << height();
    // convertToGlyphPixels();
    update ();
    emit characterChanged(m_character);
}

void GlyphWidget::resetCharacter()
{
    setCharacter({}); // TODO: Adapt to use your actual default value
    // update ();
    emit characterChanged (m_character);
}

QFont GlyphWidget::font() const
{
    return m_font;
}

void GlyphWidget::setFont(const QFont &newFont)
{
    if (m_font == newFont)
        return;
    m_font = newFont;

    resetFont();
    QString fontPath;
    if (GetSystemFontFilePath(m_font.family(), fontPath)) {
        m_fontPath = fontPath;
        m_ftError = FT_New_Face(m_library, m_fontPath.toLatin1().constData(), 0, &m_face);
        if (!m_ftError)
        {
            qDebug() << "New Face Created";
        } else {
            qDebug() << "New Face error";
        }
    }

    emit fontChanged(m_font);
}

void GlyphWidget::resetFont()
{
    setFont({}); // TODO: Adapt to use your actual default value
    if (m_face != nullptr) {
        FT_Done_Face(m_face);
    }
    m_face = nullptr;
}

QColor GlyphWidget::bgColor() const
{
    return m_bgColor;
}

void GlyphWidget::setBgColor(const QColor &newBgColor)
{
    if (m_bgColor == newBgColor)
        return;
    m_bgColor = newBgColor;
    emit bgColorChanged();
}

void GlyphWidget::resetBgColor()
{
    setBgColor({}); // TODO: Adapt to use your actual default value
}

QColor GlyphWidget::glyphColor() const
{
    return m_glyphColor;
}

void GlyphWidget::resetGlyphColor()
{
    setGlyphColor({}); // TODO: Adapt to use your actual default value
}

float GlyphWidget::scale() const
{
    return m_scale;
}

void GlyphWidget::setScale(float newScale)
{
    if (qFuzzyCompare(m_scale, newScale))
        return;
    m_scale = newScale;
    emit scaleChanged();
}

void GlyphWidget::resetScale()
{
    setScale({}); // TODO: Adapt to use your actual default value
}

bool GlyphWidget::showGrid() const
{
    return m_showGrid;
}

void GlyphWidget::setShowGrid(bool newShowGrid)
{
    if (m_showGrid == newShowGrid)
        return;
    m_showGrid = newShowGrid;
    emit showGridChanged();
}

void GlyphWidget::resetShowGrid()
{
    setShowGrid({}); // TODO: Adapt to use your actual default value
}

int GlyphWidget::gridSize() const
{
    return m_gridSize;
}

void GlyphWidget::setGridSize(int newGridSize)
{
    if (m_gridSize == newGridSize)
        return;

    m_gridSize = newGridSize;
    m_pixels.resize(newGridSize);
    for (auto &row : m_pixels) {
        row.resize(newGridSize);
        row.fill(false);
    }
    setFixedSize(newGridSize, newGridSize);  // Увеличим для удобства редактирования
    update();

    emit gridSizeChanged();
}

void GlyphWidget::resetGridSize()
{
    setGridSize({}); // TODO: Adapt to use your actual default value
}

int GlyphWidget::gridColumns() const
{
    return m_gridColumns;
}

void GlyphWidget::setGridColumns(int newGridColumns)
{
    if (m_gridColumns == newGridColumns)
        return;
    m_gridColumns = newGridColumns;
    emit gridColumnsChanged();
}

void GlyphWidget::resetGridColumns()
{
    setGridColumns({}); // TODO: Adapt to use your actual default value
}

int GlyphWidget::gridRows() const
{
    return m_gridRows;
}

void GlyphWidget::setGridRows(int newGridRows)
{
    if (m_gridRows == newGridRows)
        return;
    m_gridRows = newGridRows;
    emit gridRowsChanged();
}

void GlyphWidget::resetGridRows()
{
    setGridRows({}); // TODO: Adapt to use your actual default value
}
