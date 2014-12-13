#include "KeyInputBox.h"
#include <QDebug>
#include <QKeyEvent>
#include <QPushButton>

KeyInputBox::KeyInputBox(QWidget * const parent) :
        QMessageBox(parent),
        key(0)
{
    setText("Press new key setting");
    setStandardButtons(QMessageBox::Cancel);
    button(QMessageBox::Cancel)->setFocusPolicy(Qt::NoFocus);
    exec();
}

void KeyInputBox::keyPressEvent(QKeyEvent * const event) {
    key = event->key();
    close();
}

int KeyInputBox::GetKey(QWidget * const parent) {
    KeyInputBox temp(parent);
    return temp.Key();
}
