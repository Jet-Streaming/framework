#!/usr/bin/env bash

gyp all.gyp -Isrc/core/uv/common.gypi --depth=. -Dhost_arch=x64 -Dtarget_arch=x64 -Duv_library=static_library -Dcomponent=static_library -Dvisibility=no_hidden --generator-output=build/linux64