/****************************************************************************
 ****************************************************************************
 ***
 ***   This header was generated from a ReactOS header to make
 ***   information necessary for userspace to call into the Windows
 ***   kernel available to Dr. Memory.  It contains only constants,
 ***   structures, and macros generated from the original header, and
 ***   thus, contains no copyrightable information.
 ***
 ****************************************************************************
 ****************************************************************************/

/* from reactos/include/psdk/d3dnthal.h */

/*
 * d3dnthal.h
 *
 * Direct3D NT driver interface
 *
 * Contributors:
 *   Created by Ge van Geldorp
 *
 * THIS SOFTWARE IS NOT COPYRIGHTED
 *
 * This source code is offered for use in the public domain. You may
 * use, modify or distribute it freely.
 *
 * This code is distributed in the hope that it will be useful but
 * WITHOUT ANY WARRANTY. ALL WARRANTIES, EXPRESS OR IMPLIED ARE HEREBY
 * DISCLAIMED. This includes but is not limited to warranties of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

#ifndef __DDK_D3DNTHAL_H
#define __DDK_D3DNTHAL_H

#include <ddrawint.h>
#include <d3dtypes.h>
#include <d3dcaps.h>

#ifdef __cplusplus
extern "C" {
#endif

DEFINE_GUID(GUID_D3DCallbacks, 0x7BF06990, 0x8794, 0x11D0, 0x91, 0x39, 0x08, 0x00, 0x36,
            0xD2, 0xEF, 0x02);
DEFINE_GUID(GUID_D3DCallbacks3, 0xDDF41230, 0xEC0A, 0x11D0, 0xA9, 0xB6, 0x00, 0xAA, 0x00,
            0xC0, 0x99, 0x3E);
DEFINE_GUID(GUID_D3DExtendedCaps, 0x7DE41F80, 0x9D93, 0x11D0, 0x89, 0xAB, 0x00, 0xA0,
            0xC9, 0x05, 0x41, 0x29);
DEFINE_GUID(GUID_D3DParseUnknownCommandCallback, 0x2E04FFA0, 0x98E4, 0x11D1, 0x8C, 0xE1,
            0x00, 0xA0, 0xC9, 0x06, 0x29, 0xA8);
DEFINE_GUID(GUID_ZPixelFormats, 0x93869880, 0x36CF, 0x11D1, 0x9B, 0x1B, 0x00, 0xAA, 0x00,
            0xBB, 0xB8, 0xAE);
DEFINE_GUID(GUID_DDStereoMode, 0xF828169C, 0xA8E8, 0x11D2, 0xA1, 0xF2, 0x00, 0xA0, 0xC9,
            0x83, 0xEA, 0xF6);

typedef struct _D3DNTHAL_CONTEXTCREATEDATA {
    __GNU_EXTENSION union {
        PDD_DIRECTDRAW_GLOBAL lpDDGbl;
        PDD_DIRECTDRAW_LOCAL lpDDLcl;
    };
    __GNU_EXTENSION union {
        PDD_SURFACE_LOCAL lpDDS;
        PDD_SURFACE_LOCAL lpDDSLcl;
    };
    __GNU_EXTENSION union {
        PDD_SURFACE_LOCAL lpDDSZ;
        PDD_SURFACE_LOCAL lpDDSZLcl;
    };
    DWORD dwPID;
    ULONG_PTR dwhContext;
    HRESULT ddrval;
} D3DNTHAL_CONTEXTCREATEDATA, *LPD3DNTHAL_CONTEXTCREATEDATA;

typedef struct _D3DNTHAL_CONTEXTDESTROYDATA {
    ULONG_PTR dwhContext;
    HRESULT ddrval;
} D3DNTHAL_CONTEXTDESTROYDATA, *LPD3DNTHAL_CONTEXTDESTROYDATA;

typedef struct _D3DNTHAL_CONTEXTDESTROYALLDATA {
    DWORD dwPID;
    HRESULT ddrval;
} D3DNTHAL_CONTEXTDESTROYALLDATA, *LPD3DNTHAL_CONTEXTDESTROYALLDATA;

typedef struct _D3DNTHAL_SCENECAPTUREDATA {
    ULONG_PTR dwhContext;
    DWORD dwFlag;
    HRESULT ddrval;
} D3DNTHAL_SCENECAPTUREDATA, *LPD3DNTHAL_SCENECAPTUREDATA;

typedef struct _D3DNTHAL_TEXTURECREATEDATA {
    ULONG_PTR dwhContext;
    HANDLE hDDS;
    ULONG_PTR dwHandle;
    HRESULT ddrval;
} D3DNTHAL_TEXTURECREATEDATA, *LPD3DNTHAL_TEXTURECREATEDATA;

typedef struct _D3DNTHAL_TEXTUREDESTROYDATA {
    ULONG_PTR dwhContext;
    ULONG_PTR dwHandle;
    HRESULT ddrval;
} D3DNTHAL_TEXTUREDESTROYDATA, *LPD3DNTHAL_TEXTUREDESTROYDATA;

typedef struct _D3DNTHAL_TEXTURESWAPDATA {
    ULONG_PTR dwhContext;
    ULONG_PTR dwHandle1;
    ULONG_PTR dwHandle2;
    HRESULT ddrval;
} D3DNTHAL_TEXTURESWAPDATA, *LPD3DNTHAL_TEXTURESWAPDATA;

typedef struct _D3DNTHAL_TEXTUREGETSURFDATA {
    ULONG_PTR dwhContext;
    HANDLE hDDS;
    ULONG_PTR dwHandle;
    HRESULT ddrval;
} D3DNTHAL_TEXTUREGETSURFDATA, *LPD3DNTHAL_TEXTUREGETSURFDATA;

typedef DWORD(APIENTRY *LPD3DNTHAL_CONTEXTCREATECB)(LPD3DNTHAL_CONTEXTCREATEDATA);
typedef DWORD(APIENTRY *LPD3DNTHAL_CONTEXTDESTROYCB)(LPD3DNTHAL_CONTEXTDESTROYDATA);
typedef DWORD(APIENTRY *LPD3DNTHAL_CONTEXTDESTROYALLCB)(LPD3DNTHAL_CONTEXTDESTROYALLDATA);
typedef DWORD(APIENTRY *LPD3DNTHAL_SCENECAPTURECB)(LPD3DNTHAL_SCENECAPTUREDATA);
typedef DWORD(APIENTRY *LPD3DNTHAL_TEXTURECREATECB)(LPD3DNTHAL_TEXTURECREATEDATA);
typedef DWORD(APIENTRY *LPD3DNTHAL_TEXTUREDESTROYCB)(LPD3DNTHAL_TEXTUREDESTROYDATA);
typedef DWORD(APIENTRY *LPD3DNTHAL_TEXTURESWAPCB)(LPD3DNTHAL_TEXTURESWAPDATA);
typedef DWORD(APIENTRY *LPD3DNTHAL_TEXTUREGETSURFCB)(LPD3DNTHAL_TEXTUREGETSURFDATA);

typedef struct _D3DNTHALDeviceDesc_V1 {
    DWORD dwSize;
    DWORD dwFlags;
    D3DCOLORMODEL dcmColorModel;
    DWORD dwDevCaps;
    D3DTRANSFORMCAPS dtcTransformCaps;
    BOOL bClipping;
    D3DLIGHTINGCAPS dlcLightingCaps;
    D3DPRIMCAPS dpcLineCaps;
    D3DPRIMCAPS dpcTriCaps;
    DWORD dwDeviceRenderBitDepth;
    DWORD dwDeviceZBufferBitDepth;
    DWORD dwMaxBufferSize;
    DWORD dwMaxVertexCount;
} D3DNTHALDEVICEDESC_V1, *LPD3DNTHALDEVICEDESC_V1;

typedef struct _D3DNTHAL_GLOBALDRIVERDATA {
    DWORD dwSize;
    D3DNTHALDEVICEDESC_V1 hwCaps;
    DWORD dwNumVertices;
    DWORD dwNumClipVertices;
    DWORD dwNumTextureFormats;
    LPDDSURFACEDESC lpTextureFormats;
} D3DNTHAL_GLOBALDRIVERDATA, *LPD3DNTHAL_GLOBALDRIVERDATA;

typedef struct _D3DNTHAL_CALLBACKS {
    DWORD dwSize;
    LPD3DNTHAL_CONTEXTCREATECB ContextCreate;
    LPD3DNTHAL_CONTEXTDESTROYCB ContextDestroy;
    LPD3DNTHAL_CONTEXTDESTROYALLCB ContextDestroyAll;
    LPD3DNTHAL_SCENECAPTURECB SceneCapture;
    LPVOID dwReserved10;
    LPVOID dwReserved11;
    LPVOID dwReserved22;
    LPVOID dwReserved23;
    ULONG_PTR dwReserved;
    LPD3DNTHAL_TEXTURECREATECB TextureCreate;
    LPD3DNTHAL_TEXTUREDESTROYCB TextureDestroy;
    LPD3DNTHAL_TEXTURESWAPCB TextureSwap;
    LPD3DNTHAL_TEXTUREGETSURFCB TextureGetSurf;
    LPVOID dwReserved12;
    LPVOID dwReserved13;
    LPVOID dwReserved14;
    LPVOID dwReserved15;
    LPVOID dwReserved16;
    LPVOID dwReserved17;
    LPVOID dwReserved18;
    LPVOID dwReserved19;
    LPVOID dwReserved20;
    LPVOID dwReserved21;
    LPVOID dwReserved24;
    ULONG_PTR dwReserved0;
    ULONG_PTR dwReserved1;
    ULONG_PTR dwReserved2;
    ULONG_PTR dwReserved3;
    ULONG_PTR dwReserved4;
    ULONG_PTR dwReserved5;
    ULONG_PTR dwReserved6;
    ULONG_PTR dwReserved7;
    ULONG_PTR dwReserved8;
    ULONG_PTR dwReserved9;
} D3DNTHAL_CALLBACKS, *LPD3DNTHAL_CALLBACKS;

typedef struct _D3DNTHAL_SETRENDERTARGETDATA {
    ULONG_PTR dwhContext;
    PDD_SURFACE_LOCAL lpDDS;
    PDD_SURFACE_LOCAL lpDDSZ;
    HRESULT ddrval;
} D3DNTHAL_SETRENDERTARGETDATA, *LPD3DNTHAL_SETRENDERTARGETDATA;

typedef DWORD(APIENTRY *LPD3DNTHAL_SETRENDERTARGETCB)(LPD3DNTHAL_SETRENDERTARGETDATA);

typedef struct _D3DNTHAL_CALLBACKS2 {
    DWORD dwSize;
    DWORD dwFlags;
    LPD3DNTHAL_SETRENDERTARGETCB SetRenderTarget;
    LPVOID dwReserved1;
    LPVOID dwReserved2;
    LPVOID dwReserved3;
    LPVOID dwReserved4;
} D3DNTHAL_CALLBACKS2, *LPD3DNTHAL_CALLBACKS2;

typedef struct _D3DNTHAL_CLEAR2DATA {
    ULONG_PTR dwhContext;
    DWORD dwFlags;
    DWORD dwFillColor;
    D3DVALUE dvFillDepth;
    DWORD dwFillStencil;
    LPD3DRECT lpRects;
    DWORD dwNumRects;
    HRESULT ddrval;
} D3DNTHAL_CLEAR2DATA, FAR *LPD3DNTHAL_CLEAR2DATA;

typedef struct _D3DNTHAL_VALIDATETEXTURESTAGESTATEDATA {
    ULONG_PTR dwhContext;
    DWORD dwFlags;
    ULONG_PTR dwReserved;
    DWORD dwNumPasses;
    HRESULT ddrval;
} D3DNTHAL_VALIDATETEXTURESTAGESTATEDATA, FAR *LPD3DNTHAL_VALIDATETEXTURESTAGESTATEDATA;

typedef struct _D3DNTHAL_DRAWPRIMITIVES2DATA {
    ULONG_PTR dwhContext;
    DWORD dwFlags;
    DWORD dwVertexType;
    PDD_SURFACE_LOCAL lpDDCommands;
    DWORD dwCommandOffset;
    DWORD dwCommandLength;
    __GNU_EXTENSION union {
        PDD_SURFACE_LOCAL lpDDVertex;
        LPVOID lpVertices;
    };
    DWORD dwVertexOffset;
    DWORD dwVertexLength;
    DWORD dwReqVertexBufSize;
    DWORD dwReqCommandBufSize;
    LPDWORD lpdwRStates;
    __GNU_EXTENSION union {
        DWORD dwVertexSize;
        HRESULT ddrval;
    };
    DWORD dwErrorOffset;
} D3DNTHAL_DRAWPRIMITIVES2DATA, FAR *LPD3DNTHAL_DRAWPRIMITIVES2DATA;

typedef DWORD(APIENTRY *LPD3DNTHAL_CLEAR2CB)(LPD3DNTHAL_CLEAR2DATA);
typedef DWORD(APIENTRY *LPD3DNTHAL_VALIDATETEXTURESTAGESTATECB)(
    LPD3DNTHAL_VALIDATETEXTURESTAGESTATEDATA);
typedef DWORD(APIENTRY *LPD3DNTHAL_DRAWPRIMITIVES2CB)(LPD3DNTHAL_DRAWPRIMITIVES2DATA);

typedef struct _D3DNTHAL_CALLBACKS3 {
    DWORD dwSize;
    DWORD dwFlags;
    LPD3DNTHAL_CLEAR2CB Clear2;
    LPVOID lpvReserved;
    LPD3DNTHAL_VALIDATETEXTURESTAGESTATECB ValidateTextureStageState;
    LPD3DNTHAL_DRAWPRIMITIVES2CB DrawPrimitives2;
} D3DNTHAL_CALLBACKS3, *LPD3DNTHAL_CALLBACKS3;

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __DDK_D3DNTHAL_H */
