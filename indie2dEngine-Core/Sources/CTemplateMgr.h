//
//  CTemplateMgr.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/29/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CTemplateMgr_h
#define CTemplateMgr_h

#include "HCommon.h"
#include "HEnums.h"

class CModelTemplateLoader;
class CParticleEmitterTemplateLoader;
class ITemplateLoadingHandler;
class CTemplateMgr
{
private:
    
protected:
    
    std::shared_ptr<CModelTemplateLoader> m_modelTemplateLoader;
    std::shared_ptr<CParticleEmitterTemplateLoader> m_particleEmitterTemplateLoader;
    
public:
    
    CTemplateMgr(void);
    ~CTemplateMgr(void);
    
    void LoadModelTemplate(const std::string& _filename, std::shared_ptr<ITemplateLoadingHandler> _handler);
    void LoadParticleEmitterTemplate(const std::string& _filename, std::shared_ptr<ITemplateLoadingHandler> _handler);
};

#endif 
