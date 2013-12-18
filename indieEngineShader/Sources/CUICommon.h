//
//  CUICommon.h
//  indieEngineShader
//
//  Created by Sergey Sergeev on 12/18/13.
//
//

#ifndef CUICommon_h
#define CUICommon_h

#include <QMainWindow>
#include "HCommon.h"

static const QString kBtnDisableStyleSheet = "background-color: rgb(255, 0, 0); color: rgb(255, 255, 255); text-align:left";
static const QString kBtnEnableStyleSheet = "background-color: rgb(128, 128, 128); color: rgb(255, 255, 255); text-align:left";
static const QString kBtnFocusStyleSheet = "background-color: rgb(0, 96, 0); color: rgb(255, 255, 255); text-align:left";

static const QString kTextGLCullModeFront = "GL_FRONT";
static const QString kTextGLCullModeBack = "GL_BACK";
static const QString kTextGLBlendModeSrcAlpha = "GL_SRC_ALPHA";
static const QString kTextGLBlendModeOneMinusSrcAlpha = "GL_ONE_MINUS_SRC_ALPHA";
static const QString kTextGLTextureWrapRepeat = "GL_REPEAT";
static const QString kTextGLTextureWrapMirroredRepeat = "GL_MIRRORED_REPEAT";
static const QString kTextGLTextureWrapClampEdge = "GL_CLAMP_TO_EDGE";

#endif