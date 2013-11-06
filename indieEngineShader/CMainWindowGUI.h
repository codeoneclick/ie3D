#ifndef CMAINWINDOWGUI_H
#define CMAINWINDOWGUI_H

#include <QMainWindow>

#include "CShaderCompileGUI.h"
#include "CMaterialSettingsGUI.h"
#include "CMaterialExportGUI.h"

namespace Ui {
class CMainWindowGUI;
}

class CMainWindowGUI : public QMainWindow
{
    Q_OBJECT
    
protected:
    void closeEvent(QCloseEvent*);
    
public:
    explicit CMainWindowGUI(QWidget *parent = 0);
    ~CMainWindowGUI();
    
    void Execute(void);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::CMainWindowGUI *ui;

    CShaderCompileGUI* m_shaderCompileGUI;
    CMaterialSettingsGUI* m_materialSettingsGUI;
    CMaterialExportGUI* m_materialExportGUI;
};

#endif // CMAINWINDOWGUI_H
