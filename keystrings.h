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

/// Convert Qt::Key codes into strings.
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
    case Qt::Key_CapsLock:  return "CapsLock";

    case Qt::Key_Return:
    case Qt::Key_Enter:     return "Enter";

    case Qt::Key_Up:        return "Up";
    case Qt::Key_Down:      return "Down";
    case Qt::Key_Left:      return "Left";
    case Qt::Key_Right:     return "Right";

    case Qt::Key_Search:    return "Search";

    case Qt::Key_VolumeDown: return "VolumeDown";
    case Qt::Key_VolumeUp:   return "VolumeUp";
    case Qt::Key_VolumeMute: return "VolumeMute";

    case Qt::Key_MediaPrevious: return "MediaPrevious";
    case Qt::Key_MediaNext:     return "MediaNext";
    case Qt::Key_MediaPause:    return "MediaPause";
    case Qt::Key_MediaPlay:     return "MediaPlay";

    default: return NoKey;
    }
}

#endif // KEYSTRINGS_H

