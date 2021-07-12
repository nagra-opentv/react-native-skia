#include "ReactSkia/RSkSurfaceWindow.h"
#include "ReactSkia/components/RSkComponent.h"

#include "include/core/SkString.h"

using namespace RnsShell;

namespace facebook {
namespace react {

GrDirectContext* RSkSurfaceWindow::directContext=nullptr;

RSkSurfaceWindow::RSkSurfaceWindow() {
  SkRect viewPort(SkRect::MakeEmpty());
  compositor_ = Compositor::create(viewPort);
  if(compositor_){
    setDirectContext(compositor_->getDirectContext());
  }
 }

RSkSurfaceWindow::~RSkSurfaceWindow() {
  compositor_->invalidate();
  compositor_.reset();
}

LayoutConstraints RSkSurfaceWindow::GetLayoutConstraints() {
  Size windowSize{static_cast<Float>(compositor_->viewport().width()),
                  static_cast<Float>(compositor_->viewport().height())};
  return {windowSize, windowSize};
}

void RSkSurfaceWindow::setDirectContext(GrDirectContext* context)
{
  RSkSurfaceWindow::directContext=context;
}

} // namespace react
} // namespace facebook
