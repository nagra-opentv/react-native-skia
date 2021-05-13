/*
 * Copyright 2020 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "gl/GLWindowContext.h"

#include "WindowContext.h"

#include "include/gpu/GrDirectContext.h"

namespace RnsShell {

WindowContext::WindowContext(const DisplayParams& params)
        : displayParams_(params)
        , sampleCount_(1)
        , stencilBits_(0) {}

WindowContext::~WindowContext() {}

}   //namespace RnsShell
