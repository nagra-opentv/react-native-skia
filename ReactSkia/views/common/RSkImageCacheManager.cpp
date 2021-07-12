 /*
 * Copyright (C) 1994-2021 OpenTV, Inc. and Nagravision S.A.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */
#include "ReactSkia/views/common/RSkImageCacheManager.h"
#include "include/core/SkData.h"
#include "ReactSkia/RSkSurfaceWindow.h"
#include "ReactSkia/utils/RnsLog.h"
#include <string>
#include <iterator>
#include <map>

using namespace std;

namespace facebook {
namespace react {

namespace RSkImageCacheManager{
   
namespace {
  
  map<size_t, sk_sp<SkImage>> ImageCacheMap;
  size_t cpuCacheLimit_{0},gpuCacheLimit_{0};

  void setCpuImageCacheLimit(size_t cacheLimit)
  {
    SkGraphics::SetResourceCacheTotalByteLimit(cacheLimit);
    cpuCacheLimit_ = SkGraphics::GetResourceCacheTotalByteLimit();
  }

  void setGpuImageCacheLimit(size_t cacheLimit)
  {
    auto gpuContext = RSkSurfaceWindow::getDirectContext();
    if(gpuContext){
      gpuContext->setResourceCacheLimit(cacheLimit);  
      gpuCacheLimit_ = gpuContext->getResourceCacheLimit();
    }
  }

  sk_sp<SkImage> makeImageData(const char *path)
  {
    sk_sp<SkData> data = SkData::MakeFromFileName(path);
    if (!data) {
      RNS_LOG_ERROR("Unable to make SkData for path :" << path);
      return nullptr;
    }
    return( SkImage::MakeFromEncoded(data ));
  }
  
  bool evictAsNeeded()
  {
    size_t cpuCacheUsed{0} , gpuCacheUsed{0};
    int fOldCount{0};
    map<size_t, sk_sp<SkImage>>::iterator it;
    auto gpuContext =RSkSurfaceWindow::getDirectContext();
    //Reading used memory on CPU/GU Caches
    if(gpuContext){
      gpuContext->getResourceCacheUsage(&fOldCount, &gpuCacheUsed);       
    }
    cpuCacheUsed = SkGraphics::GetResourceCacheTotalBytesUsed();
    #ifdef RNS_IMAGECACHING_DEBUG
      RNS_LOG_DEBUG("CPU CACHE consumed bytes: "<<cpuCacheUsed<< ",, GPU CACHE consumed bytes: "<<gpuCacheUsed);
    #endif
    //eveict unrefferended entries, on exceeding the memory limit
    for(it = ImageCacheMap.begin();it != ImageCacheMap.end();it++) {
      if((cpuCacheUsed < SKIA_CPU_IMAGE_CACHE_LIMIT) && ( gpuCacheUsed < SKIA_GPU_IMAGE_CACHE_LIMIT))
        break;
      if( (it->second)->unique()) {
        //Evicting unowned entries from hash map
          ImageCacheMap.erase(it);
          it = ImageCacheMap.begin();
   	  }
    }
    return ((cpuCacheUsed < SKIA_CPU_IMAGE_CACHE_LIMIT) && ( gpuCacheUsed < SKIA_GPU_IMAGE_CACHE_LIMIT));
  }

}//namespace

sk_sp<SkImage> getImageData(const char *path)
{
  hash<string> mystdhash; 
  sk_sp<SkImage> ImageData{nullptr};
  //Set Cache limit, if not set before
  if(cpuCacheLimit_ != SKIA_CPU_IMAGE_CACHE_LIMIT)
    setCpuImageCacheLimit(SKIA_CPU_IMAGE_CACHE_LIMIT);
  if(gpuCacheLimit_ != SKIA_GPU_IMAGE_CACHE_LIMIT)
    setGpuImageCacheLimit(SKIA_GPU_IMAGE_CACHE_LIMIT);

  if(path == nullptr) {
    RNS_LOG_ERROR("Invalid File");
	  return nullptr;
  }
  #ifdef RNS_IMAGECACHING_DEBUG
    RNS_LOG_DEBUG("Number of entries in HASH map :"<<ImageCacheMap.size());
  #endif // RNS_IMAGECACHING_DEBUG
  if(ImageCacheMap.empty() || (ImageCacheMap.find(mystdhash(path))== ImageCacheMap.end())) {
    ImageData = makeImageData(path);
    //Add entry to hash map only if the cache mem usage is with in the limit
    if(evictAsNeeded()) {
      ImageCacheMap.insert(pair<size_t, sk_sp<SkImage>>(mystdhash(path),ImageData));
    }
    else {
      #ifdef RNS_IMAGECACHING_DEBUG
        RNS_LOG_DEBUG("mem limit Reached , file is not cached ...");
      #endif
    }
  }
  //fetch from hashMap
  if(ImageData == nullptr){
    map<size_t, sk_sp<SkImage>>::iterator it = ImageCacheMap.find(mystdhash(path));
    ImageData= (it != ImageCacheMap.end()) ? it->second : nullptr;
  }
  return ImageData;
}
#ifdef RNS_IMAGECACHING_DEBUG
void printCacheUsage()
{
  size_t cpuUsedMem{0},gpuUsedMem{0};
  static size_t prevCpuUsedMem{0},prevGpuUsedMem{0};
  int fOldCount{0};

  cpuUsedMem = SkGraphics::GetResourceCacheTotalBytesUsed(); 
  auto gpuContext =RSkSurfaceWindow::getDirectContext();
  if(gpuContext) {
    gpuContext->getResourceCacheUsage(&fOldCount, &gpuUsedMem);
    RNS_LOG_DEBUG("Memory consumed for this run in GPU CACHE:"<<((gpuUsedMem > prevGpuUsedMem)?(gpuUsedMem - prevGpuUsedMem) :gpuUsedMem));       
  }
  RNS_LOG_DEBUG("Memory consumed for this run in CPU CACHE :"<<((cpuUsedMem > prevCpuUsedMem)?(cpuUsedMem - prevCpuUsedMem):cpuUsedMem));
  prevCpuUsedMem = cpuUsedMem;
  prevGpuUsedMem = gpuUsedMem;
}
#endif/*RNS_IMAGECACHING_DEBUG*/
} //RSkImageCacheManager

} // namespace react
} // namespace facebook
