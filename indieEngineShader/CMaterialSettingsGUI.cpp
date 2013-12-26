#include "CMaterialSettingsGUI.h"
#include "ui_CMaterialSettingsGUI.h"

#if defined(__OSX__) || defined(__WIN32__)

#include "СIESAMainTransition.h"
#include "IGameObjectExtension.h"
#include "CTemplateGameObjects.h"
#include "CShaderExtension.h"
#include "CMaterialExtension.h"
#include "CShader.h"
#include "CTexture.h"
#include "CUICommon.h"

#endif

CMaterialSettingsGUI::CMaterialSettingsGUI(QWidget *parent) :
    QDialog(parent),
#if defined(__OSX__) || defined(__WIN32__)

    m_iesaTransition(nullptr),
    m_mode(""),

#endif
    ui(new Ui::CMaterialSettingsGUI)
{
    ui->setupUi(this);
    
    ui->cb_cull_face_mode->addItem(g_glenumToString[GL_FRONT].c_str());
    ui->cb_cull_face_mode->addItem(g_glenumToString[GL_BACK].c_str());
    
    ui->cb_blending_function_source->addItem(g_glenumToString[GL_SRC_ALPHA].c_str());
    ui->cb_blending_function_source->addItem(g_glenumToString[GL_ONE_MINUS_SRC_ALPHA].c_str());
    
    ui->cb_blending_function_destination->addItem(g_glenumToString[GL_SRC_ALPHA].c_str());
    ui->cb_blending_function_destination->addItem(g_glenumToString[GL_ONE_MINUS_SRC_ALPHA].c_str());
    
    ui->cb_texture_slot1_wrap->addItem(g_glenumToString[GL_REPEAT].c_str());
    ui->cb_texture_slot1_wrap->addItem(g_glenumToString[GL_CLAMP_TO_EDGE].c_str());
    ui->cb_texture_slot1_wrap->addItem(g_glenumToString[GL_MIRRORED_REPEAT].c_str());
    
    ui->cb_texture_slot2_wrap->addItem(g_glenumToString[GL_REPEAT].c_str());
    ui->cb_texture_slot2_wrap->addItem(g_glenumToString[GL_CLAMP_TO_EDGE].c_str());
    ui->cb_texture_slot2_wrap->addItem(g_glenumToString[GL_MIRRORED_REPEAT].c_str());
    
    ui->cb_texture_slot3_wrap->addItem(g_glenumToString[GL_REPEAT].c_str());
    ui->cb_texture_slot3_wrap->addItem(g_glenumToString[GL_CLAMP_TO_EDGE].c_str());
    ui->cb_texture_slot3_wrap->addItem(g_glenumToString[GL_MIRRORED_REPEAT].c_str());
    
    ui->cb_texture_slot4_wrap->addItem(g_glenumToString[GL_REPEAT].c_str());
    ui->cb_texture_slot4_wrap->addItem(g_glenumToString[GL_CLAMP_TO_EDGE].c_str());
    ui->cb_texture_slot4_wrap->addItem(g_glenumToString[GL_MIRRORED_REPEAT].c_str());
}

CMaterialSettingsGUI::~CMaterialSettingsGUI()
{
    delete ui;
}

