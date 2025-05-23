# **********************************************************
# Copyright (c) 2021-2025 Google, Inc.    All rights reserved.
# **********************************************************

# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# * Redistributions of source code must retain the above copyright notice,
#   this list of conditions and the following disclaimer.
#
# * Redistributions in binary form must reproduce the above copyright notice,
#   this list of conditions and the following disclaimer in the documentation
#   and/or other materials provided with the distribution.
#
# * Neither the name of Google, Inc. nor the names of its contributors may be
#   used to endorse or promote products derived from this software without
#   specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL GOOGLE, INC. OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
# OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
# DAMAGE.

cmake_minimum_required(VERSION 3.14)

include(../../make/policies.cmake NO_POLICY_SCOPE)

# DynamoRIO Callstack Walker

# We try to avoid libc to shrink the size on Windows.
set(DynamoRIO_USE_LIBC OFF)

set(srcs drcallstack.c)

set(srcs_static ${srcs})

if (WIN32)
  set(srcs ${srcs} ${PROJECT_SOURCE_DIR}/core/win32/resources.rc)
  # i#1491#2: VS generators fail if static lib has resources
endif ()

add_library(drcallstack SHARED ${srcs})
set(PREFERRED_BASE 0x79800000)
configure_extension(drcallstack OFF OFF)
target_link_libraries(drcallstack unwind)

add_library(drcallstack_static STATIC ${srcs_static})
configure_extension(drcallstack_static ON OFF)
target_link_libraries(drcallstack_static unwind)

add_library(drcallstack_drstatic STATIC ${srcs_static})
configure_extension(drcallstack_drstatic ON ON)
target_link_libraries(drcallstack_drstatic unwind)

install_ext_header(drcallstack.h)
