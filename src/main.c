#include <stdio.h>
#include "quickjs/quickjs-libc.h"
#include "uv.h"

static void onTimerTick(uv_timer_t *handle) {
  printf("timer tick\n");
}

static int eval_buf(JSContext *ctx, const void *buf, int buf_len,
                    const char *filename, int eval_flags)
{
    JSValue val;
    int ret;

    if ((eval_flags & JS_EVAL_TYPE_MASK) == JS_EVAL_TYPE_MODULE) {
        /* for the modules, we compile then run to be able to set
           import.meta */
        val = JS_Eval(ctx, buf, buf_len, filename,
                      eval_flags | JS_EVAL_FLAG_COMPILE_ONLY);
        if (!JS_IsException(val)) {
            js_module_set_import_meta(ctx, val, TRUE, TRUE);
            val = JS_EvalFunction(ctx, val);
        }
    } else {
        val = JS_Eval(ctx, buf, buf_len, filename, eval_flags);
    }
    if (JS_IsException(val)) {
        js_std_dump_error(ctx);
        ret = -1;
    } else {
        ret = 0;
    }
    JS_FreeValue(ctx, val);
    return ret;
}

int main(int argc, char **argv)
{
  JSRuntime *rt;
  JSContext *ctx;
  rt = JS_NewRuntime();
  ctx = JS_NewContextRaw(rt);
  JS_SetModuleLoaderFunc(rt, NULL, js_module_loader, NULL);
  JS_AddIntrinsicBaseObjects(ctx);
  JS_AddIntrinsicDate(ctx);
  JS_AddIntrinsicEval(ctx);
  JS_AddIntrinsicStringNormalize(ctx);
  JS_AddIntrinsicRegExp(ctx);
  JS_AddIntrinsicJSON(ctx);
  JS_AddIntrinsicProxy(ctx);
  JS_AddIntrinsicMapSet(ctx);
  JS_AddIntrinsicTypedArrays(ctx);
  JS_AddIntrinsicPromise(ctx);
  JS_AddIntrinsicBigInt(ctx);
  js_std_add_helpers(ctx, argc, argv);

  {
    extern JSModuleDef *js_init_module_fib(JSContext *ctx, const char *name);
    js_init_module_fib(ctx, "fib.so");
  }

  uint8_t *buf;
  size_t buf_len;
  const char *filename = "../src/test.js";
  buf = js_load_file(ctx, &buf_len, filename);
  eval_buf(ctx, buf, buf_len, filename, JS_EVAL_TYPE_MODULE);
  // js_rt_loop(ctx);

  JS_FreeContext(ctx);
  JS_FreeRuntime(rt);

  uv_loop_t *loop = uv_default_loop();
  uv_timer_t timerHandle;
  uv_timer_init(loop, &timerHandle);
  uv_timer_start(&timerHandle, onTimerTick, 0, 1000);
  uv_run(loop, UV_RUN_DEFAULT);

  return 0;
}
