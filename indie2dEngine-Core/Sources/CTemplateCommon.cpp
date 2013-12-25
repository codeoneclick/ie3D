//
//  CTemplateCommon.cpp
//  indieEngineCore
//
//  Created by Sergey Sergeev on 12/20/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CTemplateCommon.h"

#if defined(__IOS__) || defined(__OSX__) || defined(__NDK__)

std::map<std::string, GLenum> g_stringToGLenum = {
    {"GL_FRONT", GL_FRONT},
    {"GL_BACK", GL_BACK},
    {"GL_SRC_ALPHA", GL_SRC_ALPHA},
    {"GL_ONE_MINUS_SRC_ALPHA", GL_ONE_MINUS_SRC_ALPHA},
    {"GL_REPEAT", GL_REPEAT},
    {"GL_CLAMP_TO_EDGE", GL_CLAMP_TO_EDGE },
    {"GL_MIRRORED_REPEAT", GL_MIRRORED_REPEAT }
};

std::map<GLenum, std::string> g_glenumToString = {
    {GL_FRONT, "GL_FRONT"},
    {GL_BACK, "GL_BACK"},
    {GL_SRC_ALPHA, "GL_SRC_ALPHA"},
    {GL_ONE_MINUS_SRC_ALPHA, "GL_ONE_MINUS_SRC_ALPHA"},
    {GL_REPEAT, "GL_REPEAT"},
    {GL_CLAMP_TO_EDGE, "GL_CLAMP_TO_EDGE"},
    {GL_MIRRORED_REPEAT, "GL_MIRRORED_REPEAT"}
};

#elif defined(__WIN32__)

static std::map<std::string, GLenum> g_glenumToString(void)
{
    static std::map<std::string, GLenum> container;
    if(container.size() == 0)
    {
        container.insert(std::make_pair("GL_FRONT", GL_FRONT));
        container.insert(std::make_pair("GL_BACK", GL_BACK));
        container.insert(std::make_pair("GL_SRC_ALPHA", GL_SRC_ALPHA));
        container.insert(std::make_pair("GL_ONE_MINUS_SRC_ALPHA", GL_ONE_MINUS_SRC_ALPHA));
        container.insert(std::make_pair("GL_REPEAT", GL_REPEAT));
        container.insert(std::make_pair("GL_CLAMP_TO_EDGE", GL_CLAMP_TO_EDGE));
        container.insert(std::make_pair("GL_MIRRORED_REPEAT", GL_MIRRORED_REPEAT));
    }
    return container;
};

std::map<std::string, GLenum> g_glEnumContainer = __CreateGlEnumContainer();

#endif


E_TEMPLATE_META_TYPE TTemplateAttribute::Get_MetaType(void)
{
    return m_metaType;
}

bool TTemplateAttribute::Get_Bool(void)
{
    return m_data.m_BOOL;
}

i8 TTemplateAttribute::Get_I8(void)
{
    return m_data.m_I8;
}

ui8 TTemplateAttribute::Get_UI8(void)
{
    return m_data.m_UI8;
}

i16 TTemplateAttribute::Get_I16(void)
{
    return m_data.m_I16;
}

ui16 TTemplateAttribute::Get_UI16(void)
{
    return m_data.m_UI16;
}

i32 TTemplateAttribute::Get_I32(void)
{
    return m_data.m_I32;
}

ui32 TTemplateAttribute::Get_UI32(void)
{
    return m_data.m_UI32;
}

f32 TTemplateAttribute::Get_F32(void)
{
    return m_data.m_F32;
}

std::string TTemplateAttribute::Get_String(void)
{
    return m_STRING;
}

I_RO_TemplateCommon::I_RO_TemplateCommon(void)
{
    
}

I_RO_TemplateCommon::~I_RO_TemplateCommon(void)
{
    
    
}

I_WO_TemplateCommon::I_WO_TemplateCommon(void)
{
    
}

I_WO_TemplateCommon::~I_WO_TemplateCommon(void)
{
    
}

CTemplateCommon::CTemplateCommon(void) :
I_RO_TemplateCommon(),
I_WO_TemplateCommon()
{
    
}

CTemplateCommon::~CTemplateCommon(void)
{
    
}

void CTemplateCommon::Set_Attribute(const std::string& _attributeName,
                                    E_TEMPLATE_META_TYPE _metaType,
                                    void* _value,
                                    bool _isReplacing,
                                    ui32 _replacingIndex)
{
    std::shared_ptr<TTemplateAttribute> attribute;
    switch (_metaType)
    {
        case E_TEMPLATE_META_TYPE_BOOL:
        {
            bool value = *static_cast<bool*>(_value);
            attribute = std::make_shared<TTemplateAttribute>(value);
        }
        break;
        case E_TEMPLATE_META_TYPE_I8:
        {
            i8 value = *static_cast<i8*>(_value);
            attribute = std::make_shared<TTemplateAttribute>(value);
        }
        break;
        case E_TEMPLATE_META_TYPE_UI8:
        {
            ui8 value = *static_cast<ui8*>(_value);
            attribute = std::make_shared<TTemplateAttribute>(value);
        }
        break;
        case E_TEMPLATE_META_TYPE_I16:
        {
            i16 value = *static_cast<i16*>(_value);
            attribute = std::make_shared<TTemplateAttribute>(value);
        }
        break;
        case E_TEMPLATE_META_TYPE_UI16:
        {
            ui16 value = *static_cast<ui16*>(_value);
            attribute = std::make_shared<TTemplateAttribute>(value);
        }
        break;
        case E_TEMPLATE_META_TYPE_I32:
        {
            i32 value = *static_cast<i32*>(_value);
            attribute = std::make_shared<TTemplateAttribute>(value);
        }
        break;
        case E_TEMPLATE_META_TYPE_UI32:
        {
            ui32 value = *static_cast<ui32*>(_value);
            attribute = std::make_shared<TTemplateAttribute>(value);
        }
        break;
        case E_TEMPLATE_META_TYPE_F32:
        {
            f32 value = *static_cast<f32*>(_value);
            attribute = std::make_shared<TTemplateAttribute>(value);
        }
        break;
        case E_TEMPLATE_META_TYPE_STRING:
        {
            std::string value = *static_cast<std::string*>(_value);
            attribute = std::make_shared<TTemplateAttribute>(value.c_str());
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
        std::vector<std::shared_ptr<TTemplateAttribute>> attributes;
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

