#include "CMaterialExportGUI.h"
#include "ui_CMaterialExportGUI.h"

CMaterialExportGUI::CMaterialExportGUI(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CMaterialExportGUI)
{
    ui->setupUi(this);
}

CMaterialExportGUI::~CMaterialExportGUI()
{
    delete ui;
}
