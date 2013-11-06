#ifndef CMATERIALEXPORTGUI_H
#define CMATERIALEXPORTGUI_H

#include <QDialog>

namespace Ui {
class CMaterialExportGUI;
}

class CMaterialExportGUI : public QDialog
{
    Q_OBJECT

public:
    explicit CMaterialExportGUI(QWidget *parent = 0);
    ~CMaterialExportGUI();

private:
    Ui::CMaterialExportGUI *ui;
};

#endif // CMATERIALEXPORTGUI_H
