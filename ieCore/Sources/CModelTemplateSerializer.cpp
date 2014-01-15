//
//  CModelTemplateSerializer.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/28/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CModelTemplateSerializer.h"
#include "CTemplateGameObjects.h"

CModelTemplateSerializer::CModelTemplateSerializer(void)
{
    
}

CModelTemplateSerializer::~CModelTemplateSerializer(void)
{
    
}

std::shared_ptr<I_RO_TemplateCommon> CModelTemplateSerializer::Serialize(const std::string& _filename)
{
    pugi::xml_document document;
    pugi::xml_parse_result result = ITemplateSerializer::_LoadDocument(document, _filename);
    assert(result.status == pugi::status_ok);
    
    std::shared_ptr<CModelTemplate> modelTemplate = std::make_shared<CModelTemplate>();
    pugi::xml_node node = document.child(modelTemplate->kModelMainNode.c_str());
    
    std::string meshFilename = node.attribute(modelTemplate->kModelMeshFilenameAttribute.c_str()).as_string();
    modelTemplate->Set_Attribute(Get_TemplateAttributeKey(modelTemplate->kModelMainNode,
                                                          modelTemplate->kModelMeshFilenameAttribute),
                                 E_TEMPLATE_META_TYPE_STRING,
                                 &meshFilename);
    
    bool isBatching = node.attribute(modelTemplate->kModelMeshIsBatchingAttribute.c_str()).as_bool();
    modelTemplate->Set_Attribute(Get_TemplateAttributeKey(modelTemplate->kModelMainNode,
                                                          modelTemplate->kModelMeshIsBatchingAttribute),
                                 E_TEMPLATE_META_TYPE_BOOL,
                                 &isBatching);
    
    pugi::xml_node skeletonNode = node.child(modelTemplate->kModelSkeletonNode.c_str());
    std::string skeletonFilename = skeletonNode.attribute(modelTemplate->kModelSkeletonFilenameAttribute.c_str()).as_string();
    modelTemplate->Set_Attribute(Get_TemplateAttributeKey(modelTemplate->kModelMainNode,
                                                          modelTemplate->kModelSkeletonNode,
                                                          modelTemplate->kModelSkeletonFilenameAttribute),
                                 E_TEMPLATE_META_TYPE_STRING,
                                 &skeletonFilename);
    
    pugi::xml_node sequencesNode = node.child(modelTemplate->kModelSequencesNode.c_str());
    for (pugi::xml_node sequence = sequencesNode.child(modelTemplate->kModelSequenceNode.c_str());
         sequence;
         sequence = sequencesNode.next_sibling(modelTemplate->kModelSequenceNode.c_str()))
    {
        std::string filename = sequence.attribute(modelTemplate->kModelSequenceFilenameAttribute.c_str()).as_string();
        modelTemplate->Set_Attribute(Get_TemplateAttributeKey(modelTemplate->kModelMainNode,
                                                              modelTemplate->kModelSequencesNode,
                                                              modelTemplate->kModelSequenceNode,
                                                              modelTemplate->kModelSequenceFilenameAttribute),
                                     E_TEMPLATE_META_TYPE_STRING,
                                     &filename);
        
        
        std::string animation = sequence.attribute(modelTemplate->kModelSequenceAnimationNameAttribute.c_str()).as_string();
        modelTemplate->Set_Attribute(Get_TemplateAttributeKey(modelTemplate->kModelMainNode,
                                                              modelTemplate->kModelSequencesNode,
                                                              modelTemplate->kModelSequenceNode,
                                                              modelTemplate->kModelSequenceAnimationNameAttribute),
                                     E_TEMPLATE_META_TYPE_STRING,
                                     &filename);
        
    }
    
    pugi::xml_node materialsNode = node.child(modelTemplate->kGameObjectMaterialsTemplatesNode.c_str());
    for (pugi::xml_node material = materialsNode.child(modelTemplate->kGameObjectMaterialTemplateNode.c_str());
         material;
         material = material.next_sibling(modelTemplate->kGameObjectMaterialTemplateNode.c_str()))
    {        std::string filename = material.attribute(modelTemplate->kGameObjectMaterialFilenameAttribute.c_str()).as_string();
        modelTemplate->Set_Attribute(Get_TemplateAttributeKey(modelTemplate->kGameObjectMaterialsTemplatesNode,
                                                              modelTemplate->kGameObjectMaterialTemplateNode,
                                                              modelTemplate->kGameObjectMaterialFilenameAttribute),
                                     E_TEMPLATE_META_TYPE_STRING,
                                     &filename);
    }

    return modelTemplate;
}

std::shared_ptr<I_RO_TemplateCommon> CModelTemplateSerializer::Serialize(const std::string& _host, ui32 _port, const std::string& _filename)
{
    return nullptr;
}

void CModelTemplateSerializer::Deserialize(const std::string& _filename, std::shared_ptr<I_RO_TemplateCommon> _template)
{
    /*std::shared_ptr<SModelTemplate> modelTemplate = std::static_pointer_cast<SModelTemplate>(_template);
    pugi::xml_document document;
    document.load("<model name=\"\">\
                   <mesh filename=\"\" is_batching=\"\"/>\
                   <skeleton filename=\"\"/>\
                   <animations>\
                   <animation name=\"\" filename=\"\"/>\
                   </animations>\
                   <materials>\
                   <material filename=\"\"/>\
                   </materials>\
                   </model>");
    pugi::xml_node node = document.child("model");
    node.attribute("name").set_value(modelTemplate->m_meshFilename.c_str());
    node.child("mesh").attribute("filename").set_value(modelTemplate->m_meshFilename.c_str());
    node.child("mesh").attribute("is_batching").set_value(0);
    node.child("skeleton").attribute("filename").set_value(modelTemplate->m_skeletonFilename.c_str());
    for(auto sequence : modelTemplate->m_sequencesFilenames)
    {
        node.child("animations").append_child("animation");
        node.child("animations").child("animation").append_attribute("filename");
        node.child("animations").child("animation").attribute("filename").set_value(sequence.c_str());
    }
    for(auto material : modelTemplate->m_materialsFilenames)
    {
        node.child("materials").append_child("material");
        node.child("materials").child("material").append_attribute("filename");
        node.child("materials").child("material").attribute("filename").set_value(material.c_str());
    }*/
}
