/*
 * Copyright (C) 1994-2021 OpenTV, Inc. and Nagravision S.A.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#pragma once

#include "ReactSkia/utils/RnsUtils.h"

#if PLATFORM(LIBWPE) || USE(WPE_RENDERER)
#include <wpe/wpe.h>
#if USE(EGL)
#include <wpe/wpe-egl.h>
#endif

#include "PlatformDisplay.h"

namespace RnsShell {

class Display {
public:
    struct wpe_view_backend* viewBackend() const { return viewBackend_; }

    Display();

private:
    ~Display();
    struct wpe_view_backend* viewBackend_ = { nullptr };
};

class PlatformDisplayLibWPE final : public PlatformDisplay {
public:
    static std::unique_ptr<PlatformDisplayLibWPE> create();

    virtual ~PlatformDisplayLibWPE();

    bool initialize(int);

    Display* native() const { return display_; }
    struct wpe_renderer_backend_egl* renderBackend() const { return rendererBackend_; }

private:
    PlatformDisplayLibWPE(Display *display);

    Type type() const override { return PlatformDisplay::Type::WPE; }

    Display* display_;
    struct wpe_renderer_backend_egl* rendererBackend_ = { nullptr };
};

} // namespace RnsShell

#endif // USE(WPE_RENDERER)
