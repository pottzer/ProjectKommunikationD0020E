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


#include "WebRtcConnectionManager.h"


WebRtcConnectionManager::WebRtcConnectionManager() 
{
const char kStunServerUri[] = "stun:stun.l.google.com:19302";
peer_connection_factory_ = webrtc::CreatePeerConnectionFactory();
server_.uri = kStunServerUri;
servers_.push_back(server_);
constraints_.SetMandatoryReceiveAudio(false);
constraints_.SetMandatoryReceiveVideo(false);
}

bool WebRtcConnectionManager::InitConnection() 
{

const char kAudioLabel[] = "audio_label";
const char kStreamLabel[] = "stream_label";

FakeIdentityService* dtls_service = rtc::SSLStreamAdapter::HaveDtlsSrtp() ? new FakeIdentityService() : NULL;
peer_connection_ = peer_connection_factory_->CreatePeerConnection(servers_, NULL, NULL, dtls_service, this);
audio_track_ = peer_connection_factory_->CreateAudioTrack(kAudioLabel,
NULL);
stream_ = peer_connection_factory_->CreateLocalMediaStream(kStreamLabel);
stream_->AddTrack(audio_track_);
peer_connection_->AddStream(stream_);
data_channel_ = peer_connection_->CreateDataChannel("test1", NULL);
data_channel_->RegisterObserver(new ChatDataChannelObserver(data_channel_));
return true;
}
 
void WebRtcConnectionManager::CreateOffer() {
peer_connection_->CreateOffer(this, &constraints_);
}
 
void WebRtcConnectionManager::OnOfferRequest(
webrtc::SessionDescriptionInterface* desc) {
peer_connection_->SetRemoteDescription(
DummySetSessionDescriptionObserver::Create(), desc);
peer_connection_->CreateAnswer(this, &constraints_);
}
 
void WebRtcConnectionManager::OnOfferReply(
webrtc::SessionDescriptionInterface* desc) {
peer_connection_->SetRemoteDescription(
DummySetSessionDescriptionObserver::Create(), desc);
}
 
void WebRtcConnectionManager::OnError() {
std::cout << "PEERCONNECTION ERROR" << std::endl;
}
 
void WebRtcConnectionManager::OnStateChange(
webrtc::PeerConnectionObserver::StateType state_changed) {
}
 
void WebRtcConnectionManager::OnAddStream(
webrtc::MediaStreamInterface* stream) {
}
 
void WebRtcConnectionManager::OnRemoveStream(
webrtc::MediaStreamInterface* stream) {
}
 
void WebRtcConnectionManager::OnIceCandidate(
const webrtc::IceCandidateInterface* candidate) {
}
 
void WebRtcConnectionManager::OnSuccess(
webrtc::SessionDescriptionInterface* desc) {
peer_connection_->SetLocalDescription(
DummySetSessionDescriptionObserver::Create(), desc);
}
 
void WebRtcConnectionManager::OnDataChannel(
webrtc::DataChannelInterface* data_channel) {
}
 
void WebRtcConnectionManager::OnRenegotiationNeeded() {
}
 
void WebRtcConnectionManager::OnFailure(const std::string &error) {
std::cout << error << std::endl;
}
 
webrtc::SessionDescriptionInterface*
WebRtcConnectionManager::StringToDescription(const std::string string_desc) {
Json::Reader reader;
Json::Value jdesc;
if (!reader.parse(string_desc, jdesc)) {
LOG(WARNING) << "Unknown string desc " << string_desc;
return NULL;
}
 
// replace with global constants
std::string type = jdesc["type"].asString();
std::string sdp = jdesc["sdp"].asString();
return webrtc::CreateSessionDescription(type, sdp);
}
 
std::string WebRtcConnectionManager::DescriptionToString(
const webrtc::SessionDescriptionInterface* desc) {
Json::FastWriter writer;
Json::Value jdesc;
jdesc["type"] = desc->type();
std::string sdp;
desc->ToString(&sdp);
jdesc["sdp"] = sdp;
return writer.write(jdesc);
}
