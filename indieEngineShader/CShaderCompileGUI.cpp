#include "CShaderCompileGUI.h"
#include "ui_CShaderCompileGUI.h"
#include "CCodeEditor.h"
#include "QFileDialog.h"
#include "QMessageBox.h"

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
    
    m_vsEditor = new CCodeEditor(ui->groupBox);
    m_vsEditor->setGeometry(ui->source_vs->geometry());
    m_vsEditor->setFont(ui->source_vs->font());
    m_vsEditor->setStyleSheet(ui->source_vs->styleSheet());
    ui->source_vs->setVisible(false);
    
    m_fsEditor = new CCodeEditor(ui->groupBox_2);
    m_fsEditor->setGeometry(ui->source_fs->geometry());
    m_fsEditor->setFont(ui->source_fs->font());
    m_fsEditor->setStyleSheet(ui->source_fs->styleSheet());
    ui->source_fs->setVisible(false);
}

CShaderCompileGUI::~CShaderCompileGUI()
{
    delete ui;
    delete m_vsEditor;
    delete m_fsEditor;
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
        m_vsEditor->clear();
        m_vsEditor->appendPlainText(shaderExtension->Get_VertexShaderSourceCode().c_str());
        
        m_fsEditor->clear();
        m_fsEditor->appendPlainText(shaderExtension->Get_FragmentShaderSourceCode().c_str());
    };
    handler = std::make_shared<std::function<void(const std::shared_ptr<IResource>&)>>(function);
    m_iesaTransition->Get_GameObjectExtension()->Get_Shader(handler, m_mode);
}

void CShaderCompileGUI::Set_Mode(const std::string &_mode)
{
    assert(_mode.length() != 0);
    m_mode = _mode;
}
void CShaderCompileGUI::on_btn_compile_clicked()
{
#if defined(__OSX__) || defined(__WIN32__)
    
    IResourceLoadingHandler::RESOURCE_LOADING_HANDLER handler;
    std::function<void(const std::shared_ptr<IResource>&)> function = [handler, this](const std::shared_ptr<IResource>& _resource){
        std::shared_ptr<CShaderExtension> shaderExtension = std::static_pointer_cast<CShaderExtension>(_resource);
        std::string vsSourceCode = m_vsEditor->toPlainText().toUtf8().constData();
        std::string fsSourceCode = m_fsEditor->toPlainText().toUtf8().constData();
        shaderExtension->Compile(vsSourceCode, fsSourceCode, [this](const std::string& _message){
            ui->console->clear();
            ui->console->appendHtml(_message.c_str());
        });
    };
    handler = std::make_shared<std::function<void(const std::shared_ptr<IResource>&)>>(function);
    m_iesaTransition->Get_GameObjectExtension()->Get_Shader(handler, m_mode);
    
#endif
}

void CShaderCompileGUI::on_btn_open_vs_clicked()
{
    QString recentVSFilename = m_recentVSFilename.length() != 0 ? QString(m_recentVSFilename.c_str()) : "";
    QString filename = QFileDialog::getOpenFileName(this, tr("Open..."), recentVSFilename, tr("Files (*.*)"));
    if (filename.isEmpty())
    {
        return;
    }
    else
    {
        std::ifstream* filestream = new std::ifstream();
        filestream->open(filename.toUtf8().constData());
        if (!filestream->is_open())
        {
            QMessageBox messageBox(QMessageBox::Critical, "Error", "Can't open!");
            messageBox.exec();
            return;
        }
        std::stringstream stringstream;
        stringstream<<filestream->rdbuf();
        filestream->close();
        delete filestream;
        std::string vsSourceCode(stringstream.str());
        m_vsEditor->clear();
        m_vsEditor->appendPlainText(vsSourceCode.c_str());
        m_recentVSFilename = filename.toUtf8().constData();
        stringstream.flush();
    }
}

void CShaderCompileGUI::on_btn_open_fs_clicked()
{
    QString recentFSFilename = m_recentFSFilename.length() != 0 ? QString(m_recentFSFilename.c_str()) : "";
    QString filename = QFileDialog::getOpenFileName(this, tr("Open..."), recentFSFilename, tr("Files (*.*)"));
    if (filename.isEmpty())
    {
        return;
    }
    else
    {
        std::ifstream* filestream = new std::ifstream();
        filestream->open(filename.toUtf8().constData());
        if (!filestream->is_open())
        {
            QMessageBox messageBox(QMessageBox::Critical, "Error", "Can't open!");
            messageBox.exec();
            return;
        }
        std::stringstream stringstream;
        stringstream<<filestream->rdbuf();
        filestream->close();
        delete filestream;
        std::string fsSourceCode(stringstream.str());
        m_fsEditor->clear();
        m_fsEditor->appendPlainText(fsSourceCode.c_str());
        m_recentFSFilename = filename.toUtf8().constData();
        stringstream.flush();
    }
}

void CShaderCompileGUI::on_btn_save_vs_clicked()
{
    
}

void CShaderCompileGUI::on_btn_save_vs_as_clicked()
{
    QString recentVSFilename = m_recentVSFilename.length() != 0 ? QString(m_recentVSFilename.c_str()) : "";
    QString filename = QFileDialog::getSaveFileName(this, tr("Save as..."), recentVSFilename, tr("Files (*.*)"));
    if (filename.isEmpty())
    {
        return;
    }
    else
    {
        std::ofstream* filestream = new std::ofstream();
        filestream->open(filename.toUtf8().constData());
        if (!filestream->is_open())
        {
            QMessageBox messageBox(QMessageBox::Critical, "Error", "Can't create!");
            messageBox.exec();
            return;
        }
        std::string vsSourceCode = m_vsEditor->toPlainText().toUtf8().constData();
        filestream->write(vsSourceCode.c_str(), vsSourceCode.length());
        filestream->close();
        delete filestream;
        
        QMessageBox messageBox(QMessageBox::Warning, "Success", "Don't forget move shader file relative material!");
        messageBox.exec();
    }
}

void CShaderCompileGUI::on_btn_save_fs_clicked()
{

}

void CShaderCompileGUI::on_btn_save_fs_as_clicked()
{
    QString recentVSFilename = m_recentVSFilename.length() != 0 ? QString(m_recentVSFilename.c_str()) : "";
    QString filename = QFileDialog::getSaveFileName(this, tr("Save as..."), recentVSFilename, tr("Files (*.*)"));
    if (filename.isEmpty())
    {
        return;
    }
    else
    {
        std::ofstream* filestream = new std::ofstream();
        filestream->open(filename.toUtf8().constData());
        if (!filestream->is_open())
        {
            QMessageBox messageBox(QMessageBox::Critical, "Error", "Can't create!");
            messageBox.exec();
            return;
        }
        std::string fsSourceCode = m_fsEditor->toPlainText().toUtf8().constData();
        filestream->write(fsSourceCode.c_str(), fsSourceCode.length());
        filestream->close();
        delete filestream;
        
        QMessageBox messageBox(QMessageBox::Warning, "Success", "Don't forget move shader file relative material!");
        messageBox.exec();
    }
}

void CShaderCompileGUI::on_btn_back_clicked()
{
    close();
}
