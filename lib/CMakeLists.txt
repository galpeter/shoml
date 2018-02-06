# Copyright 2018-present Samsung Electronics Co., Ltd. and other contributors
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

#
# This is a standalone (almost) shared libray which
# only requires the iotjs and jerry header file(s).
#
# Currently it is integrated to be built during iotjs build
# and the created shared object will be placed into the
# build output directory's 'lib' sub-dir.
#
set(NAME iotjs_dynmodule)

add_library(${NAME} SHARED
    loadable_module.c)
target_include_directories(${NAME}
    PRIVATE ${IOTJS_SOURCE_DIR} ${JERRY_INCLUDE_DIR} ../)
set_target_properties(${NAME} PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
