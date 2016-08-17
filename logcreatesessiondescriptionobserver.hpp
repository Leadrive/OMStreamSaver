#ifndef LOGCREATESESSIONDESCRIPTIONOBSERVER_H
#define LOGCREATESESSIONDESCRIPTIONOBSERVER_H

#include <iostream>

class LogCreateSessionDescriptionObserver : public webrtc::CreateSessionDescriptionObserver {
public:
    void OnSuccess(webrtc::SessionDescriptionInterface* desc) override {
        std::cerr << "CreateSessionDescriptionObserver::OnSuccess\n";
        // TODO peerconnection->SetLocalDescription(observer, desc);

        std::string sdp;
        desc->ToString(&sdp);
        std::cerr << " SDP:begin\n" << sdp << "\n SDP:end\n";
    }

    void OnFailure(const std::string& error) override {
        std::cerr << "CreateSessionDescriptionObserver::OnFailure " << error << "\n";
    }

    // managing is manual
    int AddRef() const override {
        return 0;
    }

    int Release() const override {
        return 0;
    }
};

#endif // LOGCREATESESSIONDESCRIPTIONOBSERVER_H
