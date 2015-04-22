//
//  CHeightmapProcessor.h
//  iGaia
//
//  Created by Sergey Sergeev on 3/3/13.
//
//

#ifndef CHeightmapProcessor_h
#define CHeightmapProcessor_h

#include "HCommon.h"
#include "HDeclaration.h"
#include "HEnums.h"
#include "CVertexBuffer.h"

class CHeightmap
{
private:
    
    static const ui8 kMaxContainsInFace = 32;
    static const ui8 kMaxContainsInVBO = 4;
    
    struct SUncomressedVertex
    {
        glm::vec3 m_position;
        glm::vec3 m_normal;
        glm::vec2 m_texcoord;
        
        std::vector<ui32> containsInFace;
        
        ui32 m_containsInFace[kMaxContainsInFace];
        ui8 m_containsInFaceSize;
        
        glm::ivec2 m_containsInVBO[kMaxContainsInVBO];
        ui8 m_containsInVBOSize;
        
        SUncomressedVertex(void) :
        m_containsInFaceSize(0),
        m_containsInVBOSize(0)
        {
            memset(m_containsInFace, 0x0, sizeof(m_containsInFace));
            memset(m_containsInVBO, 0x0, sizeof(m_containsInVBO));
        };
        ~SUncomressedVertex(void) = default;
        
        SUncomressedVertex(const SUncomressedVertex& copy) = delete;
        SUncomressedVertex(SUncomressedVertex&& copy) = delete;
        SUncomressedVertex& operator = (const SUncomressedVertex& copy) = delete;
        SUncomressedVertex& operator = (SUncomressedVertex&& copy) = delete;
    };
    
    struct SCompressedVertex
    {
        glm::vec3 m_position;
        glm::uint32 m_normal;
        glm::uint32 m_texcoord;
        
        SCompressedVertex(void) = default;
        ~SCompressedVertex(void) = default;
    };
    
    struct SFace
    {
        glm::vec3 m_normal;
        ui32 m_indexes[3];
        
        SFace(void) = default;
        ~SFace(void) = default;
        
        SFace(const SFace& copy) = delete;
        SFace(SFace&& copy) = delete;
        SFace& operator = (const SFace& copy) = delete;
        SFace& operator = (SFace&& copy) = delete;
    };
    
protected:
    
    SUncomressedVertex* m_uncompressedVertices;
    SFace* m_faces;
    SCompressedVertex* m_compressedVertices;
    
    i32 m_uncompressedVerticesFiledescriptor;
    i32 m_facesFiledescriptor;
    i32 m_compressedVerticesFiledescriptor;
    
    glm::ivec2 m_size;
    void mapVertices(f32* data);
    
public:
    
    CHeightmap(const std::string& filename, const glm::ivec2& size);
    CHeightmap(const glm::ivec2& size, f32 frequency, i32 octaves, ui32 seed);
    ~CHeightmap(void);
    
    void updateVertices(const std::vector<glm::vec3>& vertices);
    void attachUncompressedVertexToVBO(ui32 x, ui32 y, ui32 vboIndex, ui32 vboVertexIndex);
    glm::ivec2* isVertexAttachedToVBO(ui32 x, ui32 y, ui8* containsInCount);
    
    glm::vec3 getVertexPosition(ui32 i, ui32 j) const;
    glm::uint32 getVertexTexcoord(ui32 i, ui32 j) const;
    glm::uint32 getVertexNormal(ui32 i, ui32 j) const;
    
    glm::ivec2 getSize(void) const;
    
    f32 getMaxHeight(void) const;
    f32 getMinHeight(void) const;
};

class CHeightmapAccessor
{
private:
    
    static f32 getAngleOnHeightmapSurface(const glm::vec3& point_01,
                                         const glm::vec3& point_02,
                                         const glm::vec3& point_03);
    
protected:
    
public:
    
    CHeightmapAccessor(void) = default;
    ~CHeightmapAccessor(void) = default;
    
    static f32 getHeight(CSharedHeightmapRef data, const glm::vec3& position);
    static glm::vec2 getAngleOnHeightmapSurface(CSharedHeightmapRef data, const glm::vec3& position);
    static glm::vec3 getNormal(CSharedHeightmapRef data, const glm::vec3& position);
};

