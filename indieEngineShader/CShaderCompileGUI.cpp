#include "CShaderCompileGUI.h"
#include "ui_CShaderCompileGUI.h"

#if defined(__OSX__) || defined(__WIN32__)

#include "СIESAMainTransition.h"
#include "IGameObjectExtension.h"
#include "CShaderExtension.h"

#endif

CShaderCompileGUI::CShaderCompileGUI(QWidget *parent) :
    QDialog(parent),
#if defined(__OSX__) || defined(__WIN32__)
    m_iesaTransition(nullptr),
    m_mode(""),
#endif
    ui(new Ui::CShaderCompileGUI)
{
    ui->setupUi(this);
}

CShaderCompileGUI::~CShaderCompileGUI()
{
    delete ui;
}


void CShaderCompileGUI::Set_Transition(const std::shared_ptr<CIESAMainTransition> _ieasTransition)
{
    assert(_ieasTransition != nullptr);
    assert(m_mode.length() != 0);
    m_iesaTransition = _ieasTransition;
    
    IResourceLoadingHandler::RESOURCE_LOADING_HANDLER handler;
    std::function<void(const std::shared_ptr<IResource>&)> function = [handler, this](const std::shared_ptr<IResource>& _resource)
    {
        std::shared_ptr<CShaderExtension> shaderExtension = std::static_pointer_cast<CShaderExtension>(_resource);
        ui->source_vs->clear();
        ui->source_vs->appendPlainText(shaderExtension->Get_VertexShaderSourceCode().c_str());
        
        ui->source_fs->clear();
        ui->source_fs->appendPlainText(shaderExtension->Get_FragmentShaderSourceCode().c_str());
    };
    handler = std::make_shared<std::function<void(const std::shared_ptr<IResource>&)>>(function);
    m_iesaTransition->Get_GameObjectExtension()->Get_Shader(handler, m_mode);
}

void CShaderCompileGUI::Set_Mode(const std::string &_mode)
{
    assert(_mode.length() != 0);
    m_mode = _mode;
}