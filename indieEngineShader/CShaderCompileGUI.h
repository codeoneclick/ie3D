#ifndef CSHADERCOMPILEGUI_H
#define CSHADERCOMPILEGUI_H

#include <QDialog>

class CCodeEditor;

#if defined(__OSX__) || defined(__WIN32__)

class CIESAMainTransition;

#endif

namespace Ui {
class CShaderCompileGUI;
}

class CShaderCompileGUI : public QDialog
{
    Q_OBJECT
    
protected:
    
#if defined(__OSX__) || defined(__WIN32__)
    
    std::shared_ptr<CIESAMainTransition> m_iesaTransition;
    std::string m_mode;
    
#endif

    CCodeEditor* m_vsEditor;
    CCodeEditor* m_fsEditor;
    
    std::string m_recentVSFilename;
    std::string m_recentFSFilename;

public:
    explicit CShaderCompileGUI(QWidget *parent = 0);
    ~CShaderCompileGUI();
    
#if defined(__OSX__) || defined(__WIN32__)
    
    void Set_Transition(const std::shared_ptr<CIESAMainTransition> _ieasTransition);
    void Set_Mode(const std::string& _mode);
    
#endif

private slots:
    void on_btn_compile_clicked();

    void on_btn_open_vs_clicked();

    void on_btn_open_fs_clicked();

    void on_btn_save_vs_clicked();

    void on_btn_save_vs_as_clicked();

    void on_btn_save_fs_clicked();

    void on_btn_save_fs_as_clicked();

    void on_btn_back_clicked();

private:
    Ui::CShaderCompileGUI *ui;
};

#endif // CSHADERCOMPILEGUI_H
