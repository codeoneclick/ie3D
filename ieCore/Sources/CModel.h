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
    void _OnBind(const std::string& _mode);
    void _OnDraw(const std::string& _mode);
    void _OnUnbind(const std::string& _mode);
    void _OnBatch(const std::string& _mode);
    
    void _OnTemplateLoaded(std::shared_ptr<I_RO_TemplateCommon> _template);
    void _OnResourceLoaded(std::shared_ptr<IResource> _resource, bool _success);
    
public:
    
    CModel(const std::shared_ptr<CResourceAccessor>& _resourceAccessor, const std::shared_ptr<IScreenSpaceTextureAccessor>& _screenSpaceTextureAccessor);
    ~CModel(void);
    
    void Set_Animation(const std::string& _name);
};



#endif 
