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
#include <unordered_map>
#include <unordered_set>

#if defined(__IOS__) || defined(__OSX__)

#undef  __USE_CURL__
#undef  __USE_JSONCPP__
#define __USE_GCDPP__
#define __USE_OPENGLES__

#elif defined(__WIN32__)

#define M_PI 3.14f
#define M_PI_2 1.57f
#define __USE_OPENGLES__

#endif

#include <glm/glm.hpp>
#include <glm/gtc/type_precision.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <pugixml/pugixml.hpp>

#if defined(__USE_GCDPP__)
#include "gcdppLib.h"
#endif

#if defined(__USE_CURL__)
#include <curl/curl.h>
#endif

#if defined(__USE_JSONCPP__)
#include <json/json.h>
#endif

#if defined(__IOS__)

#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#include <QuartzCore/QuartzCore.h>

#elif defined(__WIN32__)

#include <windows.h>
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#elif defined(__OSX__)

#include <OpenGL/OpenGL.h>
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#elif defined(__NDK__)

#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include <jni.h>
#include <android/window.h>
#include <android/sensor.h>
#include <android/log.h>
#include <android_native_app_glue.h>
#include <android/asset_manager.h>

#define NDK_LOG(...) ((void)__android_log_print(ANDROID_LOG_WARN, "native-activity", __VA_ARGS__))

#endif

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
        
        memstream(char* _data, size_t _size):
        std::istream(&m_buffer),
        m_buffer(_data, _size),
        m_data(_data),
        m_size(_size)
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

            membuf(char* _data, size_t _size)
            {
                setp(_data, _data + _size);
                setg(_data, _data, _data + _size);
            };
        };
        membuf m_buffer;
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
    
    inline glm::vec3 transform(const glm::vec3 &_vertex, const glm::mat4x4 &_matrix)
    {
        glm::vec4 value = _matrix * glm::vec4(_vertex, 1.0f);
        return glm::vec3(value.x, value.y, value.z);
    }
};


#define MAX_VALUE(a,b) (((a) > (b)) ? (a) : (b))
#define TO_RGB(r,g,b) (unsigned short)(((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3))

#endif
