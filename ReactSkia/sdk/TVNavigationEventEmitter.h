#include "NotificationCenter.h"
#include "cxxreact/Instance.h"
#include "core_modules/RSkEventEmitter.h"
#include "tools/skui/Key.h"

namespace facebook {
namespace react {

class RSkTVNavigationEventEmitter final: public RSkEventEmitter {
    private:
        std::vector<std::string> events_ = {"onHWKeyEvent"};
        std::string tvEventName_ = "RCTTVNavigationEventNotification";
        unsigned int eventId_ = 0xFFF; // Some undefined ID

        NotificationCenter keyEventNotification;

        void onTVKeyEvent(int  eventType,int eventKeyAction/*, const unsigned int*/);

    public:
        RSkTVNavigationEventEmitter(
            const std::string &name, 
            std::shared_ptr<CallInvoker> jsInvoker,
            Instance *bridgeInstance);

        ~RSkTVNavigationEventEmitter() {}

        std::vector<std::string> supportedEvents() {
            return events_;
        }

        void startObserving();

        void stopObserving();
};

} //namespace react
} //namespace facebook
