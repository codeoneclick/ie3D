//
//  CBatch.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 8/22/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CBatch_h
#define CBatch_h

#include "HCommon.h"

class CMaterial;
class CMesh;

class CBatch
{
private:
    
protected:
    
    std::shared_ptr<CMaterial> m_material;
    std::shared_ptr<CMesh> m_mesh;
    std::string m_guid;
    ui32 m_numIndices;
    ui32 m_numVertices;
    
public:
    
    CBatch(std::shared_ptr<CMaterial> _material);
    ~CBatch(void);
    
    inline std::string Get_Guid(void) const
    {
        return m_guid;
    };

    void Erase(void);
    void Batch(std::shared_ptr<CMesh> _mesh, const glm::mat4x4& _matrix);
    void Draw(void);
};

#endif 
