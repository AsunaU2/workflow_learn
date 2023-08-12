#include <stdio.h>
#include "workflow/WFTaskFactory.h"

void callback(WFHttpTask *task)
{
    const void *body;
    size_t size;

    if (task->get_resp()->get_parsed_body(&body, &size))
        fwrite(body, 1, size, stdout);

    printf("\nFinished. Press enter to end.\n"); 
}

int main(void)
{
    WFHttpTask *task = WFTaskFactory::create_http_task("http://gitee.com/", 3, 2, callback);
    task->start();
    getchar();
    return 0;
}
