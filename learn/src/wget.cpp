/**
 ******************************************************************************
 * @file           : wget.cpp
 * @author         : KiritoU2
 * @brief          : None
 * @attention      : None
 * @date           : 2023/9/4
 ******************************************************************************
 */

#include <csignal>
#include <iostream>
#include <string>

#include "workflow/HttpUtil.h"
#include "workflow/WFFacilities.h"
#include "workflow/WFTaskFactory.h"

static WFFacilities::WaitGroup wait_group(1);

void sig_handler(int) {
    wait_group.done();
}

void wget_callback(WFHttpTask* task) {
    protocol::HttpRequest* req = task->get_req();
    protocol::HttpResponse* resp = task->get_resp();

    int state = task->get_state();
    int error = task->get_error();

    switch (state) {
        case WFT_STATE_SYS_ERROR: {
            std::cout << "system_error: " << error << "\n";
        } break;
        case WFT_STATE_DNS_ERROR: {
            std::cout << "DNS error: " << error << "\n";
        } break;
        case WFT_STATE_SSL_ERROR: {
            std::cout << "SSL error: " << error << "\n";
        } break;
        case WFT_STATE_TASK_ERROR: {
            std::cout << "Task error: " << error << "\n";
        } break;
        case WFT_STATE_SUCCESS: {
        } break;
        default: {
            std::cout << "Other error: " << error << "\n";
        }
    }

    if (state != WFT_STATE_SUCCESS) {
        std::cout << "Failed. Press Ctrl-C to exit.\n";
        return;
    }

    std::string name;
    std::string value;

    std::cout << "request method: " << req->get_method() << "\n";
    std::cout << "request http version: " << req->get_http_version() << "\n";
    std::cout << "request uri: " << req->get_request_uri() << "\n";

    protocol::HttpHeaderCursor req_cursor(req);
    while (req_cursor.next(name, value)) {
        std::cout << "request name: " << name << "\n";
        std::cout << "request value: " << value << "\n";
    }

    protocol::HttpHeaderCursor resp_cursor(resp);
    while (resp_cursor.next(name, value)) {
        std::cout << "responds http version: " << resp->get_http_version() << "\n";
        std::cout << "responds status code: " << resp->get_status_code() << "\n";
        std::cout << "responds reason parse: " << resp->get_reason_phrase() << "\n";
    }

    const void* body = nullptr;
    size_t body_len = 0;

    resp->get_parsed_body(&body, &body_len);
    std::cout << std::string(static_cast<const char*>(body)) << "\n";

    std::cout << "success. Press Ctrl-C to exit.\n";
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "USAGE: " << argv[0] << " <http URL>\n";
        exit(1);
    }

    signal(SIGINT, sig_handler);
    std::string url = argv[1];

    if (url.substr(0, 7) != "http://" && url.substr(0, 8) != "https://") {
        url = "http://" + url;
    }

    WFHttpTask* task = WFTaskFactory::create_http_task(url, 3, 2, wget_callback);
    protocol::HttpRequest* req = task->get_req();
    req->add_header_pair("Accept", "*/*");
    req->add_header_pair("User-Agent", "Wget/1.14 (linux-gnu)");
    req->add_header_pair("Connection", "close");
    task->start();

    task->start();
    wait_group.wait();
    return 0;
}