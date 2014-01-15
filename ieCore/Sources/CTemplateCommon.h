//
//  CTemplateCommon.h
//  indieEngineCore
//
//  Created by Sergey Sergeev on 12/20/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CTemplateCommon_h
#define CTemplateCommon_h

#include "HCommon.h"
#include "HEnums.h"

class TTemplateAttribute
{
private:
    
protected:
    
    E_TEMPLATE_META_TYPE m_metaType;
    union
    {
        bool        m_BOOL;
        i8          m_I8;
        ui8         m_UI8;
        i16         m_I16;
        ui16        m_UI16;
        i32         m_I32;
        ui32        m_UI32;
        f32         m_F32;
    } m_data;
    std::string m_STRING;
    
public:
    
    TTemplateAttribute(bool _value) { m_data.m_BOOL = _value; m_metaType = E_TEMPLATE_META_TYPE_BOOL; };
    TTemplateAttribute(i8 _value) { m_data.m_I8 = _value; m_metaType = E_TEMPLATE_META_TYPE_I8; };
    TTemplateAttribute(ui8 _value) { m_data.m_UI8 = _value; m_metaType = E_TEMPLATE_META_TYPE_UI8; };
    TTemplateAttribute(i16 _value) { m_data.m_I16 = _value; m_metaType = E_TEMPLATE_META_TYPE_I16; };
    TTemplateAttribute(ui16 _value) { m_data.m_UI16 = _value; m_metaType = E_TEMPLATE_META_TYPE_UI16; };
    TTemplateAttribute(i32 _value) { m_data.m_I32 = _value; m_metaType = E_TEMPLATE_META_TYPE_I32; };
    TTemplateAttribute(ui32 _value) { m_data.m_UI32 = _value; m_metaType = E_TEMPLATE_META_TYPE_UI32; };
    TTemplateAttribute(f32 _value) { m_data.m_F32 = _value; m_metaType = E_TEMPLATE_META_TYPE_F32; };
    TTemplateAttribute(const char* _value) { m_STRING = _value; m_metaType = E_TEMPLATE_META_TYPE_STRING; };
    
    E_TEMPLATE_META_TYPE Get_MetaType(void);
    bool Get_Bool(void);
    i8 Get_I8(void);
    ui8 Get_UI8(void);
    i16 Get_I16(void);
    ui16 Get_UI16(void);
    i32 Get_I32(void);
    ui32 Get_UI32(void);
    f32 Get_F32(void);
    std::string Get_String(void);
};

#if defined(__IOS__) || defined(__OSX__) || defined(__NDK__)

extern std::map<std::string, GLenum> g_stringToGLenum;
extern std::map<GLenum, std::string> g_glenumToString;

#elif defined(__WIN32__)

#endif

template <class ...Args>
std::string Get_TemplateAttributeKey(const Args&... args)
{
    std::vector<std::string> keys = {args...};
    assert(keys.size() != 0);
    std::string key = "";
    for (ui32 i = 0; i < (keys.size() - 1); ++i)
    {
        key.append(keys[i] + ":");
    }
    key.append(keys[keys.size() - 1]);
    assert(key.length() != 0);
    return key;
}

class I_RO_TemplateCommon
{
private:
    
protected:
    
public:
    
    I_RO_TemplateCommon(void);
    ~I_RO_TemplateCommon(void);
};

class I_WO_TemplateCommon
{
private:
    
protected:
    
public:
    
    I_WO_TemplateCommon(void);
    ~I_WO_TemplateCommon(void);
    
    virtual void Set_Attribute(const std::string& _attributeName,
                               E_TEMPLATE_META_TYPE _metaType,
                               void* _value,
                               bool _isReplacing = false,
                               ui32 _replacingIndex = 0) = 0;
    virtual void Set_Template(const std::string& _templateName,
                              const std::shared_ptr<I_RO_TemplateCommon>& _template,
                              bool _isReplacing = false,
                              ui32 _replacingIndex = 0) = 0;
};

class CTemplateCommon :
public I_RO_TemplateCommon,
public I_WO_TemplateCommon
{
private:
    
protected:
    
    std::map<std::string, std::vector<std::shared_ptr<TTemplateAttribute>>> m_attributes;
    std::map<std::string, std::vector<std::shared_ptr<I_RO_TemplateCommon>>> m_templates;
    
public:
    
    CTemplateCommon(void);
    ~CTemplateCommon(void);
    
    void Set_Attribute(const std::string& _attributeName,
                       E_TEMPLATE_META_TYPE _metaType,
                       void* _value,
                       bool _isReplacing = false,
                       ui32 _replacingIndex = 0);
    void Set_Template(const std::string& _templateName,
                      const std::shared_ptr<I_RO_TemplateCommon>& _template,
                      bool _isReplacing = false,
                      ui32 _replacingIndex = 0);
};

#endif
