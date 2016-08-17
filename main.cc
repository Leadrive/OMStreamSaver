/*
 *  Copyright 2012 The WebRTC Project Authors. All rights reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include "webrtc/examples/peerconnection/client/peer_connection_client.h"

#include "webrtc/base/ssladapter.h"
#include "webrtc/base/thread.h"
#include "peerconnectionentry.hpp"
#include "logpeerconnectionobserver.hpp"
#include "logcreatesessiondescriptionobserver.hpp"

#include <iostream>
#include <string>

// trivial RAII wrapper for SSL functions
struct SSL {
    SSL() {
        rtc::InitializeSSL();
    }

    ~SSL() {
        rtc::CleanupSSL();
    }
};

int main() {
    SSL ssl;
    rtc::Thread rtcThread;
    PeerConnectionEntry entry;

    rtcThread.Start(&entry);

    std::string command;
    rtc::scoped_refptr<webrtc::PeerConnectionInterface> peerConnection;
    rtc::scoped_refptr<LogPeerConnectionObserver> logPeerConnectionObserver;
    rtc::scoped_refptr<LogCreateSessionDescriptionObserver> logCreateSessionDescriptionObserver;

    bool stop = false;
    while (!stop) {
        std::getline(std::cin, command);
        if (command == "quit") {
            std::cout <<  "by" << std::endl;
            rtcThread.Quit();
            stop = true;
        }
        else if (command == "init") {
            peerConnection = entry.createPeerConnection(logPeerConnectionObserver.get());
        }
        else if (command == "create offer") {
            peerConnection->CreateOffer(logCreateSessionDescriptionObserver.get(), nullptr);
        }
        else {
            std::cout << "unknown command " << command << std::endl;
        }
    }

    peerConnection = nullptr;
    logPeerConnectionObserver = nullptr;
    logCreateSessionDescriptionObserver = nullptr;

    return 0;
}
