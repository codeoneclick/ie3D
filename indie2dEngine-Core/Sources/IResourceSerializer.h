//
//  IResourceSerializer.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/16/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef IResourceSerializer_h
#define IResourceSerializer_h

#include "HCommon.h"
#include "HDeclaration.h"
#include "HEnums.h"

class IResourceSerializer : public std::enable_shared_from_this<IResourceSerializer>
{
private:
    
protected:
    
    std::string m_guid;
    ISharedResource m_resource;
    E_SERIALIZER_STATUS m_status;
    
#if defined(__NDK__)
    
    const AAssetManager* m_assetManager;
    
#endif
    
    std::shared_ptr<std::istream> openStream(const std::string &filename);
    void closeStream(const std::shared_ptr<std::istream>& stream);
    
    void onResourceDataSerializationStatusChanged(ISharedResourceDataRef resourceData, E_RESOURCE_DATA_STATUS status);
    
public:
    
    IResourceSerializer(const std::string& guid,
                        ISharedResourceRef resource);
    
#if defined(__NDK__)
    
    IResourceSerializer(const std::string& guid,
                        ISharedResourceRef resource,
                        const AAssetManager* assetManager);
    
#endif
    
    virtual ~IResourceSerializer(void);
    
    std::string getGuid(void) const;
    E_SERIALIZER_STATUS getStatus(void) const;
    
    virtual void serialize(void) = 0;
};

#endif 
