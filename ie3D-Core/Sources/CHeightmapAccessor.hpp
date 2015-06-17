//
//  CHeightmapAccessor.hpp
//  ie3D-Core
//
//  Created by sergey.sergeev on 5/14/15.
//  Copyright (c) 2015 Sergey Sergeev. All rights reserved.
//

#include "CHeightmapContainer.h"

inline glm::ivec2 CHeightmapAccessor::getMainSize(void) const
{
    assert(m_container != nullptr);
    return m_container ? m_container->getMainSize() : glm::ivec2(0);
}

inline glm::ivec2 CHeightmapAccessor::getChunksNum(void) const
{
    assert(m_container != nullptr);
    return m_container ? m_container->getChunksNum() : glm::ivec2(0);
}

inline glm::ivec2 CHeightmapAccessor::getChunkSize(void) const
{
    assert(m_container != nullptr);
    return m_container ? m_container->getChunkSize() : glm::ivec2(0);
}

inline const std::tuple<glm::vec3, glm::vec3> CHeightmapAccessor::getChunkBounds(ui32 i, ui32 j) const
{
    i32 index = i + j * CHeightmapAccessor::getChunksNum().x;
    assert(index >= 0 && index < m_chunksBounds.size());
    return m_chunksBounds[index];
}

inline CSharedHeightmapGeneratorStatistic CHeightmapAccessor::getGeneratorStatistic(void) const
{
    return m_generatorStatistic;
}