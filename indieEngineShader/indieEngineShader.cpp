#include "indieEngineShader.h"
#include "ui_indieEngineShader.h"

indieEngineShader::indieEngineShader(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::indieEngineShader)
{
    ui->setupUi(this);
}

indieEngineShader::~indieEngineShader()
{
    delete ui;
}
