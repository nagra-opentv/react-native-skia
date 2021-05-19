#include "ReactSkia/RSkSurfaceWindow.h"
#include "ReactSkia/components/RSkComponent.h"

#include "include/core/SkString.h"

using namespace RnsShell;

namespace facebook {
namespace react {

RSkSurfaceWindow::RSkSurfaceWindow() {
  SkRect viewPort(SkRect::MakeWH(-1, -1));
  compositor_ = Compositor::create(viewPort);
}

RSkSurfaceWindow::~RSkSurfaceWindow() {
  compositor_->invalidate();
  compositor_.reset();
}

LayoutConstraints RSkSurfaceWindow::GetLayoutConstraints() {
  RNS_LOG_TODO("This is called too early , before we get window size");
  Size windowSize{static_cast<Float>(compositor_->viewport().width()),
                  static_cast<Float>(compositor_->viewport().height())};
  return {windowSize, windowSize};
}

} // namespace react
} // namespace facebook
