#ifndef KEYSTRINGS_H
#define KEYSTRINGS_H

#include <QString>

QString F(const unsigned number) {
    return QString("F%1").arg(number < 100 ? number : 99);
}

QString Symbol(const char letter) {
    return QString(QChar(letter));
}

const QString NoKey = "NoKey";

QString GetKeyString(const int key) {
    if (Qt::Key_F1 <= key && key <= Qt::Key_F35) {
        return F(key - Qt::Key_F1 + 1);
    }

    if (Qt::Key_Space <= key && key <= Qt::Key_QuoteLeft) { // first ASCII block
        return Symbol(key - Qt::Key_Space + ' ');
    }

    if (Qt::Key_BraceLeft <= key && key <= Qt::Key_AsciiTilde) { // second ASCII block
        return Symbol(key - Qt::Key_BraceLeft + '{');
    }

    switch ( key ) {
    case Qt::Key_Escape:    return "Escape";
    case Qt::Key_Tab:       return "Tab";
    case Qt::Key_Backspace: return "Backspace";
    case Qt::Key_Home:      return "Home";
    case Qt::Key_End:       return "End";
    case Qt::Key_Insert:    return "Insert";
    case Qt::Key_Delete:    return "Delete";
    case Qt::Key_PageDown:  return "PageDown";
    case Qt::Key_PageUp:    return "PageUp";

    case Qt::Key_Shift:     return "Shift";
    case Qt::Key_Control:   return "Ctrl";
    case Qt::Key_Alt:       return "Alt";
    case Qt::Key_AltGr:     return "AltGr";
    case Qt::Key_Meta:      return "Meta";
    case Qt::Key_Menu:      return "Menu";
    case Qt::Key_SysReq:    return "SysReq";
    case Qt::Key_Pause:     return "PauseBreak";
    case Qt::Key_NumLock:   return "NumLock";

    case Qt::Key_Return:
    case Qt::Key_Enter:     return "Enter";

    case Qt::Key_Up:        return "Up";
    case Qt::Key_Down:      return "Down";
    case Qt::Key_Left:      return "Left";
    case Qt::Key_Right:     return "Right";

    case Qt::Key_Search:    return "Search";
    default: return NoKey;
    }
}

#endif // KEYSTRINGS_H

