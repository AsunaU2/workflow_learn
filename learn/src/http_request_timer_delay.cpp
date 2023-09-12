/**
  ******************************************************************************
  * @file           : http_request_timer_delay.cpp
  * @author         : KiritoU2
  * @brief          : 发出请求后，先打印响应，但延迟1s后再打印Finished. Press enter to end.
  * @attention      : 分析：本可以在http_task的回调函数中直接调用sleep后再直接打印，但是这样会阻塞住这个工作线程，导致无法处理其他任务
  *                   解决：使用timer_task，将timer_task放入http_task的series中，这样就保证了在http_task的回调函数执行完毕后，才会执行timer_task
  * @date           : 2023/8/16
  ******************************************************************************
  */
#include <string>
#include <iostream>
#include "workflow/WFTaskFactory.h"

void timer_callback(WFTimerTask *timer) {
    printf("Finished. Press enter to end.\n");
}

int main(void) {
    // 1. 创建http_task: 最多3次跳转，2次重连尝试
    WFHttpTask *task = WFTaskFactory::create_http_task("http://gitee.com/", 3, 2, [](WFHttpTask *task) {
      const void *body = nullptr;
      size_t size = 0;

      if (task->get_resp()->get_parsed_body(&body, &size)) {
          std::string str_body = std::string((const char *)body, size);
          std::cout << str_body << std::endl;
      }

      // 1.1 创建timer_task: 延迟1s
      WFTimerTask *timer = WFTaskFactory::create_timer_task(1000000, [](WFTimerTask *timer) {
        std::cout << "Finished. Press enter to end." << std::endl;
      });

      series_of(task)->push_back(timer);
      printf("\nWait for 1 second...\n");
    });

    // 2. 启动任务
    task->start();

    std::cin.get();
    return 0;
}
