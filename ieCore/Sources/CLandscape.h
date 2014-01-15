//
//  CLandscape.h
//  iGaia
//
//  Created by sergey.sergeev on 1/16/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef CLandscape_h
#define CLandscape_h

#include "IGameObject.h"
#include "CHeightmapProcessor.h"

class CLandscapeChunk;
class CLandscapeEdges;
class CShader;

class CLandscape : public IGameObject
{
private:

    std::shared_ptr<CHeightmapProcessor> m_heightmapProcessor;
    std::vector<std::shared_ptr<CLandscapeChunk>> m_chunks;
    std::shared_ptr<CLandscapeEdges> m_edges;
    
    ui32 m_numChunkRows;
    ui32 m_numChunkCells;

    std::shared_ptr<CMaterial> m_splattingDiffuseMaterial;
    std::shared_ptr<CMaterial> m_splattingNormalMaterial;
    bool m_isSplattingDiffuseTextureCommited;
    bool m_isSplattingNormalTextureCommited;
    
protected:

    void _OnSceneUpdate(f32 _deltatime);
    
    i32 _OnQueuePosition(void);
    void _OnBind(const std::string& _mode);
    void _OnDraw(const std::string& _mode);
    void _OnUnbind(const std::string& _mode);
    void _OnBatch(const std::string& _mode);
    
    void _OnTemplateLoaded(std::shared_ptr<I_RO_TemplateCommon> _template);
    void _OnResourceLoaded(std::shared_ptr<IResource> _resource, bool _success);

public:

    CLandscape(const std::shared_ptr<CResourceAccessor>& _resourceAccessor, const std::shared_ptr<IScreenSpaceTextureAccessor>& _screenSpaceTextureAccessor);
    ~CLandscape(void);

    void Set_Camera(std::shared_ptr<CCamera> _camera);
    void Set_Light(std::shared_ptr<CLight> _light, E_LIGHTS _id);
    
    void Set_RenderMgr(std::shared_ptr<CRenderMgr> _renderMgr);
    void Set_SceneUpdateMgr(std::shared_ptr<CSceneUpdateMgr> _sceneUpdateMgr);
    
    void ListenRenderMgr(bool _value);
    void ListenSceneUpdateMgr(bool _value);
    
    ui32 Get_NumTriangles(void);

    inline std::shared_ptr<CTexture> Get_HeightmapTexture(void)
    {
        assert(m_heightmapProcessor != nullptr);
        assert(m_heightmapProcessor->Get_HeightmapTexture() != nullptr);
        return m_heightmapProcessor->Get_HeightmapTexture();
    }

    inline f32* Get_HeightmapData(void)
    {
        assert(m_heightmapProcessor != nullptr);
        assert(m_heightmapProcessor->Get_HeightmapData() != nullptr);
        return m_heightmapProcessor->Get_HeightmapData();
    };

    inline ui32 Get_HeightmapWidth(void)
    {
        assert(m_heightmapProcessor != nullptr);
        assert(m_heightmapProcessor->Get_Width() != 0);
        return m_heightmapProcessor->Get_Width();
    };

    inline ui32 Get_HeightmapHeight(void)
    {
        assert(m_heightmapProcessor != nullptr);
        assert(m_heightmapProcessor->Get_Height() != 0);
        return m_heightmapProcessor->Get_Height();
    };
};

#endif
