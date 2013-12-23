//
//  CTemplateCommon.cpp
//  indieEngineCore
//
//  Created by Sergey Sergeev on 12/20/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CTemplateCommon.h"

I_RO_TemplateCommon::I_RO_TemplateCommon(void)
{
    
}

I_RO_TemplateCommon::~I_RO_TemplateCommon(void)
{
    
    
}

I_RW_TemplateCommon::I_RW_TemplateCommon(void)
{
    
}

I_RW_TemplateCommon::~I_RW_TemplateCommon(void)
{
    
}

CTemplateCommon::CTemplateCommon(void) :
I_RO_TemplateCommon(),
I_RW_TemplateCommon(),
m_path(""),
m_filename("")
{
    
}

CTemplateCommon::~CTemplateCommon(void)
{
    
}

std::string CTemplateCommon::Get_Path(void)
{
    return m_path;
}

std::string CTemplateCommon::Get_Filename(void)
{
    return m_filename;
}

void CTemplateCommon::Set_Path(const std::string &_path)
{
    m_path = _path;
}

void CTemplateCommon::Set_Filename(const std::string &_filename)
{
    m_filename = _filename;
}

void CTemplateCommon::Set_Attribute(const std::string& _attributeName,
                                    E_TEMPLATE_META_TYPE _metaType,
                                    void* _value,
                                    bool _isReplacing,
                                    ui32 _replacingIndex)
{
    TTemplateAttribute attribute;
    switch (_metaType)
    {
        case E_TEMPLATE_META_TYPE_BOOL:
        {
            bool value = *static_cast<bool*>(_value);
            attribute = std::make_pair(_metaType,
                                       std::make_tuple(value, 0, 0, 0, 0, 0, 0, 0.0f, ""));
        }
        break;
        case E_TEMPLATE_META_TYPE_I8:
        {
            i8 value = *static_cast<i8*>(_value);
            attribute = std::make_pair(_metaType,
                                       std::make_tuple(false, value, 0, 0, 0, 0, 0, 0.0f, ""));
        }
        break;
        case E_TEMPLATE_META_TYPE_UI8:
        {
            ui8 value = *static_cast<ui8*>(_value);
            attribute = std::make_pair(_metaType,
                                       std::make_tuple(false, 0, value, 0, 0, 0, 0, 0.0f, ""));
        }
        break;
        case E_TEMPLATE_META_TYPE_I16:
        {
            i16 value = *static_cast<i16*>(_value);
            attribute = std::make_pair(_metaType,
                                       std::make_tuple(false, 0, 0, value, 0, 0, 0, 0.0f, ""));
        }
        break;
        case E_TEMPLATE_META_TYPE_UI16:
        {
            ui16 value = *static_cast<ui16*>(_value);
            attribute = std::make_pair(_metaType,
                                       std::make_tuple(false, 0, 0, 0, value, 0, 0, 0.0f, ""));
        }
        break;
        case E_TEMPLATE_META_TYPE_I32:
        {
            i32 value = *static_cast<i32*>(_value);
            attribute = std::make_pair(_metaType,
                                       std::make_tuple(false, 0, 0, 0, 0, value, 0, 0.0f, ""));
        }
        break;
        case E_TEMPLATE_META_TYPE_UI32:
        {
            ui32 value = *static_cast<ui32*>(_value);
            attribute = std::make_pair(_metaType,
                                       std::make_tuple(false, 0, 0, 0, 0, 0, value, 0.0f, ""));
        }
        break;
        case E_TEMPLATE_META_TYPE_F32:
        {
            f32 value = *static_cast<f32*>(_value);
            attribute = std::make_pair(_metaType,
                                       std::make_tuple(false, 0, 0, 0, 0, 0, 0, value, ""));
        }
        break;
        case E_TEMPLATE_META_TYPE_STRING:
        {
            std::string value = *static_cast<std::string*>(_value);
            attribute = std::make_pair(_metaType,
                                       std::make_tuple(false, 0, 0, 0, 0, 0, 0, 0, value));
        }
        break;
        
        default:
        {
            assert(false);
        }
        break;
    }
    
    const auto& iterator = m_attributes.find(_attributeName);
    if(iterator != m_attributes.end())
    {
        if(_isReplacing && _replacingIndex < iterator->second.size())
        {
            iterator->second[_replacingIndex] = attribute;
        }
        else
        {
            iterator->second.push_back(attribute);
        }
    }
    else
    {
        std::vector<TTemplateAttribute> attributes;
        attributes.push_back(attribute);
        m_attributes.insert(std::make_pair(_attributeName, attributes));
    }
}

void CTemplateCommon::Set_Template(const std::string &_templateName,
                                   const std::shared_ptr<I_RO_TemplateCommon> &_template,
                                   bool _isReplacing,
                                   ui32 _replacingIndex)
{
    const auto& iterator = m_templates.find(_templateName);
    if(iterator != m_templates.end())
    {
        if(_isReplacing && _replacingIndex < iterator->second.size())
        {
            iterator->second[_replacingIndex] = _template;
        }
        else
        {
            iterator->second.push_back(_template);
        }
    }
    else
    {
        std::vector<std::shared_ptr<I_RO_TemplateCommon>> aTemplate;
        aTemplate.push_back(_template);
        m_templates.insert(std::make_pair(_templateName, aTemplate));
    }
}

