#ifndef PEERCONNECTIONENTRY_H
#define PEERCONNECTIONENTRY_H

#include "webrtc/api/peerconnectionfactory.h"
#include "webrtc/api/peerconnectioninterface.h"
#include "webrtc/base/physicalsocketserver.h"

class PeerConnectionEntry : public rtc::Runnable
{
public:
    template<class T>
    using sPtr = rtc::scoped_refptr<T>;

public:
    virtual ~PeerConnectionEntry();

    virtual void Run(rtc::Thread*) override;

    sPtr<webrtc::PeerConnectionInterface> createPeerConnection(webrtc::PeerConnectionObserver* observer);

private:
    sPtr<webrtc::PeerConnectionFactoryInterface> peerConnectionFactory;

    rtc::PhysicalSocketServer serverSocket;

    webrtc::PeerConnectionInterface::RTCConfiguration configuration;
};

#endif // PEERCONNECTIONENTRY_H
