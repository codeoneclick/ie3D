#ifndef CMAINWINDOW_H
#define CMAINWINDOW_H

#include <QMainWindow>
#include "HCommon.h"
#include "HMEDeclaration.h"
#include "HEnums.h"
#include "IConfiguration.h"
#include "IRenderTechniqueOperationTextureHandler.h"

class CMEGameController;
class CMEmseTransition;
class CMEgopTransition;
class CMEgoeTransition;
class CMEpoeTransition;

namespace Ui {
    class CMainWindow;
}

class CMainWindow :
public QMainWindow,
public std::enable_shared_from_this<CMainWindow>
{
    Q_OBJECT
    
protected:
    
#if defined(__OSX__) || defined(__WIN32__)
    
    std::shared_ptr<CMEGameController> m_mseController;
    std::shared_ptr<CMEmseTransition> m_mseTransition;
    
    std::shared_ptr<CMEGameController> m_gopController;
    std::shared_ptr<CMEgopTransition> m_gopTransition;
    
    std::shared_ptr<CMEGameController> m_goeController;
    std::shared_ptr<CMEgoeTransition> m_goeTransition;
    
    std::shared_ptr<CMEGameController> m_poeController;
    std::shared_ptr<CMEpoeTransition> m_poeTransition;
    
    ISharedUICommands m_mseSceneToUICommands;
    ISharedUICommands m_goeSceneToUICommands;
    
    std::vector<CSharedConfigurationMaterial> m_goeConfigurationsMaterials;
    
    void updateGOEUIConfigurationMaterial(CSharedConfigurationMaterialRef configuration);
    void resumeWidgets(void);
    void pauseWidgets(void);
    
    CSharedConfigurationMaterial getCurrentConfigurationMaterial(void);
    void commitCurrentConfigurationMaterial(void);
    
    void setMSEBrushSize(ui32 value);
    void setMSEBrushStrength(ui32 value);
    void setMSEFalloffCoefficient(ui32 value);
    void setMSESmoothCoefficient(ui32 value);
    void setMSETexture(CSharedTextureRef texture, E_SHADER_SAMPLER sampler);
    void setMSETillingTexcoord(f32 value, E_SHADER_SAMPLER sampler);
    
    void updateGOEConfigurationsMaterials(std::vector<CSharedConfigurationMaterial>& configurations);
    void generatorStatisticsUpdate(const std::string& operationName, E_HEIGHTMAP_GENERATION_STATUS status, const std::string& message = "");
    
    i32 m_brushSize;
    std::string m_recentOpenPath;
    
    bool event(QEvent *event);
    
#endif
    
public:
    
    explicit CMainWindow(QWidget *parent = 0);
    ~CMainWindow();
    
    void execute(void);
    
private slots:
    void on_m_brushSizeSlider_valueChanged(int value);

    void on_m_brushStrengthSlider_valueChanged(int value);

    void on_m_falloffSlider_valueChanged(int value);

    void on_m_smoothSlider_valueChanged(int value);

    void on_m_texture01Btn_pressed();

    void on_m_texture01Btn_clicked();

    void on_m_texture02Btn_clicked();

    void on_m_texture03Btn_clicked();

    void on_m_textureTilling01SpinBox_valueChanged(int arg1);

    void on_m_textureTilling02SpinBox_valueChanged(int arg1);

    void on_m_textureTilling03SpinBox_valueChanged(int arg1);

    void on_generateButton_clicked();

    void on_m_mainMenuTabs_currentChanged(int index);

    void on_m_landscapePropertiesTab_currentChanged(int index);

    void on_m_createGameObjectConfiguration_clicked();

    void on_m_cullFaceCheckBox_stateChanged(int arg1);

    void on_m_cullModeComboBox_currentIndexChanged(const QString &arg1);

    void on_m_depthTestCheckBox_stateChanged(int arg1);

    void on_m_depthMaskCheckBox_stateChanged(int arg1);

    void on_m_blendingCheckBox_stateChanged(int arg1);

    void on_m_blendingSourceComboBox_currentIndexChanged(const QString &arg1);

    void on_m_blendingDesinationComboBox_currentIndexChanged(const QString &arg1);

    void on_m_clippingCheckBox_stateChanged(int arg1);

    void on_m_clippingXSpinBox_valueChanged(double arg1);

    void on_m_clippingYSpinBox_valueChanged(double arg1);

    void on_m_clippingZSpinBox_valueChanged(double arg1);

    void on_m_clippingWSpinBox_valueChanged(double arg1);

    void on_m_reflectingCheckBox_stateChanged(int arg1);

    void on_m_shadowingCheckBox_stateChanged(int arg1);

    void on_m_debuggingCheckBox_stateChanged(int arg1);

    void on_m_shaderButton_clicked();

    void on_m_texture1LoadButton_clicked();

    void on_m_texture2LoadButton_clicked();

    void on_m_texture3LoadButton_clicked();

    void on_m_goeSettingsTab_currentChanged(int index);

    void on_m_drawCurrentRadioButton_clicked();

    void on_m_drawAllRadioButton_clicked();

    void on_m_materialEnabledCheckBox_clicked();

    void on_m_materialsComboBox_currentIndexChanged(int index);

    void on_m_addModelToSceneButton_clicked();

    void on_m_modelsList_currentRowChanged(int currentRow);

private:
    
    Ui::CMainWindow *ui;
};

#endif
