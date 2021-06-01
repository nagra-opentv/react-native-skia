#include "ReactSkia/components/RSkComponentImage.h"
#include "include/core/SkBitmap.h"
#include "include/core/SkData.h"
#include "include/core/SkImageGenerator.h"
#include "include/core/SkPaint.h"
#include "ReactSkia/views/common/RSkDrawUtils.h"
#include "react/renderer/components/image/ImageShadowNode.h"

#include <glog/logging.h>

namespace facebook {
namespace react {

using namespace RSkDrawUtils;

namespace {
std::unique_ptr<SkBitmap> GetAsset(const char *path) {
  sk_sp<SkData> data = SkData::MakeFromFileName(path);
  if (!data) {
    LOG(ERROR)
        << "RSkComponentImage::_::GetAsset() - Unable to make SkData from path: "
        << path;
    return nullptr;
  }

  std::unique_ptr<SkImageGenerator> gen(
      SkImageGenerator::MakeFromEncoded(std::move(data)));
  if (!gen) {
    return nullptr;
  }
  auto bitmap = std::make_unique<SkBitmap>();
  bool success = gen && bitmap->tryAllocPixels(gen->getInfo()) &&
      gen->getPixels(
          gen->getInfo().makeColorSpace(nullptr),
          bitmap->getPixels(),
          bitmap->rowBytes());
  if (!success) {
    return nullptr;
  }
  return bitmap;
}

} // namespace

RSkComponentImage::RSkComponentImage(const ShadowView &shadowView)
    : RSkComponent(shadowView) {}

void RSkComponentImage::OnPaint(
    SkCanvas *canvas) {
  auto component = getComponentData();
  auto const &imageProps =
      *std::static_pointer_cast<ImageProps const>(component.props);
  if (imageProps.sources.empty()) {
    return;
  }
  const auto source = imageProps.sources[0];

  if (source.type == ImageSource::Type::Local && !source.uri.empty()) {
    assert(source.uri.substr(0, 14) == "file://assets/");
    std::string path = "./" + source.uri.substr(7);
    auto bitmap = GetAsset(path.c_str());
    assert(bitmap);

    Rect frame = getAbsoluteFrame();
    SkRect rect = SkRect::MakeXYWH(frame.origin.x, frame.origin.y, frame.size.width, frame.size.height);
    auto const &imageBorderMetrics=imageProps.resolveBorderMetrics(component.layoutMetrics);

    /* Draw order 1. Background 2. Image 3. Border*/
    drawBackground(canvas,frame,imageBorderMetrics,imageProps.backgroundColor,imageProps.opacity);
    canvas->drawBitmapRect(*bitmap, rect, nullptr);
    drawBorder(canvas,frame,imageBorderMetrics,imageProps.backgroundColor,imageProps.opacity);
  }
}

} // namespace react
} // namespace facebook
