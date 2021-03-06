//
//  HCommon.h
//  indi2dEngine-Core
//
//  Created by Sergey Sergeev on 5/7/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef HCommon_h
#define HCommon_h

#include <map>
#include <set>
#include <list>
#include <queue>
#include <string>
#include <vector>
#include <array>
#include <numeric>
#include <future>
#include <algorithm>
#include <mutex>
#include <thread>
#include <fstream>
#include <strstream>
#include <sstream>
#include <iostream>
#include <assert.h>
#include <functional>
#include <cmath>
#include <random>
#include <unordered_map>
#include <unordered_set>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

#include "HGLCommands.hpp"

#include <libpng-1.6.9/png.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_precision.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <pugixml/pugixml.hpp>

#include <box2d/Box2D.h>

#define STRING_SHADER(SHADER)  #SHADER
#include "HShaders.h"

typedef signed char i8;
typedef unsigned char ui8;
typedef signed short i16;
typedef unsigned short ui16;
typedef signed int i32;
typedef unsigned int ui32;
typedef long long i64;
typedef unsigned long long ui64;
typedef float f32;

namespace std
{
    class memstream: public std::istream
    {
    protected:
        
        char* m_data;
        size_t m_size;
        
    public:
        
        memstream(char* data, size_t size):
        std::istream(&m_buffer),
        m_data(data),
        m_size(size),
        m_buffer(data, size)
        {
            rdbuf(&m_buffer);
        };
        
        inline size_t size(void)
        {
            return m_size;
        };
        
        inline char* data(void)
        {
            return m_data;
        };
        
    private:
        class membuf : public std::streambuf
        {
        public:

            membuf(char* data, size_t size)
            {
                setp(data, data + size);
                setg(data, data, data + size);
            };
        };
        membuf m_buffer;
    };
    
    template<class TValue, typename TGetter = std::function<TValue(const TValue&)>, typename TSetter = std::function<void(TValue&, const TValue&)>>
    class property
    {
    private:
        
        TGetter m_getter;
        TSetter m_setter;
        
    protected:
        
    public:
        
        property(void) :
        m_getter(nullptr),
        m_setter(nullptr),
        ivar(NULL)
        {
            
        };
        
        void init(TValue value, TGetter getter = nullptr, TSetter setter = nullptr)
        {
            ivar = value;
            m_getter = getter;
            m_setter = setter;
        };
        
        inline void getter(TGetter getter)
        {
            m_getter = getter;
        };
        
        inline void setter(TSetter setter)
        {
            m_setter = setter;
        }
        
        inline operator TValue()
        {
            if(m_getter)
            {
                return m_getter(ivar);
            }
            return ivar;
        };
        
        inline TValue operator= (const TValue& value)
        {
            if(m_setter)
            {
                m_setter(ivar, value);
            }
            else
            {
                ivar = value;
            }
            ivar = value;
            return ivar;
        };
        
        TValue ivar;
    };
    
    template<class TValue, typename TGetter = std::function<TValue(void)>>
    class property_readonly
    {
    private:
        
        TValue m_value;
        TGetter m_getter;
        
    protected:
        
        
    public:
        
        property_readonly(void) :
        m_value(NULL),
        m_getter(nullptr)
        {
            
        };
        
        void init(TValue value, TGetter getter = nullptr)
        {
            m_value = value;
            m_getter = getter;
        };
        
        __attribute__((always_inline)) operator TValue()
        {
            if(m_getter)
            {
                return m_getter();
            }
            return m_value;
        };
        
        __attribute__((always_inline)) TValue operator= (const TValue&) = delete;
    };
};

