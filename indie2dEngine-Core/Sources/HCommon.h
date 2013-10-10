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

#ifdef __APPLE__

#define USE_CURL 1
#define USE_JSONCPP 1
#define USE_GCDPP 1

#else

#define M_PI 3.14f
#define M_PI_2 1.57f

#endif

#include <glm/glm.hpp>
#include <glm/gtc/type_precision.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <pugixml/pugixml.hpp>

#ifdef USE_GCDPP
#include "gcdppLib.h"
#endif

#ifdef USE_CURL
#include <curl/curl.h>
#endif

#ifdef USE_JSONCPP
#include <json/json.h>
#endif

#ifdef __APPLE__

#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#include <QuartzCore/QuartzCore.h>

#else

#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

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

#endif
