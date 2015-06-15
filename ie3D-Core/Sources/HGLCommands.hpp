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

//#define GL_ERROR_ASSERT 1

inline bool ieGLError(void)
{
    GLenum error = glGetError();
#if defined(GL_ERROR_ASSERT)
    assert(error == GL_NO_ERROR);
#endif
    return error != GL_NO_ERROR;
};

#define CASE_STRING_VALUE(GL_ERROR, STRING_ERROR) case GL_ERROR: STRING_ERROR = #GL_ERROR; break;

inline std::string ieGLErrorString(void)
{
    GLenum error = glGetError();
    std::string stringError = "";
    switch (error)
    {
        CASE_STRING_VALUE(GL_NO_ERROR, stringError)
        CASE_STRING_VALUE(GL_INVALID_ENUM, stringError)
        CASE_STRING_VALUE(GL_INVALID_VALUE, stringError)
        CASE_STRING_VALUE(GL_INVALID_OPERATION, stringError)
        CASE_STRING_VALUE(GL_OUT_OF_MEMORY, stringError)
        CASE_STRING_VALUE(GL_INVALID_FRAMEBUFFER_OPERATION, stringError)
            
        default:
        {
            char buffer[32] = { };
            sprintf(buffer, "%u", error);
            return "UNKNOWN OpenGL ERROR" + std::string(buffer);
        }
    }
    if(stringError != "GL_NO_ERROR")
    {
        std::cout<<"OpenGL error: "<<ieGLErrorString()<<std::endl;
    }
    return stringError;
}

inline void ieEnable(GLenum cap)
{
    glEnable(cap);
    
#if defined(DEBUG)
    ieGLError();
    ieGLErrorString();
#endif
};

inline void ieDisable(GLenum cap)
{
    glDisable(cap);
    
#if defined(DEBUG)
    ieGLError();
    ieGLErrorString();
#endif
};

inline void ieDepthFunc(GLenum func)
{
    glDepthFunc(func);
    
#if defined(DEBUG)
    ieGLError();
    ieGLErrorString();
#endif
};

inline void ieDepthMask(GLboolean flag)
{
    glDepthMask(flag);
    
#if defined(DEBUG)
    ieGLError();
    ieGLErrorString();
#endif
};

inline void ieCullFace(GLenum mode)
{
    glCullFace(mode);
    
#if defined(DEBUG)
    ieGLError();
    ieGLErrorString();
#endif
};

inline void ieBlendFunc(GLenum sfactor, GLenum dfactor)
{
    glBlendFunc(sfactor, dfactor);
    
#if defined(DEBUG)
    ieGLError();
    ieGLErrorString();
#endif
};

inline void ieClear(GLbitfield mask)
{
    glClear(mask);
    
#if defined(DEBUG)
    ieGLError();
    ieGLErrorString();
#endif
};

inline void ieClearColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
    glClearColor(red, green, blue, alpha);
    
#if defined(DEBUG)
    ieGLError();
    ieGLErrorString();
#endif
};

inline void ieViewport(int x, int y, GLsizei width, GLsizei height)
{
    glViewport(x, y, width, height);
    
#if defined(DEBUG)
    ieGLError();
    ieGLErrorString();
#endif
};

inline void ieDeleteRenderbuffers(GLsizei n, const GLuint *renderbuffers)
{
    glDeleteRenderbuffers(n, renderbuffers);
    
#if defined(DEBUG)
    ieGLError();
    ieGLErrorString();
#endif
};

inline void ieGenRenderbuffers(GLsizei n, GLuint *renderbuffers)
{
    glGenRenderbuffers(n, renderbuffers);
    
#if defined(DEBUG)
    ieGLError();
    ieGLErrorString();
#endif
};

inline void ieRenderbufferStorage(GLenum target, GLenum internalformat, GLsizei width, GLsizei height)
{
    glRenderbufferStorage(target, internalformat, width, height);
    
#if defined(DEBUG)
    ieGLError();
    ieGLErrorString();
#endif
};

