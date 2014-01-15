//
//  COceanTemplateSerializer.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 7/24/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "COceanTemplateSerializer.h"
#include "CTemplateGameObjects.h"

COceanTemplateSerializer::COceanTemplateSerializer(void)
{
    
}

COceanTemplateSerializer::~COceanTemplateSerializer(void)
{

}

std::shared_ptr<I_RO_TemplateCommon> COceanTemplateSerializer::Serialize(const std::string& _filename)
{
    pugi::xml_document document;
    pugi::xml_parse_result result = ITemplateSerializer::_LoadDocument(document, _filename);
    assert(result.status == pugi::status_ok);
    
    std::shared_ptr<COceanTemplate> oceanTemplate = std::make_shared<COceanTemplate>();
    pugi::xml_node node = document.child(oceanTemplate->kOceanMainNode.c_str());
    
    f32 sizeX = node.attribute(oceanTemplate->kOceanSizeXAttribute.c_str()).as_float();
    oceanTemplate->Set_Attribute(Get_TemplateAttributeKey(oceanTemplate->kOceanMainNode,
                                                          oceanTemplate->kOceanSizeXAttribute),
                                 E_TEMPLATE_META_TYPE_F32,
                                 &sizeX);
    
    f32 sizeY = node.attribute(oceanTemplate->kOceanSizeYAttribute.c_str()).as_float();
    oceanTemplate->Set_Attribute(Get_TemplateAttributeKey(oceanTemplate->kOceanMainNode,
                                                          oceanTemplate->kOceanSizeYAttribute),
                                 E_TEMPLATE_META_TYPE_F32,
                                 &sizeY);
    
    f32 altitude = node.attribute(oceanTemplate->kOceanAltitudeAttribute.c_str()).as_float();
    oceanTemplate->Set_Attribute(Get_TemplateAttributeKey(oceanTemplate->kOceanMainNode,
                                                          oceanTemplate->kOceanAltitudeAttribute),
                                 E_TEMPLATE_META_TYPE_F32,
                                 &altitude);
    
    f32 waveGenerationInterval = node.attribute(oceanTemplate->kOceanWaveGenerationIntervalAttribute.c_str()).as_float();
    oceanTemplate->Set_Attribute(Get_TemplateAttributeKey(oceanTemplate->kOceanMainNode,
                                                          oceanTemplate->kOceanWaveGenerationIntervalAttribute),
                                 E_TEMPLATE_META_TYPE_F32,
                                 &waveGenerationInterval);
    
    pugi::xml_node materialsNode = node.child(oceanTemplate->kGameObjectMaterialsTemplatesNode.c_str());
    for (pugi::xml_node material = materialsNode.child(oceanTemplate->kGameObjectMaterialTemplateNode.c_str());
         material;
         material = material.next_sibling(oceanTemplate->kGameObjectMaterialTemplateNode.c_str()))
    {
        std::string filename = material.attribute(oceanTemplate->kGameObjectMaterialFilenameAttribute.c_str()).as_string();
        oceanTemplate->Set_Attribute(Get_TemplateAttributeKey(oceanTemplate->kGameObjectMaterialsTemplatesNode,
                                                              oceanTemplate->kGameObjectMaterialTemplateNode,
                                                              oceanTemplate->kGameObjectMaterialFilenameAttribute),
                                     E_TEMPLATE_META_TYPE_STRING,
                                     &filename);
    }
    return oceanTemplate;
}

std::shared_ptr<I_RO_TemplateCommon> COceanTemplateSerializer::Serialize(const std::string& _host, ui32 _port, const std::string& _filename)
{
    return nullptr;
}

void COceanTemplateSerializer::Deserialize(const std::string& _filename, std::shared_ptr<I_RO_TemplateCommon> _template)
{
    
}