#ifndef CMATERIALSETTINGSGUI_H
#define CMATERIALSETTINGSGUI_H

#include <QDialog>

namespace Ui {
class CMaterialSettingsGUI;
}

class CMaterialSettingsGUI : public QDialog
{
    Q_OBJECT

public:
    explicit CMaterialSettingsGUI(QWidget *parent = 0);
    ~CMaterialSettingsGUI();

private:
    Ui::CMaterialSettingsGUI *ui;
};

#endif // CMATERIALSETTINGSGUI_H
