/*
 * Copyright (C) 1994-2021 OpenTV, Inc. and Nagravision S.A.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#pragma once

#include "include/core/SkTime.h"

#define PLATFORM(RNS_FEATURE) (defined RNS_PLATFORM_##RNS_FEATURE && RNS_PLATFORM_##RNS_FEATURE)
#define USE(RNS_FEATURE) (defined USE_##RNS_FEATURE && USE_##RNS_FEATURE)

#define RNS_EXPORT __attribute__((visibility("default")))
#define RNS_UNUSED(val) (void)val;
#define RNS_MAKE_NONCOPYABLE(ClassName) \
    private: \
        ClassName(const ClassName&) = delete; \
        ClassName& operator=(const ClassName&) = delete; \

#define RNS_GET_TIME_STAMP_MS(marker) \
    double marker = SkTime::GetMSecs();

#define RNS_GET_TIME_STAMP_US(marker) \
    double marker = SkTime::GetNSecs() * 1e-3;

#define RNS_GET_TIME_STAMP_NS(marker) \
    double marker = SkTime::GetNSecs();
