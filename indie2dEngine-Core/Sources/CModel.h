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

class CSkeleton;
class CSequence;
class CAnimationMixer;

class CModel final : public IGameObject
{
private:
    
protected:
    
    std::shared_ptr<CSkeleton> m_skeleton;
    std::set<std::shared_ptr<CSequence>> m_sequences;
    std::shared_ptr<CAnimationMixer> m_animationMixer;
    
    void _OnSceneUpdate(f32 _deltatime);
    
    i32 _OnQueuePosition(void);
    void _OnBind(const std::string& _renderMode);
    void _OnDraw(const std::string& _renderMode);
    void _OnUnbind(const std::string& _renderMode);
    
    void _OnTemplateLoaded(std::shared_ptr<ITemplate> _template);
    void _OnResourceLoaded(std::shared_ptr<IResource> _resource, bool _success);
    
public:
    
    CModel(std::shared_ptr<CResourceAccessor> _resourceFabricator);
    ~CModel(void);
    
    void Set_Animation(const std::string& _name);
};



#endif 
