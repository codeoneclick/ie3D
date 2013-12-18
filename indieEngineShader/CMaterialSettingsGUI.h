#ifndef CMATERIALSETTINGSGUI_H
#define CMATERIALSETTINGSGUI_H

#include <QDialog>

#if defined(__OSX__) || defined(__WIN32__)

class CIESAMainTransition;

#endif

namespace Ui {
class CMaterialSettingsGUI;
}

class CMaterialSettingsGUI : public QDialog
{
    Q_OBJECT

protected:
    
#if defined(__OSX__) || defined(__WIN32__)
    
    std::shared_ptr<CIESAMainTransition> m_iesaTransition;
    std::string m_mode;
    
#endif
    
public:
    explicit CMaterialSettingsGUI(QWidget *parent = 0);
    ~CMaterialSettingsGUI();
    
#if defined(__OSX__) || defined(__WIN32__)
    
    void Set_Transition(const std::shared_ptr<CIESAMainTransition> _ieasTransition);
    void Set_Mode(const std::string& _mode);
    
#endif

private:
    Ui::CMaterialSettingsGUI *ui;
};

#endif // CMATERIALSETTINGSGUI_H