void CMaterialSettingsGUI::Set_Transition(const std::shared_ptr<CIESAMainTransition> _ieasTransition)
{
    assert(_ieasTransition != nullptr);
    assert(m_mode.length() != 0);
    m_iesaTransition = _ieasTransition;
    
    IResourceLoadingHandler::RESOURCE_LOADING_HANDLER handler;
    std::function<void(const std::shared_ptr<IResource>&)> function = [handler, this](const std::shared_ptr<IResource>& _resource)
    {
        std::shared_ptr<CMaterialExtension> materialExtension = std::static_pointer_cast<CMaterialExtension>(_resource);
        std::shared_ptr<CMaterial> material = materialExtension->Get_Material();
        
        ui->eb_material_name->setText(material->Get_Guid().c_str());
        ui->cb_cull_face_status->setChecked(material->Get_IsCulling());
        ui->cb_cull_face_mode->setCurrentText(g_glenumToString[material->Get_CullingMode()].c_str());
        
        ui->cb_depth_test_status->setChecked(material->Get_IsDepthTest());
        ui->cb_depth_mask_status->setChecked(material->Get_IsDepthMask());
        
        ui->cb_blending_status->setChecked(material->Get_IsBlending());
        ui->cb_blending_function_source->setCurrentText(g_glenumToString[material->Get_BlendingFunctionSource()].c_str());
        ui->cb_blending_function_destination->setCurrentText(g_glenumToString[material->Get_BlendingFunctionDestination()].c_str());
        
        ui->eb_clipping_x->setText(std::to_string(material->Get_ClippingPlane().x).c_str());
        ui->eb_clipping_y->setText(std::to_string(material->Get_ClippingPlane().y).c_str());
        ui->eb_clipping_z->setText(std::to_string(material->Get_ClippingPlane().z).c_str());
        ui->eb_clipping_w->setText(std::to_string(material->Get_ClippingPlane().w).c_str());
        
        //ui->eb_vs_shader->setText(material->Get_Shader()->Get_VSName().c_str());
        //ui->eb_fs_shader->setText(material->Get_Shader()->Get_FSName().c_str());
        
        if(material->Get_Texture(E_SHADER_SAMPLER_01) != nullptr)
        {
            ui->eb_texture_slot_1->setText(material->Get_Texture(E_SHADER_SAMPLER_01)->Get_Guid().c_str());
            //ui->cb_texture_slot1_wrap->setCurrentText(g_glenumToString[material->Get_Texture(E_SHADER_SAMPLER_01)->Get_Wrap()].c_str());
            ui->cb_texture_slot1_wrap->setDisabled(false);
            ui->cb_texture_slot1_wrap->setStyleSheet(kBtnEnableStyleSheet);
        }
        else
        {
            ui->cb_texture_slot1_wrap->setDisabled(true);
            ui->cb_texture_slot1_wrap->setStyleSheet(kBtnDisableStyleSheet);
        }
        
        if(material->Get_Texture(E_SHADER_SAMPLER_02) != nullptr)
        {
            //ui->eb_texture_slot_2->setText(material->Get_Texture(E_SHADER_SAMPLER_02)->Get_Guid().c_str());
            //ui->cb_texture_slot2_wrap->setCurrentText(g_glenumToString[material->Get_Texture(E_SHADER_SAMPLER_02)->Get_Wrap()].c_str());
            ui->cb_texture_slot2_wrap->setDisabled(false);
            ui->cb_texture_slot2_wrap->setStyleSheet(kBtnEnableStyleSheet);
        }
        else
        {
            ui->cb_texture_slot2_wrap->setDisabled(true);
            ui->cb_texture_slot2_wrap->setStyleSheet(kBtnDisableStyleSheet);
        }
        
        if(material->Get_Texture(E_SHADER_SAMPLER_03) != nullptr)
        {
            //ui->eb_texture_slot_3->setText(material->Get_Texture(E_SHADER_SAMPLER_03)->Get_Guid().c_str());
            //ui->cb_texture_slot3_wrap->setCurrentText(g_glenumToString[material->Get_Texture(E_SHADER_SAMPLER_03)->Get_Wrap()].c_str());
            ui->cb_texture_slot3_wrap->setDisabled(false);
            ui->cb_texture_slot3_wrap->setStyleSheet(kBtnEnableStyleSheet);
        }
        else
        {
            ui->cb_texture_slot3_wrap->setDisabled(true);
            ui->cb_texture_slot3_wrap->setStyleSheet(kBtnDisableStyleSheet);
        }
        
        if(material->Get_Texture(E_SHADER_SAMPLER_04) != nullptr)
        {
            //ui->eb_texture_slot_4->setText(material->Get_Texture(E_SHADER_SAMPLER_04)->Get_Guid().c_str());
            //ui->cb_texture_slot4_wrap->setCurrentText(g_glenumToString[material->Get_Texture(E_SHADER_SAMPLER_04)->Get_Wrap()].c_str());
            ui->cb_texture_slot4_wrap->setDisabled(false);
            ui->cb_texture_slot4_wrap->setStyleSheet(kBtnEnableStyleSheet);
        }
        else
        {
            ui->cb_texture_slot4_wrap->setDisabled(true);
            ui->cb_texture_slot4_wrap->setStyleSheet(kBtnDisableStyleSheet);
        }
    };
    handler = std::make_shared<std::function<void(const std::shared_ptr<IResource>&)>>(function);
    m_iesaTransition->Get_GameObjectExtension()->Get_Material(handler, m_mode);
}

void CMaterialSettingsGUI::Set_Mode(const std::string &_mode)
{
    assert(_mode.length() != 0);
    m_mode = _mode;
}

