/*
 * Copyright 2016 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#pragma once

#include "ReactSkia/utils/RnsLog.h"

#include "include/core/SkRefCnt.h"
#include "include/core/SkSurfaceProps.h"
#include "include/gpu/GrTypes.h"

#include "DisplayParams.h"

class GrDirectContext;
class SkSurface;

namespace RnsShell {

class WindowContext {
public:
    WindowContext(const DisplayParams&);

    virtual ~WindowContext();

    virtual sk_sp<SkSurface> getBackbufferSurface() = 0;

    virtual void swapBuffers() = 0;
    virtual bool makeContextCurrent() = 0;

    virtual bool isValid() = 0;

    const DisplayParams& getDisplayParams() { return displayParams_; }
    virtual void setDisplayParams(const DisplayParams& params) = 0;

    GrDirectContext* directContext() const { return context_.get(); }

    int width() const { return width_; }
    int height() const { return height_; }
    int sampleCount() const { return sampleCount_; }
    int stencilBits() const { return stencilBits_; }

protected:
    virtual bool isGpuContext() { return true;  }

    sk_sp<GrDirectContext> context_;

    int               width_;
    int               height_;
    DisplayParams     displayParams_;

    // parameters obtained from the native window
    // Note that the platform .cpp file is responsible for
    // initializing sampleCount_ and stencilBits_!
    int               sampleCount_;
    int               stencilBits_;
};

}   // namespace RnsShell