namespace glm
{
    inline glm::quat slerp(const glm::quat& _from, const glm::quat& _to, f32 _interpolation)
    {
        glm::quat value;
        f32 omega, cosom, sinom, scale0, scale1;
        
        if (_interpolation <= 0.0f)
        {
            return _from;
        }
        else if (_interpolation >= 1.0f)
        {
            return _to;
        }
        
        cosom = _from.x * _to.x + _from.y * _to.y + _from.z * _to.z + _from.w * _to.w;
        if ( cosom < 0.0f )
        {
            value  = -_to;
            cosom = -cosom;
        }
        else
        {
            value = _to;
        }
        
        if ((1.0f - cosom) > 1e-6f)
        {
            omega  = acosf(cosom);
            sinom  = 1.0f / sinf(omega);
            scale0 = sinf((1.0f - _interpolation) * omega) * sinom;
            scale1 = sinf(_interpolation * omega) * sinom;
        }
        else
        {
            scale0 = 1.0f - _interpolation;
            scale1 = _interpolation;
        }
        
        return ((_from * scale0) + (value * scale1));
    };
    
    inline glm::vec3 transform(const glm::vec3 &vertex, const glm::mat4x4 &matrix)
    {
        glm::vec4 result = matrix * glm::vec4(vertex.x, vertex.y, vertex.z, 1.0f);
        return glm::vec3(result.x, result.y, result.z);
    };
    
    inline glm::vec4 transform(const glm::vec4 &vertex, const glm::mat4x4 &matrix)
    {
        glm::vec4 result = matrix * glm::vec4(vertex.x, vertex.y, vertex.z, 1.0f);
        return glm::vec4(result.x, result.y, result.z, vertex.w);
    };
    
    struct ray
    {
    protected:
        
        glm::vec3 m_origin;
        glm::vec3 m_direction;
        glm::vec3 m_invertedDirection;
        i32 m_signs[3];
        
    public:
        ray(void) : m_origin(0.0),
                    m_direction(0.0)
        {
            
        };
        ray(const glm::vec3& origin, const glm::vec3& direction)
        {
            m_origin = origin;
            ray::setDirection(direction);
        };
        
        ray(const ray &copy)
        {
            m_origin = copy.m_origin;
            m_direction = copy.m_direction;
            m_invertedDirection = copy.m_invertedDirection;
            m_signs[0] = copy.m_signs[0]; m_signs[1] = copy.m_signs[1]; m_signs[2] = copy.m_signs[2];
        };
        
        void setOrigin(const glm::vec3& origin)
        {
            m_origin = origin;
        };
        
        glm::vec3 getOrigin(void) const
        {
            return m_origin;
        };
        
        void setDirection(const glm::vec3& direction)
        {
            m_direction = direction;
            m_invertedDirection = glm::vec3(1.0 / m_direction.x,
                                            1.0 / m_direction.y,
                                            1.0 / m_direction.z);
            m_signs[0] = (m_invertedDirection.x < 0);
            m_signs[1] = (m_invertedDirection.y < 0);
            m_signs[2] = (m_invertedDirection.z < 0);
        };
        
        glm::vec3 getDirection(void) const
        {
            return m_direction;
        };
        
        glm::vec3 getInvertedDirection(void) const
        {
            return m_invertedDirection;
        };
        
        i32 getSign(ui32 index) const
        {
            assert(index < 3);
            return m_signs[index];
        }
    };
    
    inline bool intersect(const glm::ray &ray,
                          const glm::vec3& minBound,
                          const glm::vec3& maxBound)
    {
        glm::vec3 parameters[2];
        parameters[0] = minBound;
        parameters[1] = maxBound;
        f32 tmin, tmax, tymin, tymax, tzmin, tzmax;
        
        tmin = (parameters[ray.getSign(0)].x - ray.getOrigin().x) * ray.getInvertedDirection().x;
        tmax = (parameters[1 - ray.getSign(0)].x - ray.getOrigin().x) * ray.getInvertedDirection().x;
        tymin = (parameters[ray.getSign(1)].y - ray.getOrigin().y) * ray.getInvertedDirection().y;
        tymax = (parameters[1 - ray.getSign(1)].y - ray.getOrigin().y) * ray.getInvertedDirection().y;
        if ( (tmin > tymax) || (tymin > tmax) )
        {
            return false;
        }
        if (tymin > tmin)
            tmin = tymin;
        if (tymax < tmax)
            tmax = tymax;
        tzmin = (parameters[ray.getSign(2)].z - ray.getOrigin().z) * ray.getInvertedDirection().z;
        tzmax = (parameters[1 - ray.getSign(2)].z - ray.getOrigin().z) * ray.getInvertedDirection().z;
        if ( (tmin > tzmax) || (tzmin > tmax) )
        {
            return false;
        }
        return true;
    };
};

