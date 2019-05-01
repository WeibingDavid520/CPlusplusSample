/*
 *
 * Copyright (c) Microsoft Corporation.
 * All rights reserved.
 *
 * This code is licensed under the MIT License.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files(the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions :
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */
/**
 * @brief A file containing initialization/uninitialization utilities
 * 
 * @file mip_init.h
 */

#ifndef API_MIP_MIP_INIT_H_
#define API_MIP_MIP_INIT_H_

#include "mip_export.h"

namespace mip {

/**
 * @brief Releases all resources (threads, etc) before shutdown
 * 
 * @note This function must be called exactly once prior to process termination. It provides MIP the opportunity to
 *       uninitialize itself in a moment where its dependent libraries are still guaranteed to be loaded and thread
 *       joining is still possible. Applications must release references to all MIP objects (for example, Profiles,
 *       Engines, Handlers) before calling this function.
 * 
 * @note If this function is not called, MIP will be naturally unloaded as part of standard process teardown. On some
 *       platforms, this may result in deadlock (for example, threads cannot be joined on win32 in response to process 
 *       teardown) or crashes (for example, the DLL unload order for delay-loaded libraries on win32 is not controlled
 *       by MIP, so its dependent libraries may have been unloaded by the time MIP shutdown code executes, resulting in
 *       invalid read failures).
 */
MIP_API void __CDECL ReleaseAllResources();

}  // namespace mip

#endif  // API_MIP_MIP_INIT_H_