/**
  ******************************************************************************
  * @file           : http_server_timer_delay.cpp
  * @author         : KiritoU2
  * @brief          : 延迟回复的http服务器
  * @attention      : 本程序是延迟回复的http服务器
  *                   和普通的服务器相比，只是在回调函数中多了一个timer_task
  *                   服务器会保证所有的series中的task都会执行完毕后，才会回复客户端
  * @date           : 2023/8/21
  ******************************************************************************
  */

#include <iostream>

#include "workflow/WFTaskFactory.h"
#include "workflow/WFHttpServer.h"

int main() {
    WFHttpServer server([](WFHttpTask *task) {
        task->get_resp()->append_output_body("<html>Hello World!</html>");

      series_of(task)->push_back(WFTaskFactory::create_timer_task(1000000, [](WFTimerTask *task){
          std::cout << "wait 1 second...\n";
      }));
    });

    if (server.start(8888) == 0) {
        std::cin.get();
        server.stop();
    }

    return 0;
}