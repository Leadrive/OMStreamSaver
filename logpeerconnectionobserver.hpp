#ifndef LOGPEERCONNECTIONOBSERVER_H
#define LOGPEERCONNECTIONOBSERVER_H

#include <iostream>
#include "webrtc/api/peerconnectioninterface.h"

class LogPeerConnectionObserver : public webrtc::PeerConnectionObserver {
public:
    void OnSignalingChange(webrtc::PeerConnectionInterface::SignalingState new_state) override {
      std::cerr << "PeerConnectionObserver::SignalingChange(" << new_state << ")" << "\n";
    }

    void OnAddStream(webrtc::MediaStreamInterface* stream) override {
      std::cerr << "PeerConnectionObserver::AddStream\n";
    }

    void OnRemoveStream(webrtc::MediaStreamInterface* stream) override {
      std::cerr << "PeerConnectionObserver::RemoveStream\n";
    }

    void OnDataChannel(webrtc::DataChannelInterface* dataChannel) override {
        std::cerr << "PeerConnectionObserver::OnDataChannel\n";
    }

    void OnRenegotiationNeeded() override {
      std::cerr << "PeerConnectionObserver::RenegotiationNeeded\n";
    }

    void OnIceConnectionChange(webrtc::PeerConnectionInterface::IceConnectionState new_state) override {
      std::cerr << "PeerConnectionObserver::IceConnectionChange(" << new_state << ")" << "\n";
    }

    void OnIceGatheringChange(webrtc::PeerConnectionInterface::IceGatheringState new_state) override {
      std::cerr << "PeerConnectionObserver::IceGatheringChange(" << new_state << ")" << "\n";
    }

    void OnIceCandidate(const webrtc::IceCandidateInterface* candidate) override {
      std::cout << "PeerConnectionObserver::IceCandidate\n";
    }

    // managing is manual
    int AddRef() const {
        return 0;
    }

    int Release() const {
        return 0;
    }
};

#endif // LOGPEERCONNECTIONOBSERVER_H
