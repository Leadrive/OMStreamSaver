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

rtc::scoped_refptr<webrtc::PeerConnectionInterface>
CreatePeerConnection(bool dtls, rtc::scoped_refptr<webrtc::PeerConnectionFactoryInterface> factory) {
  webrtc::PeerConnectionInterface::RTCConfiguration config;
  webrtc::PeerConnectionInterface::IceServer server;
  server.uri = GetPeerConnectionString();
  config.servers.push_back(server);

  webrtc::FakeConstraints constraints;
  if (dtls) {
    constraints.AddOptional(webrtc::MediaConstraintsInterface::kEnableDtlsSrtp, "true");
  } else {
    constraints.AddOptional(webrtc::MediaConstraintsInterface::kEnableDtlsSrtp, "false");
  }

  return factory->CreatePeerConnection(config, &constraints, nullptr, nullptr, nullptr);
}

int main(int argc, char* argv[]) {
  rtc::scoped_refptr<webrtc::PeerConnectionFactoryInterface> factory  = webrtc::CreatePeerConnectionFactory();
  auto peerConnection = CreatePeerConnection(false, factory);
  return 0;
}
