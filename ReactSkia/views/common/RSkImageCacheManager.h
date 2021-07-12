/*
 * Copyright (C) 1994-2021 OpenTV, Inc. and Nagravision S.A.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */
#pragma once

#include "include/core/SkImage.h"
#include "include/core/SkGraphics.h"
#include "include/gpu/GrDirectContext.h"

#define SKIA_CPU_IMAGE_CACHE_LIMIT  50*1024*1024 
#define SKIA_GPU_IMAGE_CACHE_LIMIT  50*1024*1024

namespace facebook {
namespace react {

namespace RSkImageCacheManager{
  
  sk_sp<SkImage> getImageData(const char *path); // Interface to get teh extarcted image Data
  void printCacheUsage();

} //namespace RSkImageCacheManager

} // namespace react
} // namespace facebook



