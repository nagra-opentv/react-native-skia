#include "ReactSkia/RSkSurfaceWindow.h"
#include "ReactSkia/components/RSkComponent.h"

#include "include/core/SkString.h"

using namespace RnsShell;

namespace facebook {
namespace react {

RSkSurfaceWindow::RSkSurfaceWindow() {
  SkRect viewPort(SkRect::MakeWH(1200, 800));
  compositor_ = Compositor::create(viewPort);
}

RSkSurfaceWindow::~RSkSurfaceWindow() {
  compositor_->invalidate();
  compositor_.reset();
}

LayoutConstraints RSkSurfaceWindow::GetLayoutConstraints() {
  RNS_LOG_TODO("Read width and height from compositor viewport attribute");
  Size windowSize{static_cast<Float>(1200/*compositor_->viewport.width()*/),
                  static_cast<Float>(800/*compositor_->viewport.height()*/)};
  return {windowSize, windowSize};
}

} // namespace react
} // namespace facebook
