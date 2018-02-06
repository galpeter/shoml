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

/*
 * This header describes the contract between the IoT.js dynload module
 * and the loadable shared objects (modules)
 */
#ifndef IOTJS_SOMODULE_H
#define IOTJS_SOMODULE_H


#include "jerryscript.h"

/* Function pointer to pass the Module initializer function around */
typedef jerry_value_t (*InitFunc)(void);

struct iotjs_module {
  uint32_t version; /* module version info */
  char name[255]; /* module name */
  InitFunc initializer; /* module initializer function */
};

/* Module entry point function */
typedef struct iotjs_module (*iotjsModuleFunc)(void);

#endif /* IOTJS_SOMODULE_H */
