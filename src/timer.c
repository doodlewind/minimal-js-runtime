#include <quickjs/quickjs-libc.h>
#include <uv.h>

#define countof(x) (sizeof(x) / sizeof((x)[0]))

typedef struct {
    JSContext *ctx;
    uv_timer_t handle;
    int interval;
    JSValue obj;
    JSValue func;
    int argc;
    JSValue argv[];
} UVTimer;

static JSClassID uv_timer_class_id;

static void clearTimer(UVTimer *th) {
    JSContext *ctx = th->ctx;

    JS_FreeValue(ctx, th->func);
    th->func = JS_UNDEFINED;

    for (int i = 0; i < th->argc; i++) {
        JS_FreeValue(ctx, th->argv[i]);
        th->argv[i] = JS_UNDEFINED;
    }
    th->argc = 0;

    JSValue obj = th->obj;
    th->obj = JS_UNDEFINED;
    JS_FreeValue(ctx, obj);
}

static void callTimer(UVTimer *th) {
    JSContext *ctx = th->ctx;
    JSValue ret, func1;
    /* 'func' might be destroyed when calling itself (if it frees the handler), so must take extra care */
    func1 = JS_DupValue(ctx, th->func);
    ret = JS_Call(ctx, func1, JS_UNDEFINED, th->argc, (JSValueConst *) th->argv);
    JS_FreeValue(ctx, func1);
    // FIXME
    // if (JS_IsException(ret))
    //     dump_error(ctx);
    JS_FreeValue(ctx, ret);
}

static void timerCallback(uv_timer_t *handle) {
    UVTimer *th = handle->data;
    JSContext *ctx = th->ctx;

    JSContext *ctx1;
    int err;

    for (;;) {
        err = JS_ExecutePendingJob(JS_GetRuntime(ctx), &ctx1);
        if (err <= 0) {
            if (err < 0)
                js_std_dump_error(ctx1);
            break;
        }
    }

    callTimer(th);
    if (!th->interval)
        clearTimer(th);
}

static JSValue js_uv_setTimeout(JSContext *ctx, JSValueConst this_val,
                                int argc, JSValueConst *argv) {
    int64_t delay;
    JSValueConst func;
    UVTimer *th;
    JSValue obj;

    func = argv[0];
    if (!JS_IsFunction(ctx, func))
        return JS_ThrowTypeError(ctx, "not a function");
    if (JS_ToInt64(ctx, &delay, argv[1]))
        return JS_EXCEPTION;

    obj = JS_NewObjectClass(ctx, uv_timer_class_id);
    if (JS_IsException(obj))
        return obj;

    int nargs = argc - 2;

    th = calloc(1, sizeof(*th) + nargs * sizeof(JSValue));
    if (!th) {
        JS_FreeValue(ctx, obj);
        return JS_EXCEPTION;
    }

    th->ctx = ctx;
    uv_loop_t *loop = JS_GetContextOpaque(ctx);
    uv_timer_init(loop, &th->handle);
    th->handle.data = th;
    th->interval = 0;
    th->obj = JS_DupValue(ctx, obj);
    th->func = JS_DupValue(ctx, func);
    th->argc = nargs;
    for (int i = 0; i < nargs; i++)
        th->argv[i] = JS_DupValue(ctx, argv[i + 2]);

    uv_timer_start(&th->handle, timerCallback, delay, 0);

    JS_SetOpaque(obj, th);
    return obj;
}

static const JSCFunctionListEntry js_os_funcs[] = {
    JS_CFUNC_DEF("setTimeout", 2, js_uv_setTimeout ),
};

static int js_uv_init(JSContext *ctx, JSModuleDef *m) {        
    return JS_SetModuleExportList(ctx, m, js_os_funcs,
                                  countof(js_os_funcs));
}

JSModuleDef *js_init_module_uv(JSContext *ctx, const char *module_name) {
    JSModuleDef *m;
    m = JS_NewCModule(ctx, module_name, js_uv_init);
    if (!m)
        return NULL;
    JS_AddModuleExportList(ctx, m, js_os_funcs, countof(js_os_funcs));
    return m;
}
