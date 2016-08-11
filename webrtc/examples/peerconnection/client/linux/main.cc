/*
 *  Copyright 2012 The WebRTC Project Authors. All rights reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include "webrtc/examples/peerconnection/client/conductor.h"
#include "webrtc/examples/peerconnection/client/flagdefs.h"
#include "webrtc/examples/peerconnection/client/linux/main_wnd.h"
#include "webrtc/examples/peerconnection/client/peer_connection_client.h"

#include "webrtc/base/ssladapter.h"
#include "webrtc/base/thread.h"
#include "webrtc/api/test/fakeconstraints.h"
#include "webrtc/examples/peerconnection/client/defaults.h"

#include "webrtc/api/test/fakertccertificategenerator.h"
#include "webrtc/p2p/base/fakeportallocator.h"
#include "webrtc/api/test/fakeaudiocapturemodule.h"

#include <iostream>

class Connection {
 public:
  rtc::scoped_refptr<webrtc::PeerConnectionInterface> peer_connection;
  rtc::scoped_refptr<webrtc::DataChannelInterface> data_channel;
  std::string sdp_type;

  // Offer/Answer
  void onSuccessCSD(webrtc::SessionDescriptionInterface* desc) {
    peer_connection->SetLocalDescription(&ssdo, desc);

    std::string sdp;
    desc->ToString(&sdp);
    std::cout << sdp_type << " SDP:begin" << std::endl << sdp << sdp_type << " SDP:end" << std::endl;
  }

  void onIceCandidate(const webrtc::IceCandidateInterface* candidate) {
      std::cerr << "onIceCandidate\n";
  }

  class PCO : public webrtc::PeerConnectionObserver {
   private:
    Connection& parent;

   public:
    PCO(Connection& parent) : parent(parent) {}

    void OnSignalingChange(webrtc::PeerConnectionInterface::SignalingState new_state) override {
      std::cout << "PeerConnectionObserver::SignalingChange(" << new_state << ")" << std::endl;
    }

    void OnAddStream(webrtc::MediaStreamInterface* stream) override {
      std::cout << "PeerConnectionObserver::AddStream" << std::endl;
    }

    void OnRemoveStream(webrtc::MediaStreamInterface* stream) override {
      std::cout << "PeerConnectionObserver::RemoveStream" << std::endl;
    }

    void OnDataChannel(webrtc::DataChannelInterface* data_channel) override {
      std::cout << "PeerConnectionObserver::DataChannel(" << data_channel
                << ", " << parent.data_channel.get() << ")" << std::endl;
      parent.data_channel = data_channel;
    }

    void OnRenegotiationNeeded() override {
      std::cout << "PeerConnectionObserver::RenegotiationNeeded" << std::endl;
    }

    void OnIceConnectionChange(webrtc::PeerConnectionInterface::IceConnectionState new_state) override {
      std::cout << "PeerConnectionObserver::IceConnectionChange(" << new_state << ")" << std::endl;
    }

    void OnIceGatheringChange(webrtc::PeerConnectionInterface::IceGatheringState new_state) override {
      std::cout << "PeerConnectionObserver::IceGatheringChange(" << new_state << ")" << std::endl;
    }

    void OnIceCandidate(const webrtc::IceCandidateInterface* candidate) override {
      std::cout << "PeerConnectionObserver::IceCandidate" << std::endl;
      parent.onIceCandidate(candidate);
    }
  };

  class DCO : public webrtc::DataChannelObserver {
   public:
    DCO() {}

    void OnStateChange() override {
      std::cout << "DataChannelObserver::StateChange" << std::endl;
    }

    void OnMessage(const webrtc::DataBuffer& buffer) override {
      std::cout << "DataChannelObserver::Message" << std::endl;
      std::cout << buffer.data.data<char>() << std::endl;
    }

    void OnBufferedAmountChange(uint64_t previous_amount) override {
      std::cout << "DataChannelObserver::BufferedAmountChange(" << previous_amount << ")" << std::endl;
    }
  };

  class CSDO : public webrtc::CreateSessionDescriptionObserver {
   private:
    Connection& parent;

   public:
    CSDO(Connection& parent) : parent(parent) {}

    void OnSuccess(webrtc::SessionDescriptionInterface* desc) override {
      std::cout << "CreateSessionDescriptionObserver::OnSuccess" << std::endl;
      parent.onSuccessCSD(desc);
    }

    void OnFailure(const std::string& error) override {
      std::cout << "CreateSessionDescriptionObserver::OnFailure" << std::endl << error << std::endl;
    }

    int AddRef() const override {
      return 0;
    }

    int Release() const override {
      return 0;
    }
  };

  class SSDO : public webrtc::SetSessionDescriptionObserver {
   public:
    SSDO() {}

    void OnSuccess() override {
      std::cout << "SetSessionDescriptionObserver::OnSuccess" << std::endl;
    }

    void OnFailure(const std::string& error) override {
      std::cout << "SetSessionDescriptionObserver::OnFailure" << std::endl << error << std::endl;
    }

    int AddRef() const override {
      return 0;
    }

    int Release() const override {
      return 0;
    }
  };

  PCO  pco;
  DCO  dco;
  CSDO csdo;
  SSDO ssdo;

  Connection()
      : pco(*this)
      , dco()
      , csdo(*this)
      , ssdo()
  {}
};

rtc::Thread* thread;
rtc::scoped_refptr<webrtc::PeerConnectionFactoryInterface> peer_connection_factory;
webrtc::PeerConnectionInterface::RTCConfiguration configuration;
Connection connection;
rtc::PhysicalSocketServer socket_server;

void thread_entry() {
  peer_connection_factory = webrtc::CreatePeerConnectionFactory();
  if (peer_connection_factory.get() == nullptr) {
    std::cout << "Error on CreatePeerConnectionFactory." << std::endl;
    return;
  }

  // We use public google stun server
  webrtc::PeerConnectionInterface::IceServer ice_server;
  ice_server.uri = "stun:stun.l.google.com:19302";
  configuration.servers.push_back(ice_server);

  thread = rtc::Thread::Current();

  {
      rtc::SocketServerScope ss(&socket_server);
      thread->Run();
  }

  peer_connection_factory = nullptr;
}

class MyRunnable : public rtc::Runnable {
public:
    virtual void Run(rtc::Thread*) override {
        thread_entry();
    }
};

int main() {
    rtc::InitializeSSL();

    rtc::Thread thread;
    MyRunnable mr;
    thread.Start(&mr);

    sleep(1); // fake delay for this thread

    connection.peer_connection = peer_connection_factory->CreatePeerConnection(configuration, nullptr, nullptr, &connection.pco);
    if (connection.peer_connection.get() == nullptr) {
        peer_connection_factory = nullptr;
        std::cerr << "Error on CreatePeerConnection." << std::endl;
        return 0;
    }

    connection.sdp_type = "Offer";
    connection.peer_connection->CreateOffer(&connection.csdo, nullptr);

    sleep(1); // fake delay for this thread

    connection.peer_connection = nullptr;

    thread.Stop();
    rtc::CleanupSSL();

    return 0;
}