#if defined(__IOS__)

static std::map<std::string, std::string> g_platforms = {
    {"i386", "simulator" },
    {"iPod1,1", "iPod Touch" },
    {"iPod2,1", "iPod Touch Second Generation" },
    {"iPod3,1", "iPod Touch Third Generation" },
    {"iPod4,1", "iPod Touch Fourth Generation" },
    {"iPhone1,1", "iPhone" },
    {"iPhone1,2", "iPhone 3G" },
    {"iPhone2,1", "iPhone 3GS" },
    {"iPad1,1",   "iPad" },
    {"iPad2,1",   "iPad 2" },
    {"iPad3,1",   "3rd Generation iPad" },
    {"iPhone3,1", "iPhone 4" },
    {"iPhone4,1", "iPhone 4S" },
    {"iPhone5,1", "iPhone 5 (model A1428, AT&T/Canada)" },
    {"iPhone5,2", "iPhone 5 (model A1429)" },
    {"iPad3,4", "4th Generation iPad" },
    {"iPad2,5", "iPad Mini" },
    {"iPhone5,3", "iPhone 5c (model A1456, A1532 | GSM)" },
    {"iPhone5,4", "iPhone 5c (model A1507, A1516, A1526 (China), A1529 | Global)" },
    {"iPhone6,1", "iPhone 5s (model A1433, A1533 | GSM)" },
    {"iPhone6,2", "iPhone 5s (model A1457, A1518, A1528 (China), A1530 | Global)" },
    {"iPad4,1", "5th Generation iPad (iPad Air) - Wifi" },
    {"iPad4,2", "5th Generation iPad (iPad Air) - Cellular" },
    {"iPad4,4", "2nd Generation iPad Mini - Wifi" },
    {"iPad4,5", "2nd Generation iPad Mini - Cellular" },
    {"iPhone7,2", "iPhone 6"},
    {"iPhone7,1", "iPhone 6+"}
};

static std::set<std::string> g_highPerformancePlatforms = {
    "iPhone5,3",
    "iPhone5,4",
    "iPhone6,1",
    "iPhone6,2",
    "iPad2,5",
    "iPad3,1",
    "iPad3,4",
    "iPad4,1",
    "iPad4,2",
    "iPad4,4",
    "iPad4,5",
    "iPad2,5",
    "iPhone7,1",
    "iPhone7,2"
};

static inline std::string getPlatform(void)
{
    static std::string platform = "";
    if(platform.length() == 0)
    {
        size_t size;
        sysctlbyname("hw.machine", NULL, &size, NULL, 0);
        char *machine = (char *)malloc(size);
        sysctlbyname("hw.machine", machine, &size, NULL, 0);
        platform = std::string(machine);
        free(machine);
    }
    return platform;
};

#endif

#define MIN_VALUE(a, b) (((a) < (b)) ? (a) : (b))
#define MAX_VALUE(a, b) (((a) > (b)) ? (a) : (b))
#define TO_RGB565(r, g, b) (unsigned short) (((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3))
#define TO_RGBA4444(r, g, b, a) (unsigned short) ((r >> 4) << 12 | (g >> 4) << 8 | (b >> 4) << 4 | (a >> 4))

#endif
