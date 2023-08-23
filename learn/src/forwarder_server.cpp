/**
  ******************************************************************************
  * @file           : forwarder_server.cpp
  * @author         : KiritoU2
  * @brief          : None
  * @attention      : None
  * @date           : 2023/8/21
  ******************************************************************************
  */

#include <iostream>
#include "workflow/WFTaskFactory.h"
#include "workflow/WFHttpServer.h"

int main() {
    WFHttpServer  server([](WFHttpTask *task){
       std::string url("https://gitee.com");
       std::string requested_url = task->get_req()->get_request_uri();
       url.append(requested_url);

       WFHttpTask *client_task = WFTaskFactory::create_http_task(url, 3, 2, [](WFHttpTask *task){
           const void *body = nullptr;
           size_t size;

           if (task->get_resp()->get_parsed_body(&body, &size)) {
               task->get_resp()->append_output_body_nocopy(body, size);
           }

           SeriesWork *series = series_of(task);
           WFHttpTask *server_task = (WFHttpTask*)series->get_context();
           *server_task->get_resp() = std::move(*task->get_resp());
       });

       *client_task->get_req() = std::move(*(task->get_req()));
       client_task->get_req()->set_header_pair("Host", "gitee.com");

      SeriesWork *series = series_of(task);
       series->set_context(task);
       series->push_back(client_task);
    });

    if (server.start(8888) == 0) {
        std::cin.get();
        server.stop();
    }
    return 0;
}
