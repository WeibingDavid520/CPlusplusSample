/**
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

#include "policy_profile_observer_impl.h"

#include <future>

using std::exception_ptr;
using std::move;
using std::promise;
using std::shared_ptr;
using std::static_pointer_cast;
using std::string;
using std::vector;

namespace sample {
namespace upe {

PolicyProfileObserverImpl::PolicyProfileObserverImpl(std::function<void(const string&)>&& policyChangedHandler)
    : mPolicyChangedHandler(move(policyChangedHandler)) {}

void PolicyProfileObserverImpl::OnLoadSuccess(const shared_ptr<mip::PolicyProfile>& profile, const shared_ptr<void>& context) {
  auto loadPromise = static_pointer_cast<promise<shared_ptr<mip::PolicyProfile>>>(context);
  loadPromise->set_value(profile);
}

void PolicyProfileObserverImpl::OnLoadFailure(const exception_ptr& error, const shared_ptr<void>& context) {
  auto loadPromise = static_pointer_cast<promise<shared_ptr<mip::PolicyProfile>>>(context);
  loadPromise->set_exception(error);
}

void PolicyProfileObserverImpl::OnListEnginesSuccess(const vector<string>& engineIds, const shared_ptr<void>& context) {
  auto listEnginesPromise = static_pointer_cast<promise<vector<string>>>(context);
  listEnginesPromise->set_value(engineIds);
}

void PolicyProfileObserverImpl::OnListEnginesFailure(const exception_ptr& error, const shared_ptr<void>& context) {
  auto listEnginesPromise = static_pointer_cast<promise<vector<string>>>(context);
  listEnginesPromise->set_exception(error);
}

void PolicyProfileObserverImpl::OnUnloadEngineSuccess(const shared_ptr<void>& context) {
  auto unloadEnginePromise = static_pointer_cast<promise<void>>(context);
  unloadEnginePromise->set_value();
}

void PolicyProfileObserverImpl::OnUnloadEngineFailure(const exception_ptr& error, const shared_ptr<void>& context) {
  auto unloadEnginePromise = static_pointer_cast<promise<void>>(context);
  unloadEnginePromise->set_exception(error);
}

void PolicyProfileObserverImpl::OnAddEngineSuccess(
    const shared_ptr<mip::PolicyEngine>& engine,
    const shared_ptr<void>& context) {
  auto addEnginePromise = static_pointer_cast<promise<shared_ptr<mip::PolicyEngine>>>(context);
  addEnginePromise->set_value(engine);
}

void PolicyProfileObserverImpl::OnAddEngineFailure(const exception_ptr& error, const shared_ptr<void>& context) {
  auto addEnginePromise = static_pointer_cast<promise<shared_ptr<mip::PolicyEngine>>>(context);
  addEnginePromise->set_exception(error);
}

void PolicyProfileObserverImpl::OnDeleteEngineSuccess(const shared_ptr<void>& context) {
  auto deleteEnginePromise = static_pointer_cast<promise<void>>(context);
  deleteEnginePromise->set_value();
}

void PolicyProfileObserverImpl::OnDeleteEngineFailure(const exception_ptr& error, const shared_ptr<void>& context) {
  auto deleteEnginePromise = static_pointer_cast<promise<void>>(context);
  deleteEnginePromise->set_exception(error);
}

void PolicyProfileObserverImpl::OnPolicyChanged(const string& engineId) {
  mPolicyChangedHandler(engineId);
}

} // namespace sample
} // namespace upe