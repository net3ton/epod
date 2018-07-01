#include "pvr.h"
#include "epod_platform.h"


EPVRTError PVRTTextureLoadFromPointer(
    const void *       pointer,
    const void *       psTextureHeader,
    const unsigned int nLoadFromLevel,
    const void * const texPtr
  )
{
  PVR_Texture_Header *psPVRHeader;
  unsigned long *pData;

  psPVRHeader = (PVR_Texture_Header*)pointer;

  pData = (unsigned long*) pointer;
  unsigned int u32NumSurfs;

  // perform checks for old PVR psPVRHeader
  if (psPVRHeader->dwHeaderSize != sizeof(PVR_Texture_Header))
  { 
    // Header V1
    if (psPVRHeader->dwHeaderSize == PVRTEX_V1_HEADER_SIZE)
    { 
      // react to old psPVRHeader: i.e. fill in numsurfs as this is missing from old header
      ep::log("warning: PVRTTextureLoadPartialFromPointer: this is an old pvr"
              " - you can use PVRTexTool to update its header.");
      if(psPVRHeader->dwpfFlags&PVRTEX_CUBEMAP)
        u32NumSurfs = 6;
      else
        u32NumSurfs = 1;
    }
    else
    { 
      // not a pvr at all
      ep::log("error: PVRTTextureLoadPartialFromPointer failed: not a valid pvr.");
      return PVR_FAIL;
    }
  }
  else
  {
    // Header V2
    if (psPVRHeader->dwNumSurfs < 1)
    {
      // encoded with old version of PVRTexTool before zero numsurfs bug found.
      if(psPVRHeader->dwpfFlags & PVRTEX_CUBEMAP)
        u32NumSurfs = 6;
      else
        u32NumSurfs = 1;
    }
    else
    {
      u32NumSurfs = psPVRHeader->dwNumSurfs;
    }
  }

  GLenum textureFormat = 0;
  GLenum textureType = GL_RGB;

  //bool IsPVRTCSupported     = g5::IsGLExtensionSupported("GL_IMG_texture_compression_pvrtc");
  //bool IsBGRA8888Supported  = g5::IsGLExtensionSupported("GL_IMG_texture_format_BGRA8888");
  bool IsPVRTCSupported = true;
  bool IsBGRA8888Supported = true;

  bool IsCompressedFormatSupported = false, IsCompressedFormat = false;
  // Only accept untwiddled data UNLESS texture format is PVRTC
  if ( ((psPVRHeader->dwpfFlags & PVRTEX_TWIDDLE) == PVRTEX_TWIDDLE)
    && ((psPVRHeader->dwpfFlags & PVRTEX_PIXELTYPE)!=OGL_PVRTC2)
    && ((psPVRHeader->dwpfFlags & PVRTEX_PIXELTYPE)!=OGL_PVRTC4) )
  {
    // We need to load untwiddled textures -- hw will twiddle for us.
    ep::log("error: PVRTTextureLoadPartialFromPointer failed: texture should be untwiddled.");
    return PVR_FAIL;
  }

  switch(psPVRHeader->dwpfFlags & PVRTEX_PIXELTYPE)
  {
  case OGL_RGBA_4444:
    textureFormat = GL_UNSIGNED_SHORT_4_4_4_4;
    textureType = GL_RGBA;
    break;

  case OGL_RGBA_5551:
    textureFormat = GL_UNSIGNED_SHORT_5_5_5_1;
    textureType = GL_RGBA;
    break;

  case OGL_RGBA_8888:
    textureFormat = GL_UNSIGNED_BYTE;
    textureType = GL_RGBA;
    break;

  // New OGL Specific Formats Added

  case OGL_RGB_565:
    textureFormat = GL_UNSIGNED_SHORT_5_6_5;
    textureType = GL_RGB;
    break;

  case OGL_RGB_555:
    ep::log("error: PVRTTextureLoadPartialFromPointer failed: pixel type OGL_RGB_555 not supported.");
    return PVR_FAIL; // Deal with exceptional case

  case OGL_RGB_888:
    textureFormat = GL_UNSIGNED_BYTE;
    textureType = GL_RGB;
    break;

  case OGL_I_8:
    textureFormat = GL_UNSIGNED_BYTE;
    textureType = GL_LUMINANCE;
    break;

  case OGL_AI_88:
    textureFormat = GL_UNSIGNED_BYTE;
    textureType = GL_LUMINANCE_ALPHA;
    break;

  case OGL_PVRTC2:
    if(IsPVRTCSupported)
    {
      IsCompressedFormatSupported = IsCompressedFormat = true;
      textureFormat = psPVRHeader->dwAlphaBitMask==0 ? GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG : GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG ; // PVRTC2
    }
    else
    {
      ep::log("error: PVRTTextureLoadPartialFromPointer error: PVRTC2 not supported.");
      return PVR_FAIL;
    }
    break;
  case OGL_PVRTC4:
    if(IsPVRTCSupported)
    {
      IsCompressedFormatSupported = IsCompressedFormat = true;
      textureFormat = psPVRHeader->dwAlphaBitMask==0 ? GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG : GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG ; // PVRTC4
    }
    else
    {
      ep::log("error: PVRTTextureLoadPartialFromPointer error: PVRTC4 not supported.");
      return PVR_FAIL;
    }
    break;
  case OGL_BGRA_8888:
    if(IsBGRA8888Supported)
    {
      textureFormat = GL_UNSIGNED_BYTE;
      textureType   = GL_BGRA;
      break;
    }
    else
    {
      ep::log("error: PVRTTextureLoadPartialFromPointer failed: Unable to load GL_BGRA texture as extension GL_IMG_texture_format_BGRA8888 is unsupported.");
      return PVR_FAIL;
    }
  default: // NOT SUPPORTED
    ep::log("error: PVRTTextureLoadPartialFromPointer failed: pixel type not supported.");
    return PVR_FAIL;
  }

  // load the texture up
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Never have row-aligned in psPVRHeaders

  // check that this data is cube map data or not.
  if(psPVRHeader->dwpfFlags & PVRTEX_CUBEMAP)
  {
    // not in OGLES you don't
    ep::log("error: PVRTTextureLoadPartialFromPointer failed: cube map textures are not available in OGLES1.x.");
    return PVR_FAIL;
  }

  for (unsigned int i=0; i<u32NumSurfs; i++)
  {
    char *theTexturePtr = (texPtr? (char*)texPtr :  (char*)pData + psPVRHeader->dwHeaderSize) + psPVRHeader->dwTextureDataSize * i;
    char *theTextureToLoad = 0;
    int nMIPMapLevel;
    int nTextureLevelsNeeded = (psPVRHeader->dwpfFlags & PVRTEX_MIPMAP)? psPVRHeader->dwMipMapCount : 0;
    unsigned int nSizeX = psPVRHeader->dwWidth, nSizeY = psPVRHeader->dwHeight;
    unsigned int CompressedImageSize = 0;

    for (nMIPMapLevel = 0; nMIPMapLevel <= nTextureLevelsNeeded; nSizeX = PVRT_MAX(nSizeX/2, (unsigned int)1), nSizeY = PVRT_MAX(nSizeY/2, (unsigned int)1), nMIPMapLevel++)
    {
      // Do Alpha-swap if needed
      theTextureToLoad = theTexturePtr;

      // Load the Texture
      // If the texture is PVRTC then use GLCompressedTexImage2D
      if (IsCompressedFormat)
      {
        // Calculate how many bytes this MIP level occupies
        if ((psPVRHeader->dwpfFlags & PVRTEX_PIXELTYPE) == OGL_PVRTC2)
        {
          CompressedImageSize = ( PVRT_MAX(nSizeX, PVRTC2_MIN_TEXWIDTH) * PVRT_MAX(nSizeY, PVRTC2_MIN_TEXHEIGHT) * psPVRHeader->dwBitCount + 7) / 8;
        }
        else
        {
          // PVRTC4 case
          CompressedImageSize = ( PVRT_MAX(nSizeX, PVRTC4_MIN_TEXWIDTH) * PVRT_MAX(nSizeY, PVRTC4_MIN_TEXHEIGHT) * psPVRHeader->dwBitCount + 7) / 8;
        }

        if(((signed int)nMIPMapLevel - (signed int)nLoadFromLevel) >= 0)
        {
          // Load compressed texture data at selected MIP level
          glCompressedTexImage2D(GL_TEXTURE_2D, nMIPMapLevel-nLoadFromLevel, textureFormat, nSizeX, nSizeY, 0,
            CompressedImageSize, theTextureToLoad);
        }
      }
      else
      {
        if(((signed int)nMIPMapLevel - (signed int)nLoadFromLevel) >= 0)
        {
          // Load uncompressed texture data at selected MIP level
          glTexImage2D(GL_TEXTURE_2D, nMIPMapLevel-nLoadFromLevel, textureType, nSizeX, nSizeY, 0,
            textureType, textureFormat, theTextureToLoad);
        }
      }

      if (glGetError())
      {
        ep::log("error: PVRTTextureLoadPartialFromPointer failed: glBindTexture() failed.");
        return PVR_FAIL;
      }

      // offset the texture pointer by one mip-map level
      // PVRTC case
      if (IsCompressedFormat)
      {
        theTexturePtr += CompressedImageSize;
      }
      else
      {
        // New formula that takes into account bit counts inferior to 8 (e.g. 1 bpp)
        theTexturePtr += (nSizeX * nSizeY * psPVRHeader->dwBitCount + 7) / 8;
      }
    }
  }

  if (psTextureHeader)
  {
    *(PVR_Texture_Header*)psTextureHeader = *psPVRHeader;
    ((PVR_Texture_Header*)psTextureHeader)->dwPVR = PVRTEX_IDENTIFIER;
    ((PVR_Texture_Header*)psTextureHeader)->dwNumSurfs = u32NumSurfs;
  }

  if (!psPVRHeader->dwMipMapCount)
  {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  }
  else
  {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  }

  return PVR_SUCCESS;
}


unsigned int PVRTTextureFormatGetBPP(const GLuint nFormat, const GLuint nType)
{
  switch(nFormat)
  {
    case GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG:
    case GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG:
      return 2;

    case GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG:
    case GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG:
      return 4;

    case GL_UNSIGNED_BYTE:
      switch(nType)
      {
        case GL_RGBA:
        case GL_BGRA:
          return 32;
      }

    case GL_UNSIGNED_SHORT_5_5_5_1:
      switch(nType)
      {
        case GL_RGBA:
          return 16;
      }
  }

  return 0xFFFFFFFF;
}
