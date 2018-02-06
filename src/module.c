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

#include "iotjs_def.h"

#include <dlfcn.h>

#include "include/iotjs_somodule.h"

static const jerry_object_native_info_t native_module_type_info =
{
  .free_cb = NULL
};

/**
 * Helper function to set properties on objects.
 */
static void
tool_set_property(jerry_value_t obj, const char* key, jerry_value_t value)
{
  jerry_value_t jkey = jerry_create_string((const jerry_char_t*)key);
  jerry_value_t set_ok = jerry_set_property(obj, jkey, value);
  jerry_release_value(set_ok);
  jerry_release_value(jkey);
}

/**
 * Handler function to invoke the loaded shared object's
 * initializer.
 */
static jerry_value_t
module_init_wrapper (const jerry_value_t func_value, /**< function object */
                     const jerry_value_t this_value, /**< this arg */
                     const jerry_value_t *args_p, /**< function arguments */
                     const jerry_length_t args_cnt) /**< number of function arguments */
{
  void *native_p;
  const jerry_object_native_info_t *type_p;
  bool has_p = jerry_get_object_native_pointer (func_value, &native_p, &type_p);

  if (!(has_p && type_p == &native_module_type_info))
  {
    static char *error_msg = "Incorrect init call";
    return jerry_create_error(JERRY_ERROR_TYPE,
                              (const jerry_char_t *)error_msg);
  }

  InitFunc initializer = (InitFunc)native_p;
  return initializer();
}


/**
 * Native js function to load IoT.js shared objects.
 *
 * What are we doing here to load a shared object?
 *
 * 1) Check if there is at least one argument.
 *    If not return error.
 * 2) Check if the first argument is a string.
 *    If not return error.
 * 3) Try to dlopen the given argument as a shared object path.
 *    dlopen semantics are applied here.
 * 4) Check if there is a valid pointer returned by the dlopen.
 *    That is we have found the shared object.
 *    If not return error.
 * 5) Get the 'iotjsModule' symbol.
 * 6) If there is no such symbol the shared object is not a module
 *    thus return error.
 * 7) Convert the symbol to a function pointer and invoke.
 *    This returns the following module information (iotjs_somodule.h):
 *     version: module version information
 *     name: module name
 *     initializer: module initializer function (same as the built-in module initializers)
 * 8) Create an object and set the previous information as properties.
 * 9) Return this object to the js module (js/module.js)
 */
static jerry_value_t
loadso_method (const jerry_value_t func_value, /**< function object */
               const jerry_value_t this_value, /**< this arg */
               const jerry_value_t *args_p, /**< function arguments */
               const jerry_length_t args_cnt) /**< number of function arguments */
{
  if (args_cnt < 1)
  {
    static char *error_msg = "Incorrect parameter count";
    return jerry_create_error(JERRY_ERROR_TYPE,
                              (const jerry_char_t *)error_msg);
  }

  if (!jerry_value_is_string(args_p[0]))
  {
    static char *error_msg = "Incorrect parameter type";
    return jerry_create_error(JERRY_ERROR_TYPE,
                              (const jerry_char_t *)error_msg);
  }

  jerry_char_t name_str[255];
  jerry_size_t size = jerry_string_to_char_buffer(args_p[0], name_str, 254);
  name_str[size] = '\0';

  void *lib = dlopen((const char*)name_str, RTLD_NOW);

  if (lib == NULL)
  {
    static char *error_msg = "No such library";
    return jerry_create_error(JERRY_ERROR_TYPE,
                              (const jerry_char_t *)error_msg);
  }

  void *func = dlsym(lib, "iotjsModule");
  if (func == NULL)
  {
    dlclose(lib);
    static char *error_msg = "No module entrypoint";
    return jerry_create_error(JERRY_ERROR_TYPE,
                              (const jerry_char_t *)error_msg);

  }

  // Request module information from the shared object
  // TODO: add api versioning?
  iotjsModuleFunc moduleFunc = (iotjsModuleFunc)func;
  struct iotjs_module module = moduleFunc();

  printf("Loaded module: %s for v. %d\n", module.name, module.version);
  // TODO: module caching?

  // Advertise the 'init', 'name', and 'version' properties
  jerry_value_t module_object = jerry_create_object();
  {
    jerry_value_t jinit_wrapper = jerry_create_external_function(module_init_wrapper);
    jerry_set_object_native_pointer(jinit_wrapper, module.initializer, &native_module_type_info);

    tool_set_property(module_object, "init", jinit_wrapper);

    jerry_release_value(jinit_wrapper);
  }
  {
    jerry_value_t jname = jerry_create_string((const jerry_char_t*)module.name);
    tool_set_property(module_object, "name", jname);
    jerry_release_value(jname);
  }
  {
    jerry_value_t jversion = jerry_create_number(module.version);
    tool_set_property(module_object, "version", jversion);
    jerry_release_value(jversion);
  }

  // TODO: how to expose JS code parts?
  return module_object;
}

/**
 * Dynamic loader module init method called by IoT.js
 */
jerry_value_t InitSHOML() {
  jerry_value_t mymodule = jerry_create_object();
  iotjs_jval_set_method(mymodule, "loadso", loadso_method);
  return mymodule;
}
