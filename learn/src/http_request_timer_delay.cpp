#include <stdio.h>
#include "workflow/WFTaskFactory.h"

void timer_callback(WFTimerTask *timer)
{
    printf("Finished. Press enter to end.\n"); 
}

void callback(WFHttpTask *task)
{
    const void *body;
    size_t size;

    if (task->get_resp()->get_parsed_body(&body, &size))
        fwrite(body, 1, size, stdout);

    WFTimerTask *timer = WFTaskFactory::create_timer_task(1000000, timer_callback);
    series_of(task)->push_back(timer);
    printf("\nWait for 1 second...\n");
}

int main(void)
{
    WFHttpTask *task = WFTaskFactory::create_http_task("http://gitee.com/", 3, 2, callback);
    task->start();
    getchar();
    return 0;
}
