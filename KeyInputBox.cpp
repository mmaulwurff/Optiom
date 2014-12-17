#include "KeyInputBox.h"
#include <QKeyEvent>
#include <QPushButton>

KeyInputBox::KeyInputBox(QWidget * const parent) :
        QMessageBox(parent),
        key(0)
{
    setText(tr("Press a Key"));
    setStandardButtons(QMessageBox::Cancel);
    button(QMessageBox::Cancel)->setFocusPolicy(Qt::NoFocus);
    exec();
}

void KeyInputBox::keyPressEvent(QKeyEvent * const event) {
    key = event->key();
    close();
}

int KeyInputBox::GetKey(QWidget * const parent) {
    return KeyInputBox(parent).key;
}
