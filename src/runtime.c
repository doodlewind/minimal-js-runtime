
#include "runtime.h"

void js_rt_loop(JSContext *ctx)
{
    JSContext *ctx1;
    int err;

    for(;;) {
        for(;;) {
            err = JS_ExecutePendingJob(JS_GetRuntime(ctx), &ctx1);
            if (err <= 0) {
                if (err < 0) {
                    js_std_dump_error(ctx1);
                }
                break;
            }
        }
    }
}