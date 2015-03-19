//
//  HGLCommands.h
//  ie3D-Core
//
//  Created by sergey.sergeev on 2/27/15.
//  Copyright (c) 2015 Sergey Sergeev. All rights reserved.
//

#ifndef HGLCommands_h
#define HGLCommands_h

#if defined(__OSX__)

#define __OPENGL_30__ 1

#else

#define __OPENGL_20__ 1

#endif

#if defined(__IOS__)

#include <QuartzCore/QuartzCore.h>
#include <sys/types.h>
#include <sys/sysctl.h>

#if defined(__OPENGL_20__)

#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>

#elif defined(__OPENGL_30__)

#include <OpenGLES/ES3/gl.h>
#include <OpenGLES/ES3/glext.h>

#endif

#elif defined(__WIN32__)

#include <windows.h>

#elif defined(__OSX__)

#include <OpenGL/OpenGL.h>

#if defined(__OPENGL_20__)

#include <OpenGL/gl.h>
#include <OpenGL/glext.h>

#elif defined(__OPENGL_30__)

#include <OpenGL/gl3.h>
#include <OpenGL/gl3ext.h>

#endif

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

inline bool ieGLError(void)
{
    GLenum error = glGetError();
    assert(error == GL_NO_ERROR);
    return error != GL_NO_ERROR;Ent
};

#define CASE_STRING_VALUE(VALUE) case VALUE: return #VALUE;

inline std::string ieGLErrorString(void)
{
    GLenum error = glGetError();
    switch (error)
    {
        CASE_STRING_VALUE(GL_NO_ERROR)
        CASE_STRING_VALUE(GL_INVALID_ENUM)
        CASE_STRING_VALUE(GL_INVALID_VALUE)
        CASE_STRING_VALUE(GL_INVALID_OPERATION)
        CASE_STRING_VALUE(GL_OUT_OF_MEMORY)
        CASE_STRING_VALUE(GL_INVALID_FRAMEBUFFER_OPERATION)
            
        default:
        {
            char buffer[32] = { };
            sprintf(buffer, "%u", error);
            return "UNKNOWN OpenGL ERROR" + std::string(buffer);
        }
    }
}

inline void ieEnable(GLenum cap)
{
    glEnable(cap);
    
#if defined(DEBUG)
    if(ieGLError())
    {
        std::cout<<"OpenGL error: "<<ieGLErrorString()<<std::endl;
    }
#endif
};

inline void ieDisable(GLenum cap)
{
    glDisable(cap);
    
#if defined(DEBUG)
    if(ieGLError())
    {
        std::cout<<"OpenGL error: "<<ieGLErrorString()<<std::endl;
    }
#endif
};

inline void ieDepthFunc(GLenum func)
{
    glDepthFunc(func);
    
#if defined(DEBUG)
    if(ieGLError())
    {
        std::cout<<"OpenGL error: "<<ieGLErrorString()<<std::endl;
    }
#endif
};

inline void ieDepthMask(GLboolean flag)
{
    glDepthMask(flag);
    
#if defined(DEBUG)
    if(ieGLError())
    {
        std::cout<<"OpenGL error: "<<ieGLErrorString()<<std::endl;
    }
#endif
};

inline void ieCullFace(GLenum mode)
{
    glCullFace(mode);
    
#if defined(DEBUG)
    if(ieGLError())
    {
        std::cout<<"OpenGL error: "<<ieGLErrorString()<<std::endl;
    }
#endif
};

inline void ieBlendFunc(GLenum sfactor, GLenum dfactor)
{
    glBlendFunc(sfactor, dfactor);
    
#if defined(DEBUG)
    if(ieGLError())
    {
        std::cout<<"OpenGL error: "<<ieGLErrorString()<<std::endl;
    }
#endif
};

inline void ieClear(GLbitfield mask)
{
    glClear(mask);
    
#if defined(DEBUG)
    if(ieGLError())
    {
        std::cout<<"OpenGL error: "<<ieGLErrorString()<<std::endl;
    }
#endif
};

