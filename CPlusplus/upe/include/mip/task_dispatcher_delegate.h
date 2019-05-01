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
 * @brief A file containing the TaskDispatcherDelegate interface to be used to override MIP async task executor
 * 
 * @file task_dispatcher_delegate.h
 */


#ifndef API_MIP_TASK_DISPATCHER_DELEGATE_H_
#define API_MIP_TASK_DISPATCHER_DELEGATE_H_

#include <functional>
#include <memory>
#include <string>

namespace mip {

/**
 * @brief A class that defines the interface to the MIP SDK task dispatcher
 */
class TaskDispatcherDelegate {
public:
  /**
   * @brief Execute a task on a background thread
   *
   * @param taskId ID to uniquely identify a task
   * @param task Function to be executed
   */
  virtual void DispatchTask(const std::string& taskId, std::function<void()> task) = 0;

  /**
   * @brief Execute a task on a background thread with the given delay
   *
   * @param taskId ID to uniquely identify a task
   * @param task Function to be executed
   * @param delay Delay (in seconds) before executing task
   */
  virtual void DispatchTask(const std::string& taskId, std::function<void()> task, int64_t delay) = 0;

  /**
   * @brief Immediately execute a task on an independent thread
   *
   * @param taskId ID to uniquely identify a task
   * @param task Function to be executed
   */
  virtual void ExecuteTaskOnIndependentThread(const std::string& taskId, std::function<void()> task) = 0;

  /**
   * @brief Cancel a background task
   *
   * @param taskId ID of task to cancel
   * 
   * @return True if task was successfully cancelled, else false
   */
  virtual bool CancelTask(const std::string& taskId) = 0;

  /**
   * @brief Cancel all background tasks
   */
  virtual void CancelAllTasks() = 0;

  /** @cond DOXYGEN_HIDE */
  virtual ~TaskDispatcherDelegate() {}
protected:
  TaskDispatcherDelegate() {}
   /** @endcond */
};

}  // namespace mip
#endif  // API_MIP_TASK_DISPATCHER_DELEGATE_H_
