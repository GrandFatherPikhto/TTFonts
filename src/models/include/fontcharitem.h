#ifndef __FONTCHARACTERITEM_H__
#define __FONTCHARACTERITEM_H__

#include <QChar>

class FontCharItem {
public:
    FontCharItem(const QChar &character, bool selected) :
          m_character(character)
        , m_selected(selected)
    {

    }

    FontCharItem(const FontCharItem &charItem) = default;

    const QChar & getChar() const {
        return m_character;
    }

    bool isSelected() const {
        return m_selected;
    }

    inline void setSelected(bool selected) {
        m_selected = selected;
    }

    inline void setCharacter(const QChar &character) {
        m_character = character;
    }

    bool operator == (const FontCharItem & charItem) {
        return m_character == charItem.m_character;
    }

private:
    QChar m_character;
    bool m_selected;
};

#endif // __FONTCHARACTERITEM_H__
