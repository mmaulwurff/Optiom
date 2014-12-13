#ifndef KEYINPUTBOX_H
#define KEYINPUTBOX_H

#include <QMessageBox>

class KeyInputBox : public QMessageBox {
public:
    KeyInputBox(QWidget * parent);

    int Key() const { return key; }

    static int GetKey(QWidget * parent);
protected:
    void keyPressEvent(QKeyEvent * event) override;

private:
    int key;
};

#endif // KEYINPUTBOX_H
