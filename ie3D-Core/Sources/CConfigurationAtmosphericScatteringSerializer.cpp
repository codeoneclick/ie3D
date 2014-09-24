//
//  CConfigurationAtmosphericScatteringSerializer.cpp
//  ie3D-Core
//
//  Created by sergey.sergeev on 9/24/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//

#include "CConfigurationAtmosphericScatteringSerializer.h"
#include "CConfigurationGameObjects.h"

CConfigurationAtmosphericScatteringSerializer::CConfigurationAtmosphericScatteringSerializer(void)
{
    
}

CConfigurationAtmosphericScatteringSerializer::~CConfigurationAtmosphericScatteringSerializer(void)
{
    
}

ISharedConfiguration CConfigurationAtmosphericScatteringSerializer::serialize(const std::string& filename)
{
    pugi::xml_document document;
    pugi::xml_parse_result result = IConfigurationSerializer::openXMLDocument(document, filename);
    assert(result.status == pugi::status_ok);
    
    std::shared_ptr<CConfigurationAtmosphericScattering> atmosphericScatteringConfiguration = std::make_shared<CConfigurationAtmosphericScattering>();
    pugi::xml_node node = document.child(atmosphericScatteringConfiguration->kAtmosphericScatteringMainNode.c_str());
    
    i32 numCols = node.attribute(atmosphericScatteringConfiguration->kAtmosphericScatteringNumColsAttribute.c_str()).as_int();
    atmosphericScatteringConfiguration->setAttribute(getConfigurationAttributeKey(atmosphericScatteringConfiguration->kAtmosphericScatteringMainNode,
                                                                                  atmosphericScatteringConfiguration->kAtmosphericScatteringNumColsAttribute),
                                                     std::make_shared<CConfigurationAttribute>(numCols));
    
    i32 numRows = node.attribute(atmosphericScatteringConfiguration->kAtmosphericScatteringNumRowsAttribute.c_str()).as_int();
    atmosphericScatteringConfiguration->setAttribute(getConfigurationAttributeKey(atmosphericScatteringConfiguration->kAtmosphericScatteringMainNode,
                                                                                  atmosphericScatteringConfiguration->kAtmosphericScatteringNumRowsAttribute),
                                                     std::make_shared<CConfigurationAttribute>(numRows));
    
    i32 size = node.attribute(atmosphericScatteringConfiguration->kAtmosphericScatteringSizeAttribute.c_str()).as_int();
    atmosphericScatteringConfiguration->setAttribute(getConfigurationAttributeKey(atmosphericScatteringConfiguration->kAtmosphericScatteringMainNode,
                                                                                  atmosphericScatteringConfiguration->kAtmosphericScatteringSizeAttribute),
                                                     std::make_shared<CConfigurationAttribute>(size));
    
    i32 numSamples = node.attribute(atmosphericScatteringConfiguration->kAtmosphericScatteringNumSamplesAttribute.c_str()).as_int();
    atmosphericScatteringConfiguration->setAttribute(getConfigurationAttributeKey(atmosphericScatteringConfiguration->kAtmosphericScatteringMainNode,
                                                                                  atmosphericScatteringConfiguration->kAtmosphericScatteringNumSamplesAttribute),
                                                     std::make_shared<CConfigurationAttribute>(numSamples));
    
    f32 innerRadius = node.attribute(atmosphericScatteringConfiguration->kAtmosphericScatteringInnerRadiusAttribute.c_str()).as_float();
    atmosphericScatteringConfiguration->setAttribute(getConfigurationAttributeKey(atmosphericScatteringConfiguration->kAtmosphericScatteringMainNode,
                                                                                  atmosphericScatteringConfiguration->kAtmosphericScatteringInnerRadiusAttribute),
                                                     std::make_shared<CConfigurationAttribute>(innerRadius));
    
    f32 outerRadius = node.attribute(atmosphericScatteringConfiguration->kAtmosphericScatteringOuterRadiusAttribute.c_str()).as_float();
    atmosphericScatteringConfiguration->setAttribute(getConfigurationAttributeKey(atmosphericScatteringConfiguration->kAtmosphericScatteringMainNode,
                                                                                  atmosphericScatteringConfiguration->kAtmosphericScatteringOuterRadiusAttribute),
                                                     std::make_shared<CConfigurationAttribute>(outerRadius));
    
    f32 krCoefficient = node.attribute(atmosphericScatteringConfiguration->kAtmosphericScatteringKrCoefficientAttribute.c_str()).as_float();
    atmosphericScatteringConfiguration->setAttribute(getConfigurationAttributeKey(atmosphericScatteringConfiguration->kAtmosphericScatteringMainNode,
                                                                                  atmosphericScatteringConfiguration->kAtmosphericScatteringKrCoefficientAttribute),
                                                     std::make_shared<CConfigurationAttribute>(krCoefficient));
    
    f32 kmCoefficient = node.attribute(atmosphericScatteringConfiguration->kAtmosphericScatteringKmCoefficientAttribute.c_str()).as_float();
    atmosphericScatteringConfiguration->setAttribute(getConfigurationAttributeKey(atmosphericScatteringConfiguration->kAtmosphericScatteringMainNode,
                                                                                  atmosphericScatteringConfiguration->kAtmosphericScatteringKmCoefficientAttribute),
                                                     std::make_shared<CConfigurationAttribute>(kmCoefficient));
    
    f32 esunCoefficient = node.attribute(atmosphericScatteringConfiguration->kAtmosphericScatteringESunCoefficientAttribute.c_str()).as_float();
    atmosphericScatteringConfiguration->setAttribute(getConfigurationAttributeKey(atmosphericScatteringConfiguration->kAtmosphericScatteringMainNode,
                                                                                  atmosphericScatteringConfiguration->kAtmosphericScatteringESunCoefficientAttribute),
                                                     std::make_shared<CConfigurationAttribute>(esunCoefficient));
    
    f32 gCoefficient = node.attribute(atmosphericScatteringConfiguration->kAtmosphericScatteringGCoefficientAttribute.c_str()).as_float();
    atmosphericScatteringConfiguration->setAttribute(getConfigurationAttributeKey(atmosphericScatteringConfiguration->kAtmosphericScatteringMainNode,
                                                                                  atmosphericScatteringConfiguration->kAtmosphericScatteringGCoefficientAttribute),
                                                     std::make_shared<CConfigurationAttribute>(gCoefficient));
    
    f32 rayleighScaleDepthCoefficient = node.attribute(atmosphericScatteringConfiguration->kAtmosphericScatteringRayleighScaleDepthCoefficientAttribute.c_str()).as_float();
    atmosphericScatteringConfiguration->setAttribute(getConfigurationAttributeKey(atmosphericScatteringConfiguration->kAtmosphericScatteringMainNode,
                                                                                  atmosphericScatteringConfiguration->kAtmosphericScatteringRayleighScaleDepthCoefficientAttribute),
                                                     std::make_shared<CConfigurationAttribute>(rayleighScaleDepthCoefficient));
    
    f32 waveLengthX = node.attribute(atmosphericScatteringConfiguration->kAtmosphericScatteringWaveLengthXAttribute.c_str()).as_float();
    atmosphericScatteringConfiguration->setAttribute(getConfigurationAttributeKey(atmosphericScatteringConfiguration->kAtmosphericScatteringMainNode,
                                                                                  atmosphericScatteringConfiguration->kAtmosphericScatteringWaveLengthXAttribute),
                                                     std::make_shared<CConfigurationAttribute>(waveLengthX));
    
    f32 waveLengthY = node.attribute(atmosphericScatteringConfiguration->kAtmosphericScatteringWaveLengthYAttribute.c_str()).as_float();
    atmosphericScatteringConfiguration->setAttribute(getConfigurationAttributeKey(atmosphericScatteringConfiguration->kAtmosphericScatteringMainNode,
                                                                                  atmosphericScatteringConfiguration->kAtmosphericScatteringWaveLengthYAttribute),
                                                     std::make_shared<CConfigurationAttribute>(waveLengthY));
    
    f32 waveLengthZ = node.attribute(atmosphericScatteringConfiguration->kAtmosphericScatteringWaveLengthZAttribute.c_str()).as_float();
    atmosphericScatteringConfiguration->setAttribute(getConfigurationAttributeKey(atmosphericScatteringConfiguration->kAtmosphericScatteringMainNode,
                                                                                  atmosphericScatteringConfiguration->kAtmosphericScatteringWaveLengthZAttribute),
                                                     std::make_shared<CConfigurationAttribute>(waveLengthZ));
    
    pugi::xml_node materialsNode = node.child(atmosphericScatteringConfiguration->kGameObjectMaterialsConfigurationsNode.c_str());
    for (pugi::xml_node material = materialsNode.child(atmosphericScatteringConfiguration->kGameObjectMaterialConfigurationNode.c_str());
         material;
         material = material.next_sibling(atmosphericScatteringConfiguration->kGameObjectMaterialConfigurationNode.c_str()))
    {
        std::string filename = material.attribute(atmosphericScatteringConfiguration->kGameObjectMaterialFilenameAttribute.c_str()).as_string();
        atmosphericScatteringConfiguration->setAttribute(getConfigurationAttributeKey(atmosphericScatteringConfiguration->kGameObjectMaterialsConfigurationsNode,
                                                                                      atmosphericScatteringConfiguration->kGameObjectMaterialConfigurationNode,
                                                                                      atmosphericScatteringConfiguration->kGameObjectMaterialFilenameAttribute),
                                                         std::make_shared<CConfigurationAttribute>(filename));
    }
    return atmosphericScatteringConfiguration;
}

void CConfigurationAtmosphericScatteringSerializer::deserialize(const std::string& filename,
                                                 ISharedConfigurationRef configuration)
{
    
}