inline void ieClearColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
    glClearColor(red, green, blue, alpha);
    
#if defined(DEBUG)
    if(ieGLError())
    {
        std::cout<<"OpenGL error: "<<ieGLErrorString()<<std::endl;
    }
#endif
};

inline void ieViewport(int x, int y, GLsizei width, GLsizei height)
{
    glViewport(x, y, width, height);
    
#if defined(DEBUG)
    if(ieGLError())
    {
        std::cout<<"OpenGL error: "<<ieGLErrorString()<<std::endl;
    }
#endif
};

inline void ieDeleteRenderbuffers(GLsizei n, const GLuint *renderbuffers)
{
    glDeleteRenderbuffers(n, renderbuffers);
    
#if defined(DEBUG)
    if(ieGLError())
    {
        std::cout<<"OpenGL error: "<<ieGLErrorString()<<std::endl;
    }
#endif
};

inline void ieGenRenderbuffers(GLsizei n, GLuint *renderbuffers)
{
    glGenRenderbuffers(n, renderbuffers);
    
#if defined(DEBUG)
    if(ieGLError())
    {
        std::cout<<"OpenGL error: "<<ieGLErrorString()<<std::endl;
    }
#endif
};

inline void ieRenderbufferStorage(GLenum target, GLenum internalformat, GLsizei width, GLsizei height)
{
    glRenderbufferStorage(target, internalformat, width, height);
    
#if defined(DEBUG)
    if(ieGLError())
    {
        std::cout<<"OpenGL error: "<<ieGLErrorString()<<std::endl;
    }
#endif
};

inline void ieBindFramebuffer(GLenum target, GLuint framebuffer)
{
    glBindFramebuffer(target, framebuffer);
    
#if defined(DEBUG)
    if(ieGLError())
    {
        std::cout<<"OpenGL error: "<<ieGLErrorString()<<std::endl;
    }
#endif
};

inline void ieBindRenderbuffer(GLenum target, GLuint renderbuffer)
{
    glBindRenderbuffer(target, renderbuffer);
    
#if defined(DEBUG)
    if(ieGLError())
    {
        std::cout<<"OpenGL error: "<<ieGLErrorString()<<std::endl;
    }
#endif
};

inline void ieDeleteFramebuffers(GLsizei n, const GLuint *framebuffers)
{
    glDeleteFramebuffers(n, framebuffers);
    
#if defined(DEBUG)
    if(ieGLError())
    {
        std::cout<<"OpenGL error: "<<ieGLErrorString()<<std::endl;
    }
#endif
};

inline void ieGenFramebuffers(GLsizei n, GLuint *framebuffers)
{
    glGenFramebuffers(n, framebuffers);
    
#if defined(DEBUG)
    if(ieGLError())
    {
        std::cout<<"OpenGL error: "<<ieGLErrorString()<<std::endl;
    }
#endif
};

inline void ieFramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level)
{
    glFramebufferTexture2D(target, attachment, textarget, texture, level);

#if defined(DEBUG)
    if(ieGLError())
    {
        std::cout<<"OpenGL error: "<<ieGLErrorString()<<std::endl;
    }
#endif
};

inline void ieFramebufferRenderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer)
{
    glFramebufferRenderbuffer(target, attachment, renderbuffertarget, renderbuffer);
    
#if defined(DEBUG)
    if(ieGLError())
    {
        std::cout<<"OpenGL error: "<<ieGLErrorString()<<std::endl;
    }
#endif
};

inline void ieGenTextures(GLsizei n, GLuint *textures)
{
    glGenTextures(n, textures);
    
#if defined(DEBUG)
    if(ieGLError())
    {
        std::cout<<"OpenGL error: "<<ieGLErrorString()<<std::endl;
    }
#endif
};

inline void ieDeleteTextures(GLsizei n, const GLuint *textures)
{
    glDeleteTextures(n, textures);
    
#if defined(DEBUG)
    if(ieGLError())
    {
        std::cout<<"OpenGL error: "<<ieGLErrorString()<<std::endl;
    }
#endif
};