namespace ie
{
    class mmap_memory
    {
    private:
        
        static i32 g_filedescriptors;
        i32 m_filedescriptor;
        std::string m_filename;
        void* m_pointer;
        
    protected:
        
    public:
        
        mmap_memory(void);
        ~mmap_memory(void);
        
        void* allocate(const std::string& filename);
        void deallocate(void);
        void reallocate(void);
        
        inline void* pointer(void) const { return m_pointer; };
    };
};

class CHeightmapMMAP
{
private:
    
protected:
    
    std::shared_ptr<ie::mmap_memory> m_descriptor;
    
    ui32 m_size;
    ui32 m_offset;
    
public:
    
    CHeightmapMMAP(const std::shared_ptr<ie::mmap_memory>& descriptor);
    virtual ~CHeightmapMMAP(void) = default;
    
    inline void setSize(ui32 size) { m_size = size; };
    inline void setOffset(ui32 offset) { m_offset = offset; }
    inline ui32 getSize(void) const { return m_size; };
};

class CHeightmapIBOMMAP : public CHeightmapMMAP
{
private:

    
protected:
    
public:
    
    CHeightmapIBOMMAP(const std::shared_ptr<ie::mmap_memory>& descriptor) : CHeightmapMMAP(descriptor) { };
    ~CHeightmapIBOMMAP(void) = default;
    
    inline ui16* getSourcePointer(void) const
    {
        ui16* pointer = (ui16* )m_descriptor->pointer();
        assert(pointer != nullptr);
        
        return pointer + m_offset;
    };
    
    inline ui16* getOriginPointer(void) const
    {
        ui16* pointer = (ui16* )m_descriptor->pointer();
        assert(pointer != nullptr);
        
        return pointer + m_offset + m_size;
    };
    
    inline void updateSourcePointer(void)
    {
        ui16* sourcePointer = CHeightmapIBOMMAP::getSourcePointer();
        ui16* originPointer = CHeightmapIBOMMAP::getOriginPointer();
        memcpy(sourcePointer, originPointer, m_size * sizeof(ui16));
    }
};

class CHeightmapVBOMMAP : public CHeightmapMMAP
{
private:
    
protected:
    
public:
    
    CHeightmapVBOMMAP(const std::shared_ptr<ie::mmap_memory>& descriptor) : CHeightmapMMAP(descriptor) { };
    ~CHeightmapVBOMMAP(void) = default;
    
    inline SAttributeVertex* getPointer(void) const
    {
        SAttributeVertex* pointer = (SAttributeVertex* )m_descriptor->pointer();
        assert(pointer != nullptr);

        return pointer + m_offset;
    };
};

class CHeightmapTextureMMAP : public CHeightmapMMAP
{
private:
    
protected:
    
public:
    
    CHeightmapTextureMMAP(const std::shared_ptr<ie::mmap_memory>& descriptor) : CHeightmapMMAP(descriptor) { };
    ~CHeightmapTextureMMAP(void) = default;
    
    inline ui16* getPointer(void) const
    {
        ui16* pointer = (ui16* )m_descriptor->pointer();
        assert(pointer != nullptr);
        
        return pointer + m_offset;
    };
};

class CHeightmapGenerator
{
private:
    
    static const ui8 kMaxChunkSize = 65;
    ui32 m_heightmapGUID;
    
protected:
    
    std::shared_ptr<CHeightmap> m_heightmap;
    
    std::shared_ptr<ie::mmap_memory> m_vbosMMAPDescriptor;
    std::shared_ptr<ie::mmap_memory> m_ibosMMAPDescriptor;
    std::shared_ptr<ie::mmap_memory> m_texturesMMAPDescriptor;
    
    std::vector<std::shared_ptr<CHeightmapVBOMMAP>> m_vbosMMAP;
    std::vector<std::array<std::shared_ptr<CHeightmapIBOMMAP>, E_LANDSCAPE_CHUNK_LOD_MAX>> m_ibosMMAP;
    std::vector<std::shared_ptr<CHeightmapTextureMMAP>> m_texturesMMAP;
    
