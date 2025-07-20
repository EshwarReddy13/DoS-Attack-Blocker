#include "webserver.h"

WebServer::WebServer() : busy(false), time_remaining(0), current_request(nullptr) {}

bool WebServer::isBusy() const {
    return busy;
}

void WebServer::assignRequest(Request* req, int current_time) {
    if (!busy) {
        current_request = req;
        time_remaining = req->process_time;
        req->assigned_time = current_time;
        busy = true;
    }
}

void WebServer::processCycle() {
    if (busy && time_remaining > 0) {
        time_remaining--;
        if (time_remaining == 0) {
            busy = false;
            current_request->processed = true;
        }
    }
}

bool WebServer::isRequestDone() const {
    return !busy && current_request != nullptr && current_request->processed;
}

Request* WebServer::finishRequest() {
    Request* finished = current_request;
    current_request = nullptr;
    // Reset the processed flag in case the request object is reused
    if (finished) {
        finished->processed = false;
    }
    return finished;
}
