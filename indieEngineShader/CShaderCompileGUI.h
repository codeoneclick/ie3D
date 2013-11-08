#ifndef CSHADERCOMPILEGUI_H
#define CSHADERCOMPILEGUI_H

#include <QDialog>

#if defined(__OSX__) || defined(__WIN32__)

class IGameTransition;

#endif

namespace Ui {
class CShaderCompileGUI;
}

class CShaderCompileGUI : public QDialog
{
    Q_OBJECT
    
protected:
    
#if defined(__OSX__) || defined(__WIN32__)
    
    std::shared_ptr<IGameTransition> m_iesaTrantision;
    
#endif


public:
    explicit CShaderCompileGUI(QWidget *parent = 0);
    ~CShaderCompileGUI();
    
#if defined(__OSX__) || defined(__WIN32__)
    
    void Set_Transition(const std::shared_ptr<IGameTransition> _ieasTransition);
    
#endif

private:
    Ui::CShaderCompileGUI *ui;
};

#endif // CSHADERCOMPILEGUI_H