    std::vector<std::tuple<std::function<void(CSharedMeshRef)>, std::function<void(CSharedQuadTreeRef)>>> m_callbacks;
    std::vector<std::tuple<CSharedMesh, CSharedQuadTree, E_LANDSCAPE_CHUNK_LOD>> m_chunksMetadata;
    std::vector<std::tuple<glm::vec3, glm::vec3>> m_chunksBounds;
    
    std::string createVBOs(void);
    std::string createIBOs(void);
    std::string createTextures(void);
    
    CSharedTexture m_heightmapTexture;
    CSharedTexture m_splattingTexture;
    
    glm::ivec2 m_chunkSize;
    glm::ivec2 m_chunksNum;
    std::array<glm::ivec2, E_LANDSCAPE_CHUNK_LOD_MAX> m_chunkLODsSizes;
    
    std::vector<CSharedThreadOperation> m_executedOperations;
    
    ISharedRenderTechniqueAccessor m_renderTechniqueAccessor;
    
    void initContainers(const std::shared_ptr<CHeightmap>& heightmap);
    void createMesh(ui32 index, E_LANDSCAPE_CHUNK_LOD LOD);
    void generateQuadTree(ui32 index);
    CSharedTexture generateDiffuseTexture(ui32 index, CSharedMaterialRef material);
    
    void createChunkBound(const glm::ivec2& index,
                          glm::vec3* minBound, glm::vec3* maxBound);
    
    void updateSplattingTexture(CSharedTextureRef texture, bool isCreation = true,
                                ui32 offsetX = 0, ui32 offsetY = 0,
                                ui32 subWidth = 0, ui32 subHeight = 0);
    
    void updateHeightmapTexture(CSharedTextureRef texture, bool isCreation = true,
                                ui32 offsetX = 0, ui32 offsetY = 0,
                                ui32 subWidth = 0, ui32 subHeight = 0);
    
    ui32 createTextureId(void);
    
    
    static void getTriangleBasis(const glm::vec3& E, const glm::vec3& F, const glm::vec3& G,
                                 f32 sE, f32 tE, f32 sF, f32 tF, f32 sG, f32 tG,
                                 glm::vec3& tangentX, glm::vec3& tangentY);
    static glm::vec3 getClosestPointOnLine(const glm::vec3& a, const glm::vec3& b, const glm::vec3& p);
    static glm::vec3 ortogonalize(const glm::vec3& v1, const glm::vec3& v2);
    
    void eraseChunkMetadata(ui32 index);
    
public:
    
    CHeightmapGenerator(ISharedRenderTechniqueAccessorRef renderTechniqueAccessor, ISharedConfigurationRef configuration);
    ~CHeightmapGenerator(void);
    
    CSharedTexture createHeightmapTexture(void);
    CSharedTexture createSplattingTexture(void);
    
    void generateVertices(const glm::ivec2& size, f32 frequency, i32 octaves, ui32 seed);
    
    void generateTangentSpace(CSharedVertexBufferRef vertexBuffer,
                              CSharedIndexBufferRef indexBuffer);
    
    glm::ivec2 getSize(void) const;
    glm::ivec2 getNumChunks(void) const;
    
    void update(void);
    
    void runChunkLoading(ui32 i, ui32 j, E_LANDSCAPE_CHUNK_LOD LOD,
                         CSharedMaterialRef preprocessSplattingTextureMaterial,
                         const std::function<void(CSharedMeshRef)>& meshCreatedCallback,
                         const std::function<void(CSharedQuadTreeRef)>& quadTreeGeneratedCallback,
                         const std::function<void(CSharedTextureRef)>& textureGeneratedCallback);
    void runChunkUnLoading(ui32 i, ui32 j);
    
    const std::tuple<glm::vec3, glm::vec3> getChunkBounds(ui32 i, ui32 j) const;
    
    f32 getHeight(const glm::vec3& position) const;
    glm::vec2 getAngleOnHeightmapSurface(const glm::vec3& position) const;
    
    void updateHeightmap(const glm::ivec2& minBound, const glm::ivec2& maxBound,
                         const std::vector<glm::vec3>& vertices);
};

#endif 
