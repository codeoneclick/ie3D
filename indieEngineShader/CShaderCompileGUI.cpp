#include "CShaderCompileGUI.h"
#include "ui_CShaderCompileGUI.h"

CShaderCompileGUI::CShaderCompileGUI(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CShaderCompileGUI)
{
    ui->setupUi(this);
}

CShaderCompileGUI::~CShaderCompileGUI()
{
    delete ui;
}
