//
//  CMaterial.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/17/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CMaterial_h
#define CMaterial_h

#include "HCommon.h"
#include "HEnums.h"

class CShader;
class CTexture;
class IScreenSpaceTextureAccessor;
class CResourceAccessor;

struct SMaterialTemplate;

class CMaterial
{
private:
    
protected:
    
    std::shared_ptr<CShader> m_shader;
    GLenum m_cullFaceMode;
    GLenum m_blendFunctionSource;
    GLenum m_blendFunctionDest;
    glm::vec4 m_clipping;
    std::shared_ptr<CTexture> m_textures[E_SHADER_SAMPLER_MAX];
    bool m_states[E_RENDER_STATE_MAX];
    bool m_isReflected;
    bool m_isBatching;
    bool m_isDebug;
    std::string m_guid;
    
public:
    
    CMaterial(std::shared_ptr<CShader> _shader, const std::string& _guid);
    ~CMaterial(void);
    
    void Serialize(const std::shared_ptr<SMaterialTemplate>& _template, const std::shared_ptr<CResourceAccessor>& _resourceAccessor, const std::shared_ptr<IScreenSpaceTextureAccessor>& _screenSpaceTextureAccessor);
    
    void Set_CullFaceMode(GLenum _mode);
    void Set_BlendFunctionSource(GLenum _blendFunction);
    void Set_BlendFunctionDest(GLenum _blendFunction);
    void Set_RenderState(E_RENDER_STATE _state, bool _value);
    
    std::shared_ptr<CShader> Get_Shader(void)
    {
        return m_shader;
    };
    
    void Set_Clipping(const glm::vec4& _clipping);
    glm::vec4 Get_Clipping(void);
    
    inline void Set_IsReflected(bool _value)
    {
        m_isReflected = _value;
    };
    
    inline bool Get_IsReflected(void) const
    {
        return m_isReflected;
    };
    
    inline void Set_IsDebug(bool _value)
    {
        m_isDebug = _value;
    };
    
    inline bool Get_IsDebug(void) const
    {
        return m_isDebug;
    };
    
    inline void Set_IsBatching(bool _value)
    {
        m_isBatching = _value;
    };
    
    inline bool Get_IsBatching(void) const
    {
        return m_isBatching;
    };
    
    inline std::string Get_Guid(void) const
    {
        return m_guid;
    };
    
    void Set_Texture(std::shared_ptr<CTexture> _texture, E_SHADER_SAMPLER _sampler);
    
    void Bind(void);
    void Unbind(void);
};

#endif 
