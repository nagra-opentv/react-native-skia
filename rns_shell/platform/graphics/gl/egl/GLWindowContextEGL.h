/*
 * Copyright 2016 Google Inc.
 * Copyright (C) 1994-2021 OpenTV, Inc. and Nagravision S.A.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#pragma once

#include "include/gpu/gl/GrGLInterface.h"

#include "gl/GLWindowContext.h"
#include "PlatformDisplay.h"
#include "WindowContextFactory.h"

#if PLATFORM(LIBWPE)
struct wpe_renderer_backend_egl_offscreen_target;
#endif

//using RnsShell::WCF::UnixWindowInfo;
using RnsShell::DisplayParams;
using RnsShell::GLWindowContext;

namespace RnsShell {

class GLWindowContextEGL : public GLWindowContext {
public:
    static std::unique_ptr<WindowContext> createContext(GLNativeWindowType window, PlatformDisplay& platformDisplay, const DisplayParams& params);
    static EGLContext createSharingContext(PlatformDisplay& platformDisplay);

    GLWindowContextEGL(GLNativeWindowType window, PlatformDisplay& platformDisplay, const DisplayParams& params, EGLContext context, EGLSurface surface);
    ~GLWindowContextEGL() override;

    static const char* errorString(int statusCode);
    static const char* eglErrorString();

    static bool isExtensionSupported(const char* extensionList, const char* extension);

    void onSwapBuffers() override;
    void onDestroyContext() override;

protected:
    sk_sp<const GrGLInterface> onInitializeContext() override;

private:
    static EGLContext createContextForEGLVersion(PlatformDisplay&, EGLConfig, EGLContext);

    GLWindowContextEGL(void*, const DisplayParams&);

    enum EGLSurfaceType { PbufferSurface, WindowSurface, PixmapSurface, Surfaceless };

    static std::unique_ptr<GLWindowContextEGL> createWindowContext(GLNativeWindowType, PlatformDisplay&, const DisplayParams&, EGLContext sharingContext = nullptr);

    bool makeContextCurrent() override;
    void swapInterval();

#if PLATFORM(X11)
    static EGLSurface createWindowSurfaceX11(EGLDisplay, EGLConfig, GLNativeWindowType);
#endif
    static bool getEGLConfig(EGLDisplay, EGLConfig*, EGLSurfaceType);

    GLNativeWindowType      window_;

    PlatformDisplay& platformDisplay_;
    EGLSurface glSurface_ { nullptr };
    EGLContext glContext_ { nullptr };
    EGLSurfaceType surfaceType_;

    typedef GLWindowContext INHERITED;
};

}  // namespace RnsShell
