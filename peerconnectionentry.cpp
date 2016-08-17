#include "peerconnectionentry.hpp"
#include <iostream>

PeerConnectionEntry::~PeerConnectionEntry() {
    peerConnectionFactory = nullptr;
}

void PeerConnectionEntry::Run(rtc::Thread*) {
    peerConnectionFactory = webrtc::CreatePeerConnectionFactory();
    if (peerConnectionFactory.get() == nullptr) {
        std::cerr << "Error on CreatePeerConnectionFactory.\n";
        return;
    }

    // We use public google stun server
    webrtc::PeerConnectionInterface::IceServer ice_server;
    ice_server.uri = "stun:stun.l.google.com:19302";
    configuration.servers.push_back(ice_server);

    // open c++ for setting socket for the thread
    {
        rtc::SocketServerScope serverScope(&serverSocket);
    }

    peerConnectionFactory = nullptr;
}

sPtr<webrtc::PeerConnectionInterface> PeerConnectionEntry::createPeerConnection(webrtc::PeerConnectionObserver* observer) {
    auto peerConneciton = peerConnectionFactory->CreatePeerConnection(configuration, nullptr, nullptr, observer);

    if (peerConneciton.get() == nullptr) {
        peerConnectionFactory = nullptr;
        std::cerr << "Error on CreatePeerConnection\n";
        return;
    }

    return peerConneciton;
}
