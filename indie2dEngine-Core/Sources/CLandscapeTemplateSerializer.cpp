//
//  CLandscapeTemplateSerializer.cpp
//  indieEngineCore
//
//  Created by Sergey Sergeev on 11/25/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CLandscapeTemplateSerializer.h"
#include "CTemplateGameObjects.h"

CLandscapeTemplateSerializer::CLandscapeTemplateSerializer(void)
{
    
}

CLandscapeTemplateSerializer::~CLandscapeTemplateSerializer(void)
{
    
}

std::shared_ptr<I_RO_TemplateCommon> CLandscapeTemplateSerializer::Serialize(const std::string& _filename)
{
    pugi::xml_document document;
    pugi::xml_parse_result result = ITemplateSerializer::_LoadDocument(document, _filename);
    assert(result.status == pugi::status_ok);
    
    std::shared_ptr<CLandscapeTemplate> landscapeTemplate = std::make_shared<CLandscapeTemplate>();
    pugi::xml_node node = document.child(landscapeTemplate->kLandscapeMainNode.c_str());
    
    f32 sizeX = node.attribute(landscapeTemplate->kLandscapeSizeXAttribute.c_str()).as_float();
    landscapeTemplate->Set_Attribute(Get_TemplateAttributeKey(landscapeTemplate->kLandscapeMainNode,
                                                              landscapeTemplate->kLandscapeSizeXAttribute),
                                     E_TEMPLATE_META_TYPE_F32,
                                     &sizeX);
    
    f32 sizeY = node.attribute(landscapeTemplate->kLandscapeSizeYAttribute.c_str()).as_float();
    landscapeTemplate->Set_Attribute(Get_TemplateAttributeKey(landscapeTemplate->kLandscapeMainNode,
                                                              landscapeTemplate->kLandscapeSizeYAttribute),
                                     E_TEMPLATE_META_TYPE_F32,
                                     &sizeY);

    std::string heightmapDataFilename = node.attribute(landscapeTemplate->kLandscapeHeightmapDataFilenameAttribute.c_str()).as_string();
    landscapeTemplate->Set_Attribute(Get_TemplateAttributeKey(landscapeTemplate->kLandscapeMainNode,
                                                              landscapeTemplate->kLandscapeHeightmapDataFilenameAttribute),
                                     E_TEMPLATE_META_TYPE_STRING,
                                     &heightmapDataFilename);
    
    std::string splattingDataFilename = node.attribute(landscapeTemplate->kLandscapeSplattingDataFilenameAttribute.c_str()).as_string();
    landscapeTemplate->Set_Attribute(Get_TemplateAttributeKey(landscapeTemplate->kLandscapeMainNode,
                                                              landscapeTemplate->kLandscapeSplattingDataFilenameAttribute),
                                     E_TEMPLATE_META_TYPE_STRING,
                                     &splattingDataFilename);
    
    std::string splattingDiffuseMaterialFilename = node.attribute(landscapeTemplate->kLandscapeSplattingDiffuseMaterialFilenameAttribute.c_str()).as_string();
    landscapeTemplate->Set_Attribute(Get_TemplateAttributeKey(landscapeTemplate->kLandscapeMainNode,
                                                              landscapeTemplate->kLandscapeSplattingDiffuseMaterialFilenameAttribute),
                                     E_TEMPLATE_META_TYPE_STRING,
                                     &splattingDiffuseMaterialFilename);
    
    std::string splattingNormalMaterialFilename = node.attribute(landscapeTemplate->kLandscapeSplattingNormalMaterialFilenameAttribute.c_str()).as_string();
    landscapeTemplate->Set_Attribute(Get_TemplateAttributeKey(landscapeTemplate->kLandscapeMainNode,
                                                              landscapeTemplate->kLandscapeSplattingNormalMaterialFilenameAttribute),
                                     E_TEMPLATE_META_TYPE_STRING,
                                     &splattingNormalMaterialFilename);
    
    bool isEdgesEnabled = node.attribute(landscapeTemplate->kLandscapeIsEdgesEnabledAttribute.c_str()).as_bool();
    landscapeTemplate->Set_Attribute(Get_TemplateAttributeKey(landscapeTemplate->kLandscapeMainNode,
                                                              landscapeTemplate->kLandscapeIsEdgesEnabledAttribute),
                                     E_TEMPLATE_META_TYPE_BOOL,
                                     &isEdgesEnabled);
    
    pugi::xml_node edgesNode = node.child(landscapeTemplate->kLandscapeEdgesNode.c_str());
    
    f32 edgesSizeX = edgesNode.attribute(landscapeTemplate->kLandscapeEdgesSizeXAttribute.c_str()).as_float();
    landscapeTemplate->Set_Attribute(Get_TemplateAttributeKey(landscapeTemplate->kLandscapeMainNode,
                                                              landscapeTemplate->kLandscapeEdgesNode,
                                                              landscapeTemplate->kLandscapeEdgesSizeXAttribute),
                                     E_TEMPLATE_META_TYPE_F32,
                                     &edgesSizeX);
    
    f32 edgesSizeY = edgesNode.attribute(landscapeTemplate->kLandscapeEdgesSizeYAttribute.c_str()).as_float();
    landscapeTemplate->Set_Attribute(Get_TemplateAttributeKey(landscapeTemplate->kLandscapeMainNode,
                                                              landscapeTemplate->kLandscapeEdgesNode,
                                                              landscapeTemplate->kLandscapeEdgesSizeYAttribute),
                                     E_TEMPLATE_META_TYPE_F32,
                                     &edgesSizeY);
    
    pugi::xml_node edgesMaterialsNode = edgesNode.child(landscapeTemplate->kLandscapeEdgesMaterialsTemplatesNode.c_str());
    for (pugi::xml_node material = edgesMaterialsNode.child(landscapeTemplate->kLandscapeEdgeMaterialTemplateNode.c_str());
         material;
         material = material.next_sibling(landscapeTemplate->kLandscapeEdgeMaterialTemplateNode.c_str()))
    {
        std::string filename = material.attribute(landscapeTemplate->kLandscapeEdgeMaterialTemplateFilenameAttribute.c_str()).as_string();
        landscapeTemplate->Set_Attribute(Get_TemplateAttributeKey(landscapeTemplate->kLandscapeMainNode,
                                                                  landscapeTemplate->kLandscapeEdgesNode,
                                                                  landscapeTemplate->kLandscapeEdgesMaterialsTemplatesNode,
                                                                  landscapeTemplate->kLandscapeEdgeMaterialTemplateNode,
                                                                  landscapeTemplate->kLandscapeEdgeMaterialTemplateFilenameAttribute),
                                         E_TEMPLATE_META_TYPE_STRING,
                                         &filename);
    }

    
    pugi::xml_node materialsNode = node.child(landscapeTemplate->kGameObjectMaterialsTemplatesNode.c_str());
    for (pugi::xml_node material = materialsNode.child(landscapeTemplate->kGameObjectMaterialTemplateNode.c_str());
         material;
         material = material.next_sibling(landscapeTemplate->kGameObjectMaterialTemplateNode.c_str()))
    {
        std::string filename = material.attribute(landscapeTemplate->kGameObjectMaterialFilenameAttribute.c_str()).as_string();
        landscapeTemplate->Set_Attribute(Get_TemplateAttributeKey(landscapeTemplate->kGameObjectMaterialsTemplatesNode,
                                                                        landscapeTemplate->kGameObjectMaterialTemplateNode,
                                                                        landscapeTemplate->kGameObjectMaterialFilenameAttribute),
                                               E_TEMPLATE_META_TYPE_STRING,
                                               &filename);
    }
    
    return landscapeTemplate;
}

std::shared_ptr<I_RO_TemplateCommon> CLandscapeTemplateSerializer::Serialize(const std::string& _host, ui32 _port, const std::string& _filename)
{
    return nullptr;
}

void CLandscapeTemplateSerializer::Deserialize(const std::string& _filename, std::shared_ptr<I_RO_TemplateCommon> _template)
{
    
}
