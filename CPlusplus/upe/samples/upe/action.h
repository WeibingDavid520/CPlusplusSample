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

#ifndef SAMPLES_UPE_ACTION_H_
#define SAMPLES_UPE_ACTION_H_

#include <memory>
#include <string>
#include <unordered_map>

#include "mip/common_types.h"
#include "mip/upe/policy_engine.h"
#include "mip/upe/policy_profile.h"

#include "auth_delegate_impl.h"
#include "execution_state_impl.h"
#include "policy_profile_observer_impl.h"

namespace sample {
namespace upe {

enum class AuthenticationType {
  None,
  Token,
  UserCredentials,
};

enum class PolicyType {
  Server,
  File, // Note that the sample 'policy.xml' that ships alongside the app contains labels with non-ASCII characters
};

struct AuthenticationOptions {
  AuthenticationType type = AuthenticationType::None;
  std::string username;
  std::string password;
  std::string clientId;
  std::string token;
};

struct ProfileOptions {
  bool useStorageCache = false;
  bool simulatePolicyChange = false;
  std::string engineId;
  PolicyType policyType;
  std::string policyFile;
  mip::ApplicationInfo appInfo;
};

class Action {
public:
  Action(
      const AuthenticationOptions& authOptions,
      const ProfileOptions& profileOptions,
      const std::string& locale,
      const std::string& workingDirectory, 
      bool loadSensitivityTypes);
  ~Action();

  void ListEngines();
  void ListLabels();
  void ListSensitivityTypes();
  void ShowDefaultLabel();
  void ShowLabel(const ExecutionStateOptions& options);
  void ShowPolicyData();
  void ComputeActions(const ExecutionStateOptions& options);

private:
  void EnsurePolicyEngine();
  std::shared_ptr<mip::PolicyEngine> CreateNewPolicyEngine();
  std::shared_ptr<mip::PolicyEngine> LoadExistingPolicyEngine(const std::string& engineId);
  std::vector<std::pair<std::string, std::string>> GetCustomPolicySettings();
  void OnPolicyChanged(const std::string& engineId);
  void SimulatePolicyChange(const std::shared_ptr<mip::PolicyEngine>& engine);

  AuthenticationOptions mAuthOptions;
  ProfileOptions mProfileOptions;
  std::shared_ptr<sample::auth::AuthDelegateImpl> mAuthDelegate;
  std::shared_ptr<PolicyProfileObserverImpl> mProfileObserver;
  std::shared_ptr<mip::PolicyProfile> mProfile;
  std::shared_ptr<mip::PolicyEngine> mEngine;
  std::string mLocale;
  bool mLoadSensitivityTypes;
};

} // namespace sample
} // namespace upe

#endif // SAMPLES_UPE_ACTION_H_
