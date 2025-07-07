#ifndef GLYPHWIDGET_H
#define GLYPHWIDGET_H

#include <QObject>
#include <QWidget>

#include <QLineEdit>
#include <QComboBox>
#include <QDockWidget>
#include <QSplitter>
#include <QListWidget>
#include <QSpinBox>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_TYPES_H
#include FT_OUTLINE_H
#include FT_RENDER_H

class GlyphWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GlyphWidget(QWidget *parent = nullptr);
    ~GlyphWidget();

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
    void loadCharFT(const QChar &character, quint32 charSize);

signals:
    void clicked();

    void characterChanged(const QChar &);
    void fontChanged(const QFont &);
    void bgColorChanged();
    void scaleChanged();
    void showGridChanged();
    void gridSizeChanged();
    void gridColumnsChanged();
    void gridRowsChanged();

private:
    void initFT();
    void loadFTFont(const QFont &font);
    void clearFT();
    void convertToGlyphPixels();
    void loadFromChar ();
    void clearGlyph ();

    QChar m_character;
    QFont m_font;
    QColor m_bgColor = Qt::white;
    QColor m_glyphColor = Qt::black;
    float m_scale = 1.0f;
    bool m_showGrid = true; //< показывать ли сетку
    int m_gridSize;         //< размер ячейки сетки (16 пикселей)
    int m_gridColumns;      //< кол-во столбцов (8x8)
    int m_gridRows;         //< кол-во строк
    int m_cellSize;         //< Размер ячейки для отрисовки
    int m_charSize;         //< Размер шрифта для отрисовки

    QVector<bool> m_glyphPixels;
    quint32 m_glyphRows;
    quint32 m_glyphCols;

    QVector<QVector<bool>> m_pixels;        // Матрица пикселей
    QColor m_pixelColor = Qt::black;        // Цвет "включенного" пикселя

    FT_Library m_library;
    FT_Face m_face;

    quint32 m_fontSize;
    QRect m_glyphRect;
    QString m_fontPath;
    FT_Error m_ftError;

    bool m_glyphUpdated;

    Q_PROPERTY(QChar character READ character WRITE setCharacter RESET resetCharacter NOTIFY characterChanged FINAL)
    Q_PROPERTY(QFont font READ font WRITE setFont RESET resetFont NOTIFY fontChanged FINAL)
    Q_PROPERTY(QColor bgColor READ bgColor WRITE setBgColor RESET resetBgColor NOTIFY bgColorChanged FINAL)
    Q_PROPERTY(float scale READ scale WRITE setScale RESET resetScale NOTIFY scaleChanged FINAL)
    Q_PROPERTY(bool showGrid READ showGrid WRITE setShowGrid RESET resetShowGrid NOTIFY showGridChanged FINAL)
    Q_PROPERTY(int gridSize READ gridSize WRITE setGridSize RESET resetGridSize NOTIFY gridSizeChanged FINAL)
    Q_PROPERTY(int gridColumns READ gridColumns WRITE setGridColumns RESET resetGridColumns NOTIFY gridColumnsChanged FINAL)
    Q_PROPERTY(int gridRows READ gridRows WRITE setGridRows RESET resetGridRows NOTIFY gridRowsChanged FINAL)
};

#endif // GLYPHWIDGET_H