inline void ieBindFramebuffer(GLenum target, GLuint framebuffer)
{
    glBindFramebuffer(target, framebuffer);
    
#if defined(DEBUG)
    ieGLError();
    ieGLErrorString();
#endif
};

inline void ieBindRenderbuffer(GLenum target, GLuint renderbuffer)
{
    glBindRenderbuffer(target, renderbuffer);
    
#if defined(DEBUG)
    ieGLError();
    ieGLErrorString();
#endif
};

inline void ieDeleteFramebuffers(GLsizei n, const GLuint *framebuffers)
{
    glDeleteFramebuffers(n, framebuffers);
    
#if defined(DEBUG)
    ieGLError();
    ieGLErrorString();
#endif
};

inline void ieGenFramebuffers(GLsizei n, GLuint *framebuffers)
{
    glGenFramebuffers(n, framebuffers);
    
#if defined(DEBUG)
    ieGLError();
    ieGLErrorString();
#endif
};

inline void ieFramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level)
{
    glFramebufferTexture2D(target, attachment, textarget, texture, level);

#if defined(DEBUG)
    ieGLError();
    ieGLErrorString();
#endif
};

inline void ieFramebufferRenderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer)
{
    glFramebufferRenderbuffer(target, attachment, renderbuffertarget, renderbuffer);
    
#if defined(DEBUG)
    ieGLError();
    ieGLErrorString();
#endif
};

inline void ieGenTextures(GLsizei n, GLuint *textures)
{
    glGenTextures(n, textures);
    
#if defined(DEBUG)
    ieGLError();
    ieGLErrorString();
#endif
};

inline void ieDeleteTextures(GLsizei n, const GLuint *textures)
{
    glDeleteTextures(n, textures);
    
#if defined(DEBUG)
    ieGLError();
    ieGLErrorString();
#endif
};

inline void ieBindTexture(uint32_t target, uint32_t texture)
{
    glBindTexture(target, texture);
    
#if defined(DEBUG)
    ieGLError();
    ieGLErrorString();
#endif
};

inline void ieTexParameteri(GLenum target, GLenum pname, GLint param)
{
    glTexParameteri(target, pname, param);
    
#if defined(DEBUG)
    ieGLError();
    ieGLErrorString();
#endif
};

inline void ieTexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels)
{
    glTexImage2D(target, level, internalformat, width, height, border, format, type, pixels);
    
#if defined(DEBUG)
    ieGLError();
    ieGLErrorString();
#endif
};

inline void ieCompressedTexImage2D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid *data)
{
    glCompressedTexImage2D(target, level, internalformat, width, height, border, imageSize, data);
    
#if defined(DEBUG)
    ieGLError();
    ieGLErrorString();
#endif
}

inline void ieGenerateMipmap(GLenum target)
{
    glGenerateMipmap(target);
    
#if defined(DEBUG)
    ieGLError();
    ieGLErrorString();
#endif
};

inline void ieGenBuffers(GLsizei n, GLuint *buffers)
{
    glGenBuffers(n, buffers);
    
#if defined(DEBUG)
    ieGLError();
    ieGLErrorString();
#endif
};

inline void ieDeleteBuffers(GLsizei n, const GLuint *buffers)
{
    glDeleteBuffers(n, buffers);
    
#if defined(DEBUG)
    ieGLError();
    ieGLErrorString();
#endif
};

inline void ieBindBuffer(uint32_t target, uint32_t buffer)
{
    glBindBuffer(target, buffer);
    
#if defined(DEBUG)
    ieGLError();
    ieGLErrorString();
#endif
};

inline void ieBufferData(GLenum target, GLsizeiptr size, const GLvoid *data, GLenum usage)
{
    glBufferData(target, size, data, usage);
    
#if defined(DEBUG)
    ieGLError();
    ieGLErrorString();
#endif
};