inline void ieBindTexture(uint32_t target, uint32_t texture)
{
    glBindTexture(target, texture);
    
#if defined(DEBUG)
    if(ieGLError())
    {
        std::cout<<"OpenGL error: "<<ieGLErrorString()<<std::endl;
    }
#endif
};

inline void ieTexParameteri(GLenum target, GLenum pname, GLint param)
{
    glTexParameteri(target, pname, param);
    
#if defined(DEBUG)
    if(ieGLError())
    {
        std::cout<<"OpenGL error: "<<ieGLErrorString()<<std::endl;
    }
#endif
};

inline void ieTexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels)
{
    glTexImage2D(target, level, internalformat, width, height, border, format, type, pixels);
    
#if defined(DEBUG)
    if(ieGLError())
    {
        std::cout<<"OpenGL error: "<<ieGLErrorString()<<std::endl;
    }
#endif
};

inline void ieCompressedTexImage2D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid *data)
{
    glCompressedTexImage2D(target, level, internalformat, width, height, border, imageSize, data);
    
#if defined(DEBUG)
    if(ieGLError())
    {
        std::cout<<"OpenGL error: "<<ieGLErrorString()<<std::endl;
    }
#endif
}

inline void ieGenerateMipmap(GLenum target)
{
    glGenerateMipmap(target);
    
#if defined(DEBUG)
    if(ieGLError())
    {
        std::cout<<"OpenGL error: "<<ieGLErrorString()<<std::endl;
    }
#endif
};

inline void ieGenBuffers(GLsizei n, GLuint *buffers)
{
    glGenBuffers(n, buffers);
    
#if defined(DEBUG)
    if(ieGLError())
    {
        std::cout<<"OpenGL error: "<<ieGLErrorString()<<std::endl;
    }
#endif
};

inline void ieDeleteBuffers(GLsizei n, const GLuint *buffers)
{
    glDeleteBuffers(n, buffers);
    
#if defined(DEBUG)
    if(ieGLError())
    {
        std::cout<<"OpenGL error: "<<ieGLErrorString()<<std::endl;
    }
#endif
};

inline void ieBindBuffer(uint32_t target, uint32_t buffer)
{
    glBindBuffer(target, buffer);
    
#if defined(DEBUG)
    if(ieGLError())
    {
        std::cout<<"OpenGL error: "<<ieGLErrorString()<<std::endl;
    }
#endif
};

inline void ieBufferData(GLenum target, GLsizeiptr size, const GLvoid *data, GLenum usage)
{
    glBufferData(target, size, data, usage);
    
#if defined(DEBUG)
    if(ieGLError())
    {
        std::cout<<"OpenGL error: "<<ieGLErrorString()<<std::endl;
    }
#endif
};

inline void ieBufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid *data)
{
    glBufferSubData(target, offset, size, data);
    
#if defined(DEBUG)
    if(ieGLError())
    {
        std::cout<<"OpenGL error: "<<ieGLErrorString()<<std::endl;
    }
#endif
};

inline void ieEnableVertexAttribArray(GLuint index)
{
    glEnableVertexAttribArray(index);
    
#if defined(DEBUG)
    if(ieGLError())
    {
        std::cout<<"OpenGL error: "<<ieGLErrorString()<<std::endl;
    }
#endif
};

inline void ieDisableVertexAttribArray(GLuint index)
{
    glDisableVertexAttribArray(index);
    
#if defined(DEBUG)
    if(ieGLError())
    {
        std::cout<<"OpenGL error: "<<ieGLErrorString()<<std::endl;
    }
#endif
};

inline void ieVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer)
{
    glVertexAttribPointer(index, size, type, normalized, stride, pointer);
    
#if defined(DEBUG)
    if(ieGLError())
    {
        std::cout<<"OpenGL error: "<<ieGLErrorString()<<std::endl;
    }
#endif
};

inline void ieBindVertexArray(GLuint array)
{
#if defined(__OSX__)
    glBindVertexArray(array);
#else
    glBindVertexArrayOES(array);
#endif
    
#if defined(DEBUG)
    if(ieGLError())
    {
        std::cout<<"OpenGL error: "<<ieGLErrorString()<<std::endl;
    }
#endif
};

