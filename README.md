# Minimal JS Runtime
A toy JavaScript runtime based on QuickJS and libuv.

## Introduction
This project demonstrates how to build a simplest JavaScript runtime with QuickJS and libuv. Takeaway points include:

* Native `fib` function implementation.
* `setTimeout` implementation.
* Support for running microtasks and macrotasks.

See my Chinese blog post for technical details:

* [From JavaScript Engine to JavaScript Runtime - 1](https://ewind.us/2020/js-engine-to-js-runtime-1/)
* [From JavaScript Engine to JavaScript Runtime - 2](https://ewind.us/2020/js-engine-to-js-runtime-2/)

## Getting Started
Please make sure [CMake](https://cmake.org/), [QuickJS](https://bellard.org/quickjs/) and [libuv](https://libuv.org/) are globally installed.

Build the runtime:

``` bash
cd build
cmake .. && make
```

Start the runtime:

``` bash
./runtime
```

## Misc
To find out how to port the original event loop shipped with QuickJS, checkout commit [fce26e](https://github.com/doodlewind/minimal-js-runtime/commit/fce26ed2641ca9ff5ee3bad7dd1dc76caa679aa8).

## License
MIT
