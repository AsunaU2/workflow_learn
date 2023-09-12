/**
 ******************************************************************************
 * @file           : forwarder_server.cpp
 * @author         : KiritoU2
 * @brief          : 转发服务器
 *                   负责将客户端的请求转发给真正的目的服务器，将获取的回复再返回给客户端
 * @attention      : 对于brief中的描述，所以要做到以下几点：
 *                   1. 起一个服务器，这个服务器就是转发服务器
 *                   2. 该服务器接收到外部客户端请求以后，需要再起一个客户端，向真正的服务器转发请求
 *                   3. 该服务器需要将真正的服务器的回复数据再回复给访问自己的客户端
 * @date           : 2023/8/21
 ******************************************************************************
 */

#include <iostream>
#include "workflow/WFHttpServer.h"
#include "workflow/WFTaskFactory.h"

int main() {
    // 这个就是转发服务器了，浏览器等其他客户端会向它发起请求
    WFHttpServer server([](WFHttpTask* task) {
        // 需要转发的uri
        std::string url("https://gitee.com");

        // 获取客户端发来的url地址
        std::string requested_url = task->get_req()->get_request_uri();

        // 组装一下得到目的服务器的地址
        url.append(requested_url);

        // 起一个客户端向最终目的服务器进行进行请求，注意，这一步是一个异步操作
        WFHttpTask* client_task = WFTaskFactory::create_http_task(url, 3, 2, [](WFHttpTask* task) {
            const void* body = nullptr;
            size_t size;
            // 客户端获取到最终目的服务器地址的数据，将数据添加到回复数据里
            if (task->get_resp()->get_parsed_body(&body, &size)) {
                task->get_resp()->append_output_body_nocopy(body, size);
            }

            // 获取客户端的串行work指针
            // 目的是为了获取到共享上下文，在这个例子中，该上下文就是转发服务器本身
            // 为何需要转发服务器的指针，是因为需要将客户端收到的回复数据给到转发服务器，转发服务器负责将数据返还给浏览器
            SeriesWork* series = series_of(task);
            auto* server_task = (WFHttpTask*)series->get_context();
            *server_task->get_resp() = std::move(*task->get_resp());
        });

        // 转发服务器将浏览器的请求数据原封不动的给到内部的请求客户端
        *client_task->get_req() = std::move(*(task->get_req()));
        client_task->get_req()->set_header_pair("Host", "gitee.com");

        // 转发服务器的串行work
        // 先设置上下文，此处上下文就是转发服务器，目的是让内部的请求客户端可以获得这个服务器，可以把自己收到的回复数据给它
        // 由于请求客户端被push_back到了转发服务器的串行任务队列里，因此它们可以共享上下文
        // 由于是串口任务，因此服务器会在所有的串行任务执行完毕后再进行回复
        SeriesWork* series = series_of(task);
        series->set_context(task);
        series->push_back(client_task);
    });

    if (server.start(8888) == 0) {
        std::cin.get();
        server.stop();
    }
    return 0;
}
