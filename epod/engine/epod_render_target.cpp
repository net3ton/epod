#include "epod.h"
#include "epod_render_target.h"
#include "epod_graphics.h"
#include "epod_texture.h"

#include <gles/gl.h>
#include <gles/glext.h>

#ifndef __APPLE__
#include <egl/egl.h>
#ifdef __cplusplus
extern "C"
{
#endif
  PFNGLFRAMEBUFFERTEXTURE2DOESPROC glFramebufferTexture2DOES =
    (PFNGLFRAMEBUFFERTEXTURE2DOESPROC) eglGetProcAddress("glFramebufferTexture2DOES");
  PFNGLBINDFRAMEBUFFEROESPROC glBindFramebufferOES =
    (PFNGLBINDFRAMEBUFFEROESPROC) eglGetProcAddress("glBindFramebufferOES");
  PFNGLGENFRAMEBUFFERSOESPROC glGenFramebuffersOES =
    (PFNGLGENFRAMEBUFFERSOESPROC) eglGetProcAddress("glGenFramebuffersOES");
  PFNGLDELETEFRAMEBUFFERSOESPROC glDeleteFramebuffersOES =
    (PFNGLDELETEFRAMEBUFFERSOESPROC) eglGetProcAddress("glDeleteFramebuffersOES");
#ifdef __cplusplus
}
#endif
#endif


namespace ep
{

RenderTarget::RenderTarget(int width, int height, bool with_alpha)
  :m_graphics(new Graphics())
  ,m_texture(new Texture(width, height, with_alpha))
{
  glGenFramebuffersOES(1, (GLuint*)&m_id);
  glBindFramebufferOES(GL_FRAMEBUFFER_OES, m_id);
  glFramebufferTexture2DOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, GL_TEXTURE_2D, m_texture->getId(), 0);

  glBindFramebufferOES(GL_FRAMEBUFFER_OES, 0);
}


RenderTarget::~RenderTarget()
{
  glDeleteFramebuffersOES(1, (GLuint*)&m_id);
  m_texture->free();
}


IGraphics *RenderTarget::beginRender()
{
  glBindFramebufferOES(GL_FRAMEBUFFER_OES, m_id);

  int width = m_texture->getWidth();
  int height = m_texture->getHeight();

  m_graphics->beginRender(width, height, VIEW_ORIENT_NORMAL);
  return m_graphics.get();
}


void RenderTarget::endRender()
{
  m_graphics->endRender();
  glBindFramebufferOES(GL_FRAMEBUFFER_OES, 0);
}

} // namespace ep
