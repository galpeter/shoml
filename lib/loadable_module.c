/* Copyright 2018-present Samsung Electronics Co., Ltd. and other contributors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "jerryscript.h"
#include "iotjs_def.h"

#include "include/iotjs_somodule.h"


jerry_value_t InitDynamic(void) {
    jerry_value_t object = jerry_create_object();
    iotjs_jval_set_property_string_raw(object, "message", "Hello world!");
    iotjs_jval_set_property_string_raw(object, "sharedInfo", "This is from a shared object!");
    return object;
}


struct iotjs_module iotjsModule() {
    static struct iotjs_module module = {
        .version = 1,
        .name = "DynamicModule",
        .initializer = InitDynamic,
    };

    return module;
}
