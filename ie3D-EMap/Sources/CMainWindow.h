#ifndef CMAINWINDOW_H
#define CMAINWINDOW_H

#include <QMainWindow>

class CEditableSceneController;
class CEditableSceneTransition;

namespace Ui {
    class CMainWindow;
}

class CMainWindow : public QMainWindow
{
    Q_OBJECT
    
protected:
    
#if defined(__OSX__) || defined(__WIN32__)
    
    std::shared_ptr<CEditableSceneController> m_editableSceneController;
    std::shared_ptr<CEditableSceneTransition> m_editableSceneTransition;
    
#endif
    
public:
    
    explicit CMainWindow(QWidget *parent = 0);
    ~CMainWindow();
    
    void execute(void);
    
private:
    
    Ui::CMainWindow *ui;
};

#endif
