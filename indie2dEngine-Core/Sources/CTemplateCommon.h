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

typedef std::pair<E_TEMPLATE_META_TYPE, std::tuple<bool, i8, ui8, i16, ui16, i32, ui32, f32, std::string>>
TTemplateAttribute;

class I_RO_TemplateCommon
{
private:
    
protected:
    
public:
    
    I_RO_TemplateCommon(void);
    ~I_RO_TemplateCommon(void);
    
    virtual std::string Get_Path(void) = 0;
    virtual std::string Get_Filename(void) = 0;
};

class I_RW_TemplateCommon
{
private:
    
protected:
    
public:
    
    I_RW_TemplateCommon(void);
    ~I_RW_TemplateCommon(void);
    
    virtual void Set_Path(const std::string& _path) = 0;
    virtual void Set_Filename(const std::string& _filename) = 0;
    
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
public I_RW_TemplateCommon
{
private:
    
protected:
    
    std::string m_path;
    std::string m_filename;
    std::map<std::string, std::vector<TTemplateAttribute>> m_attributes;
    std::map<std::string, std::vector<std::shared_ptr<I_RO_TemplateCommon>>> m_templates;
    
public:
    
    CTemplateCommon(void);
    ~CTemplateCommon(void);
    
    std::string Get_Path(void);
    std::string Get_Filename(void);
    
    void Set_Path(const std::string& _path);
    void Set_Filename(const std::string& _filename);
    
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
