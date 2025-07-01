#include <QPainter>
#include <QMouseEvent>

#include "gliphwidget.h"

GliphWidget::GliphWidget(QWidget *parent) :
    QWidget(parent)
    , m_gridSize(16)
    , m_gridRows(8)
    , m_gridColumns(8)
{
    setMinimumSize(100, 100);
    setGridSize(m_gridSize);
}

void GliphWidget::setGlyph(QChar character, const QFont &font)
{
    m_character = character;
    m_font = font;
    update();
}

void GliphWidget::setBackgroundColor(const QColor &color)
{
    m_bgColor = color;
    update();
}

void GliphWidget::setGlyphColor(const QColor &color)
{
    m_glyphColor = color;
    update();
}

void GliphWidget::setScaleFactor(float scale)
{
    m_scale = qBound(0.5f, scale, 5.0f); // Ограничиваем масштаб
    update();
}

void GliphWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.fillRect(rect(), Qt::white);

    // Рисуем пиксели
    const int cellSize = 16;  // Размер ячейки в пикселях (для удобства)
    // for (int y = 0; y < m_gridSize; ++y) {
    //     for (int x = 0; x < m_gridSize; ++x) {
    //         QRect rect(x * cellSize, y * cellSize, cellSize, cellSize);
    //         painter.fillRect(rect, m_pixels[y][x] ? m_pixelColor : Qt::white);
    //         painter.setPen(Qt::lightGray);
    //         painter.drawRect(rect);
    //     }
    // }
}

void GliphWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        emit clicked();
    }
    QWidget::mousePressEvent(event);
}

void GliphWidget::wheelEvent(QWheelEvent *event)
{
    float delta = event->angleDelta().y() > 0 ? 0.1f : -0.1f;
    setScaleFactor(m_scale + delta);
}

QChar GliphWidget::character() const
{
    return m_character;
}

void GliphWidget::setCharacter(const QChar &newCharacter)
{
    if (m_character == newCharacter)
        return;
    m_character = newCharacter;
    emit characterChanged();
}

void GliphWidget::resetCharacter()
{
    setCharacter({}); // TODO: Adapt to use your actual default value
}

QFont GliphWidget::font() const
{
    return m_font;
}

void GliphWidget::setFont(const QFont &newFont)
{
    if (m_font == newFont)
        return;
    m_font = newFont;
    emit fontChanged();
}

void GliphWidget::resetFont()
{
    setFont({}); // TODO: Adapt to use your actual default value
}

QColor GliphWidget::bgColor() const
{
    return m_bgColor;
}

void GliphWidget::setBgColor(const QColor &newBgColor)
{
    if (m_bgColor == newBgColor)
        return;
    m_bgColor = newBgColor;
    emit bgColorChanged();
}

void GliphWidget::resetBgColor()
{
    setBgColor({}); // TODO: Adapt to use your actual default value
}

QColor GliphWidget::glyphColor() const
{
    return m_glyphColor;
}

void GliphWidget::resetGlyphColor()
{
    setGlyphColor({}); // TODO: Adapt to use your actual default value
}

float GliphWidget::scale() const
{
    return m_scale;
}

void GliphWidget::setScale(float newScale)
{
    if (qFuzzyCompare(m_scale, newScale))
        return;
    m_scale = newScale;
    emit scaleChanged();
}

void GliphWidget::resetScale()
{
    setScale({}); // TODO: Adapt to use your actual default value
}

bool GliphWidget::showGrid() const
{
    return m_showGrid;
}

void GliphWidget::setShowGrid(bool newShowGrid)
{
    if (m_showGrid == newShowGrid)
        return;
    m_showGrid = newShowGrid;
    emit showGridChanged();
}

void GliphWidget::resetShowGrid()
{
    setShowGrid({}); // TODO: Adapt to use your actual default value
}

int GliphWidget::gridSize() const
{
    return m_gridSize;
}

void GliphWidget::setGridSize(int newGridSize)
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

void GliphWidget::resetGridSize()
{
    setGridSize({}); // TODO: Adapt to use your actual default value
}

int GliphWidget::gridColumns() const
{
    return m_gridColumns;
}

void GliphWidget::setGridColumns(int newGridColumns)
{
    if (m_gridColumns == newGridColumns)
        return;
    m_gridColumns = newGridColumns;
    emit gridColumnsChanged();
}

void GliphWidget::resetGridColumns()
{
    setGridColumns({}); // TODO: Adapt to use your actual default value
}

int GliphWidget::gridRows() const
{
    return m_gridRows;
}

void GliphWidget::setGridRows(int newGridRows)
{
    if (m_gridRows == newGridRows)
        return;
    m_gridRows = newGridRows;
    emit gridRowsChanged();
}

void GliphWidget::resetGridRows()
{
    setGridRows({}); // TODO: Adapt to use your actual default value
}
