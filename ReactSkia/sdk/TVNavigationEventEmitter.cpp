#include "TVNavigationEventEmitter.h"
#include <glog/logging.h>
#include <iostream>
#include "RNSKeyCodeMapping.h"
using namespace std;
namespace facebook {
namespace react {

RSkTVNavigationEventEmitter::RSkTVNavigationEventEmitter(
    const std::string &name, 
    std::shared_ptr<CallInvoker> jsInvoker,
    Instance *bridgeInstance)
    : RSkEventEmitter(name, jsInvoker, bridgeInstance)
{

}
void RSkTVNavigationEventEmitter::onTVKeyEvent(int  eventType,int eventKeyAction/*, const unsigned int tag*/) {
    cout <<"RSkTVNavigationEventEmitter::onTVKeyEvent "<<endl;
    folly::dynamic parameters = folly::dynamic::object();
    parameters["eventType"] = RNSKeyMap[eventType];
    parameters["tag"] = 0xFFFF;
    parameters["eventKeyAction"] = eventKeyAction;
    sendEventWithName(events_[0], folly::dynamic(parameters));
}

void RSkTVNavigationEventEmitter::startObserving() {
     cout <<"RSkTVNavigationEventEmitter::startObserving"<<endl;
    // use the NotificationManager to register for TV Keys
    std::function<void(int,int)> handler = std::bind(&RSkTVNavigationEventEmitter::onTVKeyEvent, this, std::placeholders::_1,std::placeholders::_2);
    eventId_ = keyEventNotification.addListener(tvEventName_, handler);
    cout <<"&&&&&&&&& tvEventName_  &&&&&&&&"<<tvEventName_<<endl;
}

void RSkTVNavigationEventEmitter::stopObserving() {
    cout <<"RSkTVNavigationEventEmitter::stopObserving"<<endl;
    keyEventNotification.removeListener(eventId_);
}

} //namespace react
} //namespace facebook