inline void ieBufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid *data)
{
    glBufferSubData(target, offset, size, data);
    
#if defined(DEBUG)
    ieGLError();
    ieGLErrorString();
#endif
};

inline void ieEnableVertexAttribArray(GLuint index)
{
    glEnableVertexAttribArray(index);
    
#if defined(DEBUG)
    ieGLError();
    ieGLErrorString();
#endif
};

inline void ieDisableVertexAttribArray(GLuint index)
{
    glDisableVertexAttribArray(index);
    
#if defined(DEBUG)
    ieGLError();
    ieGLErrorString();
#endif
};

inline void ieVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer)
{
    glVertexAttribPointer(index, size, type, normalized, stride, pointer);
    
#if defined(DEBUG)
    ieGLError();
    ieGLErrorString();
#endif
};

inline void ieBindVertexArray(GLuint array)
{
#if defined(__OSX__)
    glBindVertexArray(array);
#elif defined(__IOS__) && defined(__OPENGL_30__)
    glBindVertexArray(array);
#else
    glBindVertexArrayOES(array);
#endif
    
#if defined(DEBUG)
    ieGLError();
    ieGLErrorString();
#endif
};

inline void ieDeleteVertexArrays(GLsizei n, const GLuint *arrays)
{
#if defined(__OSX__)
    glDeleteVertexArrays(n, arrays);
#elif defined(__IOS__) && defined(__OPENGL_30__)
    glDeleteVertexArrays(n, arrays);
#else
    glDeleteVertexArraysOES(n, arrays);
#endif
    
#if defined(DEBUG)
    ieGLError();
    ieGLErrorString();
#endif
};

inline void ieGenVertexArrays(GLsizei n, GLuint *arrays)
{
#if defined(__OSX__)
    glGenVertexArrays(n, arrays);
#elif defined(__IOS__) && defined(__OPENGL_30__)
     glGenVertexArrays(n, arrays);
#else
    glGenVertexArraysOES(n, arrays);
#endif
    
#if defined(DEBUG)
    ieGLError();
    ieGLErrorString();
#endif
};

inline void ieDrawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices)
{
    glDrawElements(mode, count, type, indices);
    
#if defined(DEBUG)
    ieGLError();
    ieGLErrorString();
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
    ieGLError();
    ieGLErrorString();
#endif
};

inline void ieUniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
    glUniformMatrix4fv(location, count, transpose, value);
    
#if defined(DEBUG)
    ieGLError();
    ieGLErrorString();
#endif
};

inline void ieUniform2fv(GLint location, GLsizei count, const GLfloat *value)
{
    glUniform2fv(location, count, value);
    
#if defined(DEBUG)
    ieGLError();
    ieGLErrorString();
#endif
};

inline void ieUniform3fv(GLint location, GLsizei count, const GLfloat *value)
{
    glUniform3fv(location, count, value);
    
#if defined(DEBUG)
    ieGLError();
    ieGLErrorString();
#endif
};

inline void ieUniform4fv(GLint location, GLsizei count, const GLfloat *value)
{
    glUniform4fv(location, count, value);
    
#if defined(DEBUG)
    ieGLError();
    ieGLErrorString();
#endif
};

inline void ieUniform1f(GLint location, GLfloat v0)
{
    glUniform1f(location, v0);
    
#if defined(DEBUG)
    ieGLError();
    ieGLErrorString();
#endif
};

inline void ieUniform1i(GLint location, GLint v0)
{
    glUniform1i(location, v0);
    
#if defined(DEBUG)
    ieGLError();
    ieGLErrorString();
#endif
};

inline void ieActiveTexture(GLenum texture)
{
    glActiveTexture(texture);
    
#if defined(DEBUG)
    ieGLError();
    ieGLErrorString();
#endif
};

inline void ieUseProgram(GLuint program)
{
    glUseProgram(program);
    
#if defined(DEBUG)
    ieGLError();
    ieGLErrorString();
#endif
};

#endif
