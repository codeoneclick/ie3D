// autogenerated: do not add any changes
#ifndef CConfigurationMaterial_h
#define CConfigurationMaterial_h
#include "IConfiguration.h"
#include "CConfigurationShader.h"
#include "CConfigurationTexture.h"
class CConfigurationMaterial : public IConfiguration
{
public:
CConfigurationMaterial(void) = default;
~CConfigurationMaterial(void) = default;
std::string getRenderTechniqueName(void) const;
#if defined(__EDITOR__)
void setRenderTechniqueName(std::string render_operation_name);
#endif
bool getDepthTest(void) const;
#if defined(__EDITOR__)
void setDepthTest(bool is_depth_test);
#endif
bool getDepthMask(void) const;
#if defined(__EDITOR__)
void setDepthMask(bool is_depth_mask);
#endif
bool getCulling(void) const;
#if defined(__EDITOR__)
void setCulling(bool is_cull_face);
#endif
GLenum getCullingMode(void) const;
#if defined(__EDITOR__)
void setCullingMode(GLenum cull_face_mode);
#endif
bool getBlending(void) const;
#if defined(__EDITOR__)
void setBlending(bool is_blending);
#endif
GLenum getBlendingFunctionSource(void) const;
#if defined(__EDITOR__)
void setBlendingFunctionSource(GLenum blending_function_source);
#endif
GLenum getBlendingFunctionDestination(void) const;
#if defined(__EDITOR__)
void setBlendingFunctionDestination(GLenum blending_function_destination);
#endif
bool getClipping(void) const;
#if defined(__EDITOR__)
void setClipping(bool is_cliping);
#endif
f32 getClippingX(void) const;
#if defined(__EDITOR__)
void setClippingX(f32 clipping_x);
#endif
f32 getClippingY(void) const;
#if defined(__EDITOR__)
void setClippingY(f32 clipping_y);
#endif
f32 getClippingZ(void) const;
#if defined(__EDITOR__)
void setClippingZ(f32 clipping_z);
#endif
f32 getClippingW(void) const;
#if defined(__EDITOR__)
void setClippingW(f32 clipping_w);
#endif
bool getReflecting(void) const;
#if defined(__EDITOR__)
void setReflecting(bool is_reflecting);
#endif
bool getShadowing(void) const;
#if defined(__EDITOR__)
void setShadowing(bool is_shadowing);
#endif
bool getDebugging(void) const;
#if defined(__EDITOR__)
void setDebugging(bool is_debugging);
#endif
std::shared_ptr<CConfigurationShader> getShaderConfiguration(void) const;
#if defined(__EDITOR__)
void setShaderConfiguration(const std::shared_ptr<CConfigurationShader>& shader);
#endif
std::vector<std::shared_ptr<IConfiguration>> getTexturesConfigurations(void) const;
#if defined(__EDITOR__)
void addTexturesConfigurations(const std::shared_ptr<CConfigurationTexture>& texture);
#endif
#if defined(__EDITOR__)
void setTexturesConfigurations(const std::shared_ptr<CConfigurationTexture>& texture, i32 index);
#endif
void serialize(const std::string& filename);
#if defined(__EDITOR__)
void deserialize(const std::string& filename);
#endif
};
#endif
