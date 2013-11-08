#include "CShaderCompileGUI.h"
#include "ui_CShaderCompileGUI.h"

#if defined(__OSX__) || defined(__WIN32__)

#include "СIESAMainTransition.h"

#endif

CShaderCompileGUI::CShaderCompileGUI(QWidget *parent) :
    QDialog(parent),
#if defined(__OSX__) || defined(__WIN32__)
    m_iesaTrantision(nullptr),
#endif
    ui(new Ui::CShaderCompileGUI)
{
    ui->setupUi(this);
}

CShaderCompileGUI::~CShaderCompileGUI()
{
    delete ui;
}


void CShaderCompileGUI::Set_Transition(const std::shared_ptr<IGameTransition> _ieasTransition)
{
    assert(_ieasTransition != nullptr);
    m_iesaTrantision = _ieasTransition;
    
    ui->source_vs->clear();
    ui->source_vs->appendPlainText("message");
}