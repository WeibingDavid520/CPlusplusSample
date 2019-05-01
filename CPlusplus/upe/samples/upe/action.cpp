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

#include "action.h"

#include "mip/mip_init.h"
#include "mip/upe/action.h"
#include "mip/upe/add_content_footer_action.h"
#include "mip/upe/add_content_header_action.h"
#include "mip/upe/add_watermark_action.h"
#include "mip/upe/custom_action.h"
#include "mip/upe/label.h"
#include "mip/upe/metadata_action.h"
#include "mip/upe/protect_by_template_action.h"
#include "mip/upe/remove_content_footer_action.h"
#include "mip/upe/remove_content_header_action.h"
#include "mip/upe/remove_watermark_action.h"

#include <iostream>
#include <future>

using sample::auth::AuthDelegateImpl;
using std::cout;
using std::endl;
using std::future;
using std::make_shared;
using std::pair;
using std::promise;
using std::runtime_error;
using std::shared_ptr;
using std::string;
using std::unique_ptr;
using std::unordered_map;
using std::vector;

namespace {

string GetContentAlignmentStr(mip::ContentMarkAlignment alignment) {
  switch (alignment) {
    case mip::ContentMarkAlignment::LEFT:
      return "Left";
    case mip::ContentMarkAlignment::RIGHT:
      return "Right";
    case mip::ContentMarkAlignment::CENTER:
      return "Center";
    default:
      throw runtime_error("Unrecognized ContentMarkAlignment");
  }
}

string GetWatermarkLayoutStr(mip::WatermarkLayout layout) {
  switch (layout) {
    case mip::WatermarkLayout::HORIZONTAL:
      return "Horizontal";
    case mip::WatermarkLayout::DIAGONAL:
      return "Diagonal";
    default:
      throw runtime_error("Unrecognized WatermarkLayout");
  }
}

void PrintLabel(const shared_ptr<mip::Label>& label, int indentLevel = 0) {
  string indent(indentLevel * 4, ' ');

  cout << indent << "LABEL:\n" <<
      indent << "  Id: " << label->GetId() << "\n" <<
      indent << "  Name: " << label->GetName() << "\n" <<
      indent << "  Description: " << label->GetDescription() << "\n" <<
      indent << "  IsActive: " << (label->IsActive() ? "true" : "false") << "\n" <<
      indent << "  Color: " << label->GetColor() << "\n" <<
      indent << "  Sensitivity: " << label->GetSensitivity() << "\n" <<
      indent << "  Tooltip: " << label->GetTooltip() << endl;

  shared_ptr<mip::Label> parent = label->GetParent().lock();
  if (nullptr != parent)
    cout << indent << "  Parent Id: " << parent->GetId() << endl;

  if (!label->GetChildren().empty()) {
    cout << indent << "  Children:" << endl;
    for (const shared_ptr<mip::Label>& child : label->GetChildren())
      PrintLabel(child, indentLevel + 1);
  }
}

void PrintSensitivityType(const shared_ptr<mip::SensitivityTypesRulePackage>& type) {
  cout << "SENSITIVITY TYPE:\n" <<
      "  Id: " << type->GetRulePackageId() << "\n" <<
      "  Rule: " << type->GetRulePackage() << endl;
}

void PrintAction(const shared_ptr<mip::Action>& action) {
  cout << "ACTION:\n" << "  Id: " << action->GetId() << endl;

  switch (action->GetType()) {
    case mip::ActionType::ADD_CONTENT_FOOTER: {
      auto derivedAction = static_cast<mip::AddContentFooterAction*>(action.get());
      cout << "  Type: AddContentFooter" << "\n" <<
          "  UIElementName: " << derivedAction->GetUIElementName() << "\n" <<
          "  Text: " << derivedAction->GetText() << "\n" <<
          "  FontName: " << derivedAction->GetFontName() << "\n" <<
          "  FontSize: " << derivedAction->GetFontSize() << "\n" <<
          "  FontColor: " << derivedAction->GetFontColor() << "\n" <<
          "  Alignment: " << GetContentAlignmentStr(derivedAction->GetAlignment()) << "\n" <<
          "  Margin: " << derivedAction->GetMargin() << "\n" << endl;
      break;
    }

    case mip::ActionType::ADD_CONTENT_HEADER: {
      auto derivedAction = static_cast<mip::AddContentHeaderAction*>(action.get());
      cout << "  Type: AddContentHeader" << "\n" <<
          "  UIElementName: " << derivedAction->GetUIElementName() << "\n" <<
          "  Text: " << derivedAction->GetText() << "\n" <<
          "  FontName: " << derivedAction->GetFontName() << "\n" <<
          "  FontSize: " << derivedAction->GetFontSize() << "\n" <<
          "  FontColor: " << derivedAction->GetFontColor() << "\n" <<
          "  Alignment: " << GetContentAlignmentStr(derivedAction->GetAlignment()) << "\n" <<
          "  Margin: " << derivedAction->GetMargin() << "\n" << endl;
      break;
    }

    case mip::ActionType::ADD_WATERMARK: {
      auto derivedAction = static_cast<mip::AddWatermarkAction*>(action.get());
      cout << "  Type: AddWatermarkAction" << "\n" <<
          "  UIElementName: " << derivedAction->GetUIElementName() << "\n" <<
          "  Layout: " << GetWatermarkLayoutStr(derivedAction->GetLayout()) << "\n" <<
          "  Text: " << derivedAction->GetText() << "\n" <<
          "  FontName: " << derivedAction->GetFontName() << "\n" <<
          "  FontSize: " << derivedAction->GetFontSize() << "\n" <<
          "  FontColor: " << derivedAction->GetFontColor() << "\n" << endl;
      break;
    }

    case mip::ActionType::CUSTOM: {
      auto derivedAction = static_cast<mip::CustomAction*>(action.get());
      cout << "  Type: Custom" << "\n";
      if (!derivedAction->GetProperties().empty()) {
        cout << "  Properties:" << "\n";
        for (const pair<string, string>& prop : derivedAction->GetProperties())
          cout << "    '" << prop.first << "' : '" << prop.second << "'\n";
      }
      cout << endl;
      break;
    }

    case mip::ActionType::JUSTIFY: {
      cout << "  Type: Justify" << "\n" << endl;
      break;
    }

    case mip::ActionType::METADATA: {
      auto derivedAction = static_cast<mip::MetadataAction*>(action.get());
      cout << "  Type: Metadata" << "\n";
      if (!derivedAction->GetMetadataToRemove().empty()) {
        cout << "  Remove:" << "\n";
        for (const string& prop : derivedAction->GetMetadataToRemove())
          cout << "    '" << prop << "'\n";
      }
      if (!derivedAction->GetMetadataToAdd().empty()) {
        cout << "  Add:" << "\n";
        for (const pair<string, string>& prop : derivedAction->GetMetadataToAdd())
          cout << "    '" << prop.first << "' : '" << prop.second << "'\n";
      }
      cout << endl;
      break;
    }

    case mip::ActionType::PROTECT_ADHOC: {
      cout << "  Type: ProtectAdHoc" << "\n" << endl;
      break;
    }

    case mip::ActionType::PROTECT_BY_TEMPLATE: {
      auto derivedAction = static_cast<mip::ProtectByTemplateAction*>(action.get());
      cout << "  Type: ProtectByTemplate" << "\n" <<
          "  TemplateId: " << derivedAction->GetTemplateId() << "\n" << endl;
      break;
    }

    case mip::ActionType::PROTECT_DO_NOT_FORWARD: {
      cout << "  Type: ProtectDoNotForward" << "\n" << endl;
      break;
    }

    case mip::ActionType::REMOVE_CONTENT_FOOTER: {
      auto derivedAction = static_cast<mip::RemoveContentFooterAction*>(action.get());
      cout << "  Type: RemoveContentFooterAction" << "\n";
      if (!derivedAction->GetUIElementNames().empty()) {
        cout << "  UIElementNames:\n";
        for (const string& element : derivedAction->GetUIElementNames())
            cout << "    " << element << "\n";
      }
      cout << endl;
      break;
    }

    case mip::ActionType::REMOVE_CONTENT_HEADER: {
      auto derivedAction = static_cast<mip::RemoveContentHeaderAction*>(action.get());
      cout << "  Type: RemoveContentHeaderAction" << "\n";
      if (!derivedAction->GetUIElementNames().empty()) {
        cout << "  UIElementNames:\n";
        for (const string& element : derivedAction->GetUIElementNames())
            cout << "    " << element << "\n";
      }
      cout << endl;
      break;
    }

    case mip::ActionType::REMOVE_PROTECTION: {
      cout << "  Type: RemoveProtection" << "\n" << endl;
      break;
    }

    case mip::ActionType::REMOVE_WATERMARK: {
      auto derivedAction = static_cast<mip::RemoveWatermarkAction*>(action.get());
      cout << "  Type: RemoveWatermarkAction" << "\n";
      if (!derivedAction->GetUIElementNames().empty()) {
        cout << "  UIElementNames:\n";
        for (const string& element : derivedAction->GetUIElementNames())
            cout << "    " << element << "\n";
      }
      cout << endl;
      break;
    }

    default: {
      throw runtime_error("Unrecognized ActionType");
    }
  }
}

} // namespace

namespace sample {
namespace upe {

Action::Action(
    const AuthenticationOptions& authOptions,
    const ProfileOptions& profileOptions,
    const string& locale,
    const string& workingDirectory, 
    bool loadSensitivityTypes)
    : mAuthOptions(authOptions),
      mProfileOptions(profileOptions),
      mLocale(locale),
      mLoadSensitivityTypes(loadSensitivityTypes) {
  // Auth delegate will be used to acquire policy from SCC service when profileOptions.policyType == PolicyType::Server
  mAuthDelegate = make_shared<AuthDelegateImpl>(
      false /*isVerbose*/,
      authOptions.password,
      authOptions.clientId,
      authOptions.token /*sccToken*/,
      "" /*protectionToken*/,
      workingDirectory);

  // Application ID refers to Azure application ID
  // see https://docs.microsoft.com/en-us/azure/azure-resource-manager/resource-group-create-service-principal-portal
  mip::ApplicationInfo appInfo;
  appInfo.applicationId = "00000000-0000-0000-0000-000000000001";
  appInfo.applicationName = "UPE Sample App";
  appInfo.applicationVersion = "1.0.0.0";
  mProfileOptions.appInfo = appInfo;

  mProfileObserver = make_shared<PolicyProfileObserverImpl>([this](const string& engineId) {
      OnPolicyChanged(engineId); });
  
  // Storage path indicates the path under which logs, telemetry, engine database, etc. should be stored. An empty path
  // is interpreted as current working directory.
  string storagePath = "upe_sample_storage";

  // A profile can optionally cache its engines
  mip::PolicyProfile::Settings settings(
      storagePath,
      !profileOptions.useStorageCache /*useInMemoryStorage*/,
      mAuthDelegate,
      mProfileObserver,
      appInfo);

  settings.SetMinimumLogLevel(mip::LogLevel::Trace); // set the minimum log level to trace for easier debugging

  // Create a context to pass to 'PolicyProfile::LoadAsync'. That context will be forwarded to the corresponding
  // PolicyProfile::Observer methods. In this case, we use promises/futures as a simple way to to detect the async
  // operation completions synchronously.
  auto loadPromise = make_shared<promise<shared_ptr<mip::PolicyProfile>>>();
  future<shared_ptr<mip::PolicyProfile>> loadFuture = loadPromise->get_future();

  // A profile should be created and held for the duration of the application lifetime
  mip::PolicyProfile::LoadAsync(settings, loadPromise /*context*/);
  mProfile = loadFuture.get();
}

Action::~Action() {
  // Uninitialize MIP prior to process termination
  mEngine = nullptr;
  mProfile = nullptr;
  mip::ReleaseAllResources();
}

// Lists all engines known to the profile (from the storage cache). Note that if the optional 'useStorageCache' sample
// app flag is not set, this will return empty results.
void Action::ListEngines() {
  // Create a context to pass to 'PolicyProfile::ListEnginesAsync'. That context will be forwarded to the corresponding
  // PolicyProfile::Observer methods. In this case, we use promises/futures as a simple way to to detect the async 
  // operation completions synchronously.
  auto listEnginesPromise = make_shared<promise<vector<string>>>();
  future<vector<string>> listEnginesFuture = listEnginesPromise->get_future();
  mProfile->ListEnginesAsync(listEnginesPromise);
  const vector<string>& engineIds = listEnginesFuture.get();

  if (engineIds.empty()) {
    cout << "NO CACHED ENGINES" << endl;
  } else {
    cout << "CACHED ENGINES:\n";
    for (size_t i = 0; i < engineIds.size(); ++i)
      cout << "  Engine (" << i << "): " << engineIds[i] << "\n";
  }
}

// Creates/loads an engine and prints all labels defined in the policy
void Action::ListLabels() {
  EnsurePolicyEngine();

  if (mProfileOptions.simulatePolicyChange)
    SimulatePolicyChange(mEngine);

  for (const shared_ptr<mip::Label>& label : mEngine->ListSensitivityLabels())
    PrintLabel(label);
}

// Creates/loads an engine and prints all sensitivity types defined in the policy
void Action::ListSensitivityTypes() {
  EnsurePolicyEngine();

  for (const shared_ptr<mip::SensitivityTypesRulePackage>& type : mEngine->ListSensitivityTypes()) {
    PrintSensitivityType(type);
  }
}

// Creates/loads an engine and prints default label defined in the policy
void Action::ShowDefaultLabel() {
  EnsurePolicyEngine();

  if (mProfileOptions.simulatePolicyChange)
    SimulatePolicyChange(mEngine);

  shared_ptr<mip::Label> defaultLabel = mEngine->GetDefaultSensitivityLabel();
  if (nullptr != defaultLabel)
    PrintLabel(defaultLabel);
  else
    cout << "NO DEFAULT LABEL" << endl;
}

// Creates/loads an engine, shows current label based on execution state
void Action::ShowLabel(const ExecutionStateOptions& options) {
  EnsurePolicyEngine();

  if (mProfileOptions.simulatePolicyChange)
    SimulatePolicyChange(mEngine);

  ExecutionStateImpl state(options);

  // Pass in the isAuditDiscoveryEnabled flag to CreatePolicyHandler()
  auto handler = mEngine->CreatePolicyHandler(options.isAuditDiscoveryEnabled);
  shared_ptr<mip::ContentLabel> label = handler->GetSensitivityLabel(state);
  if (nullptr != label)
    PrintLabel(label->GetLabel());
  else
    cout << "NO LABEL" << endl;
}

// Creates/loads an engine, shows policy data XML
void Action::ShowPolicyData() {
  EnsurePolicyEngine();
  cout << mEngine->GetPolicyDataXml();
}

// Creates/loads an engine, computes actions based on current execution state, and prints resulting actions
void Action::ComputeActions(const ExecutionStateOptions& options) {
  EnsurePolicyEngine();

  if (mProfileOptions.simulatePolicyChange)
    SimulatePolicyChange(mEngine);

  ExecutionStateImpl state(options);
  auto handler = mEngine->CreatePolicyHandler(options.isAuditDiscoveryEnabled);
  auto actions = handler->ComputeActions(state);
  if (!actions.empty()) {
    for (const shared_ptr<mip::Action>& action : actions)
      PrintAction(action);
  } else {
    cout << "NO ACTIONS" << endl;
  }
}

// Handles policy change notifications from PolicyProfile::Observer. The SDK periodically syncs the policy from the SCC
// service in the background. If the policy has changed in any way since the last sync (i.e. if the IT admin modified
// the policy through the OIP portal), the SDK will unload the engine and then fire this notification that the policy
// has changed. The application then must re-add the engine with the same engine id to perform operations against the
// updated policy.
void Action::OnPolicyChanged(const std::string& engineId) {
  mEngine = LoadExistingPolicyEngine(engineId);
}

void Action::EnsurePolicyEngine() {
  if (mProfileOptions.engineId.empty())
    mEngine = CreateNewPolicyEngine();
  else
    mEngine = LoadExistingPolicyEngine(mProfileOptions.engineId);
}

// Creates a new policy engine. Note that the same mip::PolicyProfile::AddEngineAsync API is used both to create a 
// new engine and load a cached engine. It is up to the application to remember/record the id for the newly-created 
// engine to prevent duplicate engines from being added to the cache.
shared_ptr<mip::PolicyEngine> Action::CreateNewPolicyEngine() {
  mip::Identity identity(mAuthOptions.username);

  // TODO: Describe client data
  string clientData = "my client data";

  mip::PolicyEngine::Settings settings(identity, clientData, mLocale, mLoadSensitivityTypes);
  settings.SetCustomSettings(GetCustomPolicySettings());

  // Create a context to pass to 'PolicyProfile::AddEngineAsync'. That context will be forwarded to the corresponding
  // PolicyProfile::Observer methods. In this case, we use promises/futures as a simple way to to detect the async 
  // operation completions synchronously.
  auto addEnginePromise = make_shared<promise<shared_ptr<mip::PolicyEngine>>>();
  future<shared_ptr<mip::PolicyEngine>> addEngineFuture = addEnginePromise->get_future();

  // An engine will exist for the lifetime of the profile unless:
  //  A) Engine is manually unloaded (mip::Policy::UnloadEngineAsync)
  //  B) Engine is manually deleted (mip::Policy::DeleteEngineAsync)
  //  C) Policy has changed (mip::Policy::Observer::OnPolicyChanged called), in which case engine must be re-added
  mProfile->AddEngineAsync(settings, addEnginePromise);
  shared_ptr<mip::PolicyEngine> engine = addEngineFuture.get();

  // If the profile is configured to use a file cache for its engines (mip::PolicyProfile::Settings::UseInMemoryStorage)
  // it is important for an application to remember/record the id for this newly-created engine across sessions to 
  // prevent duplicate engine entries from being created. Subsequent engine loads should set
  //  mip::PolicyProfile::Settings::Id to load the existing engine from storage. Also note that the 
  //  mip::PolicyProfile::Settings object held by the engine may be different than the 'settings' value passed to 
  // mip::PolicyProfile::AddEngineAsync. (e.g. engine id, session id, etc. Values may have been populated during the add 
  // process if they didn't already exist.)
  cout << "Engine added with id: '" << engine->GetSettings().GetEngineId() << "'" << endl;

  return engine;
}

// Loads an existing policy engine from the file cache. Note that the same mip::PolicyProfile::AddEngineAsync API is 
// used both to create a new engine and load a cached engine.
shared_ptr<mip::PolicyEngine> Action::LoadExistingPolicyEngine(const string& engineId) {
  string clientData = "my client data";
  mip::PolicyEngine::Settings settings(engineId, clientData, mLocale, mLoadSensitivityTypes);
  settings.SetCustomSettings(GetCustomPolicySettings());

  auto addEnginePromise = make_shared<promise<shared_ptr<mip::PolicyEngine>>>();
  future<shared_ptr<mip::PolicyEngine>> addEngineFuture = addEnginePromise->get_future();
  mProfile->AddEngineAsync(settings, addEnginePromise);
  shared_ptr<mip::PolicyEngine> engine = addEngineFuture.get();

  cout << "Engine loaded with id: '" << engineId << "'" << endl;

  return engine;
}

// Generates custom settings based on the sample app parameters. Custom settings are debug-only options that allow
// creation of a PolicyEngine in a non-standard way. These settings may not be supported long term and are not intended
// for use in production code.
vector<pair<string, string>> Action::GetCustomPolicySettings() {
  vector<pair<string, string>> customSettings;
  if (mProfileOptions.policyType == PolicyType::File)
    customSettings.emplace_back("policy_file", mProfileOptions.policyFile);

  return customSettings;
}

// Simulates a change in policy on a loaded engine. The SDK periodically syncs the policy from the SCC service in the
// background. If the policy has changed in any way since the last sync (i.e. if the IT admin modified the policy
// through the OIP portal), the SDK will unload the engine and then fire a notification to the profile observer that the
// policy has changed. The application then must re-add the engine with the same engine id to perform operations against
// the updated policy.
void Action::SimulatePolicyChange(const std::shared_ptr<mip::PolicyEngine>& engine) {
  string engineId = engine->GetSettings().GetEngineId();
  
  auto unloadEnginePromise = make_shared<promise<void>>();
  future<void> unloadEngineFuture = unloadEnginePromise->get_future();
  mProfile->UnloadEngineAsync(engineId, unloadEnginePromise);
  unloadEngineFuture.get();

  mProfileObserver->OnPolicyChanged(engineId);
}

} // namespace upe
} // namespace sample