#include "CMaterialSettingsGUI.h"
#include "ui_CMaterialSettingsGUI.h"

CMaterialSettingsGUI::CMaterialSettingsGUI(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CMaterialSettingsGUI)
{
    ui->setupUi(this);
}

CMaterialSettingsGUI::~CMaterialSettingsGUI()
{
    delete ui;
}