inline void ieDeleteVertexArrays(GLsizei n, const GLuint *arrays)
{
#if defined(__OSX__)
    glDeleteVertexArrays(n, arrays);
#else
    glDeleteVertexArraysOES(n, arrays);
#endif
    
#if defined(DEBUG)
    if(ieGLError())
    {
        std::cout<<"OpenGL error: "<<ieGLErrorString()<<std::endl;
    }
#endif
};

inline void ieGenVertexArrays(GLsizei n, GLuint *arrays)
{
#if defined(__OSX__)
    glGenVertexArrays(n, arrays);
#else
    glGenVertexArraysOES(n, arrays);
#endif
    
#if defined(DEBUG)
    if(ieGLError())
    {
        std::cout<<"OpenGL error: "<<ieGLErrorString()<<std::endl;
    }
#endif
};

inline void ieDrawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices)
{
    glDrawElements(mode, count, type, indices);
    
#if defined(DEBUG)
    if(ieGLError())
    {
        std::cout<<"OpenGL error: "<<ieGLErrorString()<<std::endl;
    }
#endif
};

inline GLint ieGetUniformLocation(GLuint program, const GLchar *name)
{
    return glGetUniformLocation(program, name);
};

inline GLint ieGetAttribLocation(GLuint program, const GLchar *name)
{
    return glGetAttribLocation(program, name);
};

inline void ieUniformMatrix3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
    glUniformMatrix3fv(location, count, transpose, value);
    
#if defined(DEBUG)
    if(ieGLError())
    {
        std::cout<<"OpenGL error: "<<ieGLErrorString()<<std::endl;
    }
#endif
};

inline void ieUniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
    glUniformMatrix4fv(location, count, transpose, value);
    
#if defined(DEBUG)
    if(ieGLError())
    {
        std::cout<<"OpenGL error: "<<ieGLErrorString()<<std::endl;
    }
#endif
};

inline void ieUniform2fv(GLint location, GLsizei count, const GLfloat *value)
{
    glUniform2fv(location, count, value);
    
#if defined(DEBUG)
    if(ieGLError())
    {
        std::cout<<"OpenGL error: "<<ieGLErrorString()<<std::endl;
    }
#endif
};

inline void ieUniform3fv(GLint location, GLsizei count, const GLfloat *value)
{
    glUniform3fv(location, count, value);
    
#if defined(DEBUG)
    if(ieGLError())
    {
        std::cout<<"OpenGL error: "<<ieGLErrorString()<<std::endl;
    }
#endif
};

inline void ieUniform4fv(GLint location, GLsizei count, const GLfloat *value)
{
    glUniform4fv(location, count, value);
    
#if defined(DEBUG)
    if(ieGLError())
    {
        std::cout<<"OpenGL error: "<<ieGLErrorString()<<std::endl;
    }
#endif
};

inline void ieUniform1f(GLint location, GLfloat v0)
{
    glUniform1f(location, v0);
    
#if defined(DEBUG)
    if(ieGLError())
    {
        std::cout<<"OpenGL error: "<<ieGLErrorString()<<std::endl;
    }
#endif
};

inline void ieUniform1i(GLint location, GLint v0)
{
    glUniform1i(location, v0);
    
#if defined(DEBUG)
    if(ieGLError())
    {
        std::cout<<"OpenGL error: "<<ieGLErrorString()<<std::endl;
    }
#endif
};

inline void ieActiveTexture(GLenum texture)
{
    glActiveTexture(texture);
    
#if defined(DEBUG)
    if(ieGLError())
    {
        std::cout<<"OpenGL error: "<<ieGLErrorString()<<std::endl;
    }
#endif
};

inline void ieUseProgram(GLuint program)
{
    glUseProgram(program);
    
#if defined(DEBUG)
    if(ieGLError())
    {
        std::cout<<"OpenGL error: "<<ieGLErrorString()<<std::endl;
    }
#endif
};

#endif
