#ifndef CMAINWINDOW_H
#define CMAINWINDOW_H

#include <QMainWindow>
#include "HCommon.h"
#include "HMEDeclaration.h"
#include "HEnums.h"
#include "IConfiguration.h"
#include "IRenderTechniqueOperationTextureHandler.h"

class CMEGameController;
class CMEMainSceneTransition;
class CMEPreviewModelSceneTransition;

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
    
    std::shared_ptr<CMEGameController> m_mainSceneController;
    std::shared_ptr<CMEMainSceneTransition> m_mainSceneTransition;
    
    std::shared_ptr<CMEGameController> m_modelPreviewController;
    std::shared_ptr<CMEPreviewModelSceneTransition> m_modelPreviewTransition;
    
    CSharedMESceneToUICommands m_sceneToUICommands;
    
    void setBrushSize(ui32 value);
    void setBrushStrength(ui32 value);
    void setFalloffCoefficient(ui32 value);
    void setSmoothCoefficient(ui32 value);
    void setTextureSampler(CSharedTextureRef texture, E_SHADER_SAMPLER sampler);
    void setTillingTexcoord(f32 value, E_SHADER_SAMPLER sampler);
    
    i32 m_previousBrushSize;
    std::string m_recentFilename;
    
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

private:
    
    Ui::CMainWindow *ui;
};

#endif
