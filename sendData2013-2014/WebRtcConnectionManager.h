#ifndef MYHEADER_H1
#define MYHEADER_H1

#include <iostream>
#include <string>
 
#include "talk/app/webrtc/peerconnectioninterface.h"
#include "talk/app/webrtc/jsep.h"
#include "talk/app/webrtc/datachannelinterface.h"
#include "talk/app/webrtc/test/fakeconstraints.h"
#include "talk/app/webrtc/test/mockpeerconnectionobservers.h"
#include "webrtc/base/json.h"
#include "webrtc/base/logging.h"
#include "webrtc/base/ssladapter.h"
#include "webrtc/base/sslstreamadapter.h"
#include "webrtc/base/thread.h"
#include "talk/app/webrtc/test/fakedtlsidentityservice.h"

#endif

using rtc::scoped_ptr;
using rtc::scoped_refptr;
using webrtc::MediaStreamInterface;
using webrtc::CreatePeerConnectionFactory;
using webrtc::DataChannelInterface;
using webrtc::MockDataChannelObserver;
using webrtc::PeerConnectionFactoryInterface;
using webrtc::PeerConnectionInterface;
using webrtc::PeerConnectionObserver;
using webrtc::PortAllocatorFactoryInterface;
using webrtc::VideoSourceInterface;
using webrtc::VideoTrackInterface;

class ChatDataChannelObserver : public webrtc::DataChannelObserver {
public:
explicit ChatDataChannelObserver(webrtc::DataChannelInterface* channel)
: channel_(channel) {
channel_->RegisterObserver(this);
state_ = channel_->state();
}
virtual ~ChatDataChannelObserver() {
channel_->UnregisterObserver();
}
 
virtual void OnStateChange() { state_ = channel_->state(); }
virtual void OnMessage(const webrtc::DataBuffer& buffer) {
std::string message_json = buffer.data.data();
Json::Reader reader;
Json::Value jdesc;
if (!reader.parse(message_json, jdesc)) {
LOG(WARNING) << "Unknown string desc " << message_json;
return;
}
 
// replace with global constants
std::string message = jdesc["message"].asString();
std::cout << message << std::endl;
}
 
bool IsOpen() const { return state_ == DataChannelInterface::kOpen; }
 
private:
rtc::scoped_refptr<webrtc::DataChannelInterface> channel_;
DataChannelInterface::DataState state_;
};

class DummySetSessionDescriptionObserver
: public webrtc::SetSessionDescriptionObserver {
public:
static DummySetSessionDescriptionObserver* Create() {
return
new rtc::RefCountedObject<DummySetSessionDescriptionObserver>();
}
virtual void OnSuccess() {
LOG(INFO) << __FUNCTION__;
}
virtual void OnFailure(const std::string& error) {
LOG(INFO) << __FUNCTION__ << " " << error;
}
 
protected:
DummySetSessionDescriptionObserver() {}
~DummySetSessionDescriptionObserver() {}
};

class WebRtcConnectionManager
: public webrtc::PeerConnectionObserver,
public webrtc::CreateSessionDescriptionObserver {
public:
WebRtcConnectionManager();
bool InitConnection();
void CreateOffer();
void OnOfferRequest(webrtc::SessionDescriptionInterface* desc);
void OnOfferReply(webrtc::SessionDescriptionInterface* desc);
 
static webrtc::SessionDescriptionInterface* StringToDescription(
const std::string string_desc);
static std::string DescriptionToString(
const webrtc::SessionDescriptionInterface* desc);
 
virtual const webrtc::SessionDescriptionInterface*
local_description() const {
return peer_connection_->local_description();
}
DataChannelInterface* data_channel() { return data_channel_; }
 
protected:
 
// inherited from PeerConnectionObserver
virtual void OnError();
virtual void OnStateChange(
webrtc::PeerConnectionObserver::StateType state_changed);
virtual void OnAddStream(webrtc::MediaStreamInterface* stream);
virtual void OnRemoveStream(webrtc::MediaStreamInterface* stream);
virtual void OnIceCandidate(const webrtc::IceCandidateInterface* candidate);
virtual void OnDataChannel(webrtc::DataChannelInterface* data_channel);
virtual void OnRenegotiationNeeded();
 
// inherited from CreateSessionDescriptionObserver
virtual void OnSuccess(webrtc::SessionDescriptionInterface* desc);
virtual void OnFailure(const std::string &error);
virtual int AddRef() { return 1; }
virtual int Release() { return 0; }
private:
rtc::scoped_refptr<webrtc::PeerConnectionFactoryInterface>
peer_connection_factory_;
webrtc::PeerConnectionInterface::IceServers servers_;
webrtc::PeerConnectionInterface::IceServer server_;
webrtc::FakeConstraints constraints_;
rtc::scoped_refptr<webrtc::PeerConnectionInterface> peer_connection_;
rtc::scoped_refptr<webrtc::AudioTrackInterface> audio_track_;
rtc::scoped_refptr<webrtc::MediaStreamInterface> stream_;
rtc::scoped_refptr<webrtc::DataChannelInterface> data_channel_;
};


