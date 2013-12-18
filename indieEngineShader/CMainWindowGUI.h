#ifndef CMAINWINDOWGUI_H
#define CMAINWINDOWGUI_H

#include <QMainWindow>

#include "CShaderCompileGUI.h"
#include "CMaterialSettingsGUI.h"
#include "CMaterialExportGUI.h"

#if defined(__OSX__) || defined(__WIN32__)

#include "HCommon.h"

class CIESAWorkflow;
class CIESAMainTransition;

#endif

namespace Ui {
class CMainWindowGUI;
}

class CMainWindowGUI : public QMainWindow
{
    Q_OBJECT
    
protected:
    void closeEvent(QCloseEvent*);
    
#if defined(__OSX__) || defined(__WIN32__)
    
    std::shared_ptr<CIESAWorkflow> m_iesaWorkflow;
    std::shared_ptr<CIESAMainTransition> m_iesaTransition;
    std::string m_mode;
    
#endif
    
public:
    
    explicit CMainWindowGUI(QWidget *parent = 0);
    ~CMainWindowGUI();
    
    void Execute(void);

private slots:
    
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_materials_list_currentIndexChanged(const QString &arg1);
    
    void on_buttonOpen_clicked(void);
    void on_buttonSave_clicked(void);
    void on_buttonExit_clicked(void);
    void on_btn_shader_compile_clicked();

    void on_btn_content_path_clicked();

    void on_btn_open_clicked();

    void on_btn_save_clicked();

    void on_btn_saveas_clicked();

    void on_btn_material_editor_clicked();

private:
    
    Ui::CMainWindowGUI *ui;

    CShaderCompileGUI* m_shaderCompileGUI;
    CMaterialSettingsGUI* m_materialSettingsGUI;
    CMaterialExportGUI* m_materialExportGUI;
};

#endif // CMAINWINDOWGUI_H
