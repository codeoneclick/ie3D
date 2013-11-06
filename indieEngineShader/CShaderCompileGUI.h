#ifndef CSHADERCOMPILEGUI_H
#define CSHADERCOMPILEGUI_H

#include <QDialog>

namespace Ui {
class CShaderCompileGUI;
}

class CShaderCompileGUI : public QDialog
{
    Q_OBJECT

public:
    explicit CShaderCompileGUI(QWidget *parent = 0);
    ~CShaderCompileGUI();

private:
    Ui::CShaderCompileGUI *ui;
};

#endif // CSHADERCOMPILEGUI_H
