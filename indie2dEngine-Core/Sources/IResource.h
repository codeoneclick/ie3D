//
//  IResource.h
//  gEngine-Core
//
//  Created by Sergey Sergeev on 5/7/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef IResource_h
#define IResource_h

#include "HCommon.h"
#include "HEnums.h"
#include "HDeclaration.h"

class IResourceData : public std::enable_shared_from_this<IResourceData>
{
private:
    
protected:
    
    E_RESOURCE_DATA_CLASS m_resourceDataClass;
    IResourceData(E_RESOURCE_DATA_CLASS resourceDataClass);
    
public:
    
    ~IResourceData(void);
    
    E_RESOURCE_DATA_CLASS getResourceDataClass(void) const;
};

class IResourceLoadingHandler;
class IResource : public std::enable_shared_from_this<IResource>
{
private:
    
    friend class IResourceCommiter;
    friend class IResourceSerializer;
    
    friend class CTextureLoadingOperation;
    friend class CShaderLoadingOperation;
    friend class CMeshLoadingOperation;
    friend class CSkeletonLoadingOperation;
    friend class CSequenceLoadingOperation;
    
protected:
    
    std::string m_guid;
    E_RESOURCE_CLASS m_class;
    ui8 m_status;
    
    std::set<std::shared_ptr<IResourceLoadingHandler> > m_handlers;
    
    void _OnLoaded(void);
    
    IResource(E_RESOURCE_CLASS resourceClass,
              const std::string& guid);
    
    virtual void onResourceDataSerialized(ISharedResourceDataRef resourceData,
                                          E_RESOURCE_DATA_STATUS status) = 0;
    
    virtual void onResourceDataCommited(ISharedResourceDataRef resourceData,
                                        E_RESOURCE_DATA_STATUS status) = 0;
    
public:
    
    virtual ~IResource(void);
    
    inline const std::string& Get_Guid(void) const
    {
        return m_guid;
    };
    
    inline E_RESOURCE_CLASS Get_Class(void) const
    {
        return m_class;
    };
    
    inline virtual bool IsLoaded(void) const
    {
		const bool value = 0 != (m_status & E_RESOURCE_STATUS_LOADED);
		return value;
    };
    
    inline virtual bool IsCommited(void) const
    {
		const bool value = 0 != (m_status & E_RESOURCE_STATUS_COMMITED);
		return value;
    };
    
    void Register_LoadingHandler(const std::shared_ptr<IResourceLoadingHandler>& _handler);
    void Unregister_LoadingHandler(const std::shared_ptr<IResourceLoadingHandler>& _handler);
};

#endif 


