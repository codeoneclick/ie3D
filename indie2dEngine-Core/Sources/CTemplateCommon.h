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
#include "HDeclaration.h"

class CTemplateAttribute
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
    
    CTemplateAttribute(bool value) { m_data.m_BOOL = value; m_metaType = E_TEMPLATE_META_TYPE_BOOL; };
    CTemplateAttribute(i8 value) { m_data.m_I8 = value; m_metaType = E_TEMPLATE_META_TYPE_I8; };
    CTemplateAttribute(ui8 value) { m_data.m_UI8 = value; m_metaType = E_TEMPLATE_META_TYPE_UI8; };
    CTemplateAttribute(i16 value) { m_data.m_I16 = value; m_metaType = E_TEMPLATE_META_TYPE_I16; };
    CTemplateAttribute(ui16 value) { m_data.m_UI16 = value; m_metaType = E_TEMPLATE_META_TYPE_UI16; };
    CTemplateAttribute(i32 value) { m_data.m_I32 = value; m_metaType = E_TEMPLATE_META_TYPE_I32; };
    CTemplateAttribute(ui32 value) { m_data.m_UI32 = value; m_metaType = E_TEMPLATE_META_TYPE_UI32; };
    CTemplateAttribute(f32 value) { m_data.m_F32 = value; m_metaType = E_TEMPLATE_META_TYPE_F32; };
    CTemplateAttribute(const char* value) { m_STRING = value; m_metaType = E_TEMPLATE_META_TYPE_STRING; };
    
    E_TEMPLATE_META_TYPE getMetaType(void) const;
    bool getBool(void) const;
    i8 getI8(void) const;
    ui8 getUI8(void) const;
    i16 getI16(void) const;
    ui16 getUI16(void) const;
    i32 getI32(void) const;
    ui32 getUI32(void) const;
    f32 getF32(void) const;
    std::string getString(void) const;
};

#if defined(__IOS__) || defined(__OSX__) || defined(__NDK__)

extern std::map<std::string, GLenum> g_stringToGLenum;
extern std::map<GLenum, std::string> g_glenumToString;

#elif defined(__WIN32__)

#endif

template <class ...Args>
std::string getTemplateAttributeKey(const Args&... args)
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

class CTemplateCommon
{
private:
    
protected:
    
    std::unordered_map<std::string, std::vector<CSharedTemplateAttribute>> m_attributes;
    std::unordered_map<std::string, std::vector<CSharedTemplateCommon>> m_templates;
    
public:
    
    CTemplateCommon(void);
    ~CTemplateCommon(void);
    
    void setAttribute(const std::string& attributeName,
                      E_TEMPLATE_META_TYPE metaType,
                      void* inValue,
                      bool isReplacing = false,
                      ui32 replacingIndex = 0);
    void setTemplate(const std::string& templateName,
                     CSharedTemplateCommonRef inTemplate,
                     bool isReplacing = false,
                     ui32 replacingIndex = 0);
};

#endif
