// gfx_OpenGL_macOS.h //                          Author: Logan Jones
///////////////////////                               Date: 5/15/2016
//
/////////////////////////////////////////////////////////////////////
#ifndef _GFX_OPENGL_MACOS_H_
#define _GFX_OPENGL_MACOS_H_
/////////////////////////////////////////////////////////////////////


#include <OpenGL/gl.h>

#ifdef __cplusplus
extern "C" {
#endif


#ifndef APIENTRY
#define APIENTRY
#endif
#ifndef APIENTRYP
#define APIENTRYP APIENTRY *
#endif
#ifndef GLAPI
#define GLAPI extern
#endif
    
    
//#define GL_GLEXT_VERSION 20160428

    
#ifndef GL_EXT_paletted_texture
#define GL_EXT_paletted_texture 1
#define GL_COLOR_INDEX1_EXT               0x80E2
#define GL_COLOR_INDEX2_EXT               0x80E3
#define GL_COLOR_INDEX4_EXT               0x80E4
#define GL_COLOR_INDEX8_EXT               0x80E5
#define GL_COLOR_INDEX12_EXT              0x80E6
#define GL_COLOR_INDEX16_EXT              0x80E7
#define GL_TEXTURE_INDEX_SIZE_EXT         0x80ED
typedef void (APIENTRYP PFNGLCOLORTABLEEXTPROC) (GLenum target, GLenum internalFormat, GLsizei width, GLenum format, GLenum type, const void *table);
typedef void (APIENTRYP PFNGLGETCOLORTABLEEXTPROC) (GLenum target, GLenum format, GLenum type, void *data);
typedef void (APIENTRYP PFNGLGETCOLORTABLEPARAMETERIVEXTPROC) (GLenum target, GLenum pname, GLint *params);
typedef void (APIENTRYP PFNGLGETCOLORTABLEPARAMETERFVEXTPROC) (GLenum target, GLenum pname, GLfloat *params);
#ifdef GL_GLEXT_PROTOTYPES
GLAPI void APIENTRY glColorTableEXT (GLenum target, GLenum internalFormat, GLsizei width, GLenum format, GLenum type, const void *table);
GLAPI void APIENTRY glGetColorTableEXT (GLenum target, GLenum format, GLenum type, void *data);
GLAPI void APIENTRY glGetColorTableParameterivEXT (GLenum target, GLenum pname, GLint *params);
GLAPI void APIENTRY glGetColorTableParameterfvEXT (GLenum target, GLenum pname, GLfloat *params);
#endif
#endif /* GL_EXT_paletted_texture */
    
#ifndef GL_EXT_shared_texture_palette
#define GL_EXT_shared_texture_palette 1
#define GL_SHARED_TEXTURE_PALETTE_EXT     0x81FB
#endif /* GL_EXT_shared_texture_palette */

    
#ifdef __cplusplus
}
#endif


/////////////////////////////////////////////////////////////////////
#endif // !defined(_GFX_OPENGL_MACOS_H_)
