//
//  CGameViewController.m
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/17/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CGameViewController.h"
#include "COGLWindow_iOS.h"
#include "CGame.h"
#include "CGameRootTransition.h"
#include "CCommonOS.h"
#include "CModel.h"
#include "CCamera.h"
#include "CLight.h"

@interface CGameViewController ()

@property (weak, nonatomic) IBOutlet COGLWindow_iOS *m_glWindow;
@property (unsafe_unretained, nonatomic) CGameRootTransition* m_transition;
@end

@implementation CGameViewController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self)
    {
        
    }
    return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    self.m_transition = new CGameRootTransition((__bridge void*)_m_glWindow);
    std::shared_ptr<CCamera> camera = self.m_transition->CreateCamera(60.0f,
                                    0.1f,
                                    256.0f,
                                    glm::vec4(0.0f, 0.0f, Get_ScreenWidth(), Get_ScreenHeight()));
    camera->Set_Position(glm::vec3(0.0f, 0.0f, 0.0f));
    camera->Set_LookAt(glm::vec3(32.0f, 0.0f, 32.0f));
    camera->Set_Distance(8.0f);
    camera->Set_Height(8.0f);
    std::shared_ptr<CLight> light = self.m_transition->CreateLight();
    light->Set_Position(glm::vec3(32.0f, 32.0f, 32.0f));
    self.m_transition->Set_Camera(camera);
    self.m_transition->Set_Light(light);
    std::shared_ptr<CModel> model = self.m_transition->CreateModel("model.xml");
    model->Set_Position(glm::vec3(32.0f, 0.0f, 32.0f));
    self.m_transition->InsertModel(model);
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
}

@end
