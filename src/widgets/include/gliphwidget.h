#ifndef GLIPHWIDGET_H
#define GLIPHWIDGET_H

#include <QWidget>

class GliphWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GliphWidget(QWidget *parent = nullptr);
    void setGlyph(QChar character, const QFont& font);
    void setBackgroundColor(const QColor& color);
    void setGlyphColor(const QColor& color);
    void setScaleFactor(float scale);

    QChar character() const;
    void setCharacter(const QChar &newCharacter);
    void resetCharacter();
    QFont font() const;
    void setFont(const QFont &newFont);
    void resetFont();
    QColor bgColor() const;
    void setBgColor(const QColor &newBgColor);
    void resetBgColor();
    QColor glyphColor() const;
    void resetGlyphColor();
    float scale() const;
    void setScale(float newScale);
    void resetScale();
    bool showGrid() const;
    void setShowGrid(bool newShowGrid);
    void resetShowGrid();
    int gridSize() const;
    void setGridSize(int newGridSize);
    void resetGridSize();
    int gridColumns() const;
    void setGridColumns(int newGridColumns);
    void resetGridColumns();
    int gridRows() const;
    void setGridRows(int newGridRows);
    void resetGridRows();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

signals:
    void clicked();

    void characterChanged();
    void fontChanged();
    void bgColorChanged();
    void scaleChanged();
    void showGridChanged();
    void gridSizeChanged();
    void gridColumnsChanged();
    void gridRowsChanged();

private:
    QChar m_character;
    QFont m_font;
    QColor m_bgColor = Qt::white;
    QColor m_glyphColor = Qt::black;
    float m_scale = 1.0f;
    bool m_showGrid = true;      // показывать ли сетку
    int m_gridSize;        // размер ячейки сетки (16 пикселей)
    int m_gridColumns;      // кол-во столбцов (8x8)
    int m_gridRows;         // кол-во строк
    QVector<QVector<bool>> m_pixels;        // Матрица пикселей
    QColor m_pixelColor = Qt::black;        // Цвет "включенного" пикселя

    Q_PROPERTY(QChar character READ character WRITE setCharacter RESET resetCharacter NOTIFY characterChanged FINAL)
    Q_PROPERTY(QFont font READ font WRITE setFont RESET resetFont NOTIFY fontChanged FINAL)
    Q_PROPERTY(QColor bgColor READ bgColor WRITE setBgColor RESET resetBgColor NOTIFY bgColorChanged FINAL)
    Q_PROPERTY(float scale READ scale WRITE setScale RESET resetScale NOTIFY scaleChanged FINAL)
    Q_PROPERTY(bool showGrid READ showGrid WRITE setShowGrid RESET resetShowGrid NOTIFY showGridChanged FINAL)
    Q_PROPERTY(int gridSize READ gridSize WRITE setGridSize RESET resetGridSize NOTIFY gridSizeChanged FINAL)
    Q_PROPERTY(int gridColumns READ gridColumns WRITE setGridColumns RESET resetGridColumns NOTIFY gridColumnsChanged FINAL)
    Q_PROPERTY(int gridRows READ gridRows WRITE setGridRows RESET resetGridRows NOTIFY gridRowsChanged FINAL)
};

#endif // GLIPHWIDGET_H
