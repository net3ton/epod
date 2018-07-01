#ifndef _EPOD_RENDER_TARGET_H_
#define _EPOD_RENDER_TARGET_H_

#include "epod_interface.h"
#include <auto_ptr.h>


namespace ep
{

class RenderTarget
{
public:
  RenderTarget(int width, int height, bool with_alpha = false);
  ~RenderTarget();

  IGraphics *beginRender();
  void endRender();

  ITexture *getTexture() const { return m_texture.get(); }

private:
  int m_id;
  TAutoPtr<IGraphics> m_graphics;
  TAutoPtr<ITexture> m_texture;
};

} // namespace ep

#endif //_EPOD_RENDER_TARGET_H_
