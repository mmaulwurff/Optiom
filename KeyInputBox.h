#ifndef KEYINPUTBOX_H
#define KEYINPUTBOX_H

#include <QMessageBox>

class KeyInputBox : public QMessageBox {
    Q_OBJECT
public:
    static int GetKey(QWidget * parent);

protected:
    void keyPressEvent(QKeyEvent *) override;

private:
    KeyInputBox(QWidget * parent);

    int key;
};

#endif // KEYINPUTBOX_H
