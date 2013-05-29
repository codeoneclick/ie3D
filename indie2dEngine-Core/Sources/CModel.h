//
//  CModel.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/28/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CModel_h
#define CModel_h

#include "IGameObject.h"

class CModel final : public IGameObject
{
private:
    
protected:
    
    void _OnSceneUpdate(f32 _deltatime);
    
    i32 _OnQueuePosition(void);
    void _OnBind(const std::string& _renderMode);
    void _OnDraw(const std::string& _renderMode);
    void _OnUnbind(const std::string& _renderMode);
    
    void _OnTemplateLoaded(std::shared_ptr<ITemplate> _template);
    
public:
    
    CModel(std::shared_ptr<CResourceFabricator> _resourceFabricator);
    ~CModel(void);
};



#endif 
