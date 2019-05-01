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

#include <sstream>

#ifdef __linux__
#include <unistd.h>
#ifndef MAX_PATH
#define MAX_PATH 4096
#endif
#endif // __linux__

#include "mip/version.h"

#include "action.h"
#include "cxxopts.hpp"
#include "string_utils.h"

using std::cout;
using std::endl;
using std::exception;
using std::getline;
using std::string;
using std::stringstream;
using std::vector;

namespace {

static const char kPathSeparatorWindows = '\\';
static const char kPathSeparatorUnix = '/';

vector<string> SplitString(const string& str, char delim) {
  vector<string> output;
  stringstream ss(str);
  string substr;

  while (ss.good()) {
    getline(ss, substr, delim);
    output.emplace_back(move(substr));
  }

  return output;
}

enum class SampleActionType {
  Invalid,
  ListEngines,
  ListLabels,
  ListSensitivityTypes,
  ShowDefaultLabel,
  ShowLabel,
  ShowPolicyData,
  ComputeActions,
};

bool ValidateOptions(
  SampleActionType actionType,
  const sample::upe::AuthenticationOptions& auth,
  const sample::upe::ProfileOptions& profile) {
  // Required options
  if (auth.username.empty()) {
    cout << "ERROR: No username specified." << endl;
    return false;
  }

  // Auth options
  if (auth.type == sample::upe::AuthenticationType::None && profile.policyType == sample::upe::PolicyType::Server) {
    cout << "ERROR: No authentication specified. Specify <password> or <token>." << endl;
    return false;
  }

  // Profile options
  if (profile.policyType == sample::upe::PolicyType::File && profile.policyFile.empty()) {
    cout << "ERROR: No policy file specified." << endl;
  }

  // Action options
  if (actionType == SampleActionType::Invalid) {
      cout << "ERROR: Unrecognized SampleActionType. Specify <listEngines>, <listLabels>, <showDefaultLabel>, or <computeActions>." << endl;
      return false;
  }

  return true;
}

string GetWorkingDirectory(int argc, char* argv[]) {
    string upeSamplePath;
    size_t position;
#ifdef _WIN32
    if (argc > 0) {
      upeSamplePath = string(argv[0]);
      position = upeSamplePath.find_last_of(kPathSeparatorWindows);
    }
#endif // _WIN32

#ifdef __linux__
    char result[MAX_PATH];
    auto count = readlink("/proc/self/exe", result, MAX_PATH);
    upeSamplePath = string(result, (count > 0) ? count : 0);
    if (count > 0) {
      position = upeSamplePath.find_last_of(kPathSeparatorUnix);
    }
#endif // __linux__
    upeSamplePath = upeSamplePath.substr(0, position + 1);
    return upeSamplePath;
}

} // namespace

int main_impl(int argc, char* argv[]) {
  try {
    const int argCount = argc; // need to save it as cxxopts change it while parsing
    string upeSampleWorkingDirectory = GetWorkingDirectory(argc, argv);
    const string appName = "Microsoft Information Protection UPE SDK Sample";
    const string helpText = appName + "\nVersion: " + VER_FILE_VERSION_STR;

    cxxopts::Options args("upe_sample", helpText);

    args.add_options()
      // Required options
      ("username", "(Required) User name.", cxxopts::value<string>())

      // Auth options
      ("password", "Set password for auth (with <username> and <clientId>).", cxxopts::value<string>())
      ("clientId", "(Optional) Override client id for auth (with <username> and <password>.", cxxopts::value<string>())
      ("token", "Set auth token (instead of <password>/<clientId>).", cxxopts::value<string>())

      // Profile options
      ("useStorageCache", "(Optional) Profile uses a to cache engines.")
      ("engineId", "(Optional) Load an engine from profile's storage cache by id rather than creating a new one.", cxxopts::value<string>())
      ("policyFile", "Import policy from xml file rather than from server.", cxxopts::value<string>())
      ("simulatePolicyChange", "(Optional) Simulate a policy change notification prior to performing any actions.")

      // Action choice
      ("listEngines", "List all engines in storage cache")
      ("listLabels", "List all labels available to <username>.")
      ("listSensitivityTypes", "List all sensitivity types")
      ("showDefaultLabel", "Shows default sensitivity label")
      ("computeActions", "List actions which should be taken given the specified execution state (<metadata>, <newLabelId>, <downgradeJustified>, <assignmentMethod>, <templateId>, <contentFormat>).")
      ("showLabel", "Calculate the current label based on given execution state (<metadata>, <templateId>, <contentFormat>).")
      ("showPolicyData", "Shows policy data XML which describes the settings, labels, and rules associated with this policy")

      // Execution state options
      ("metadata", "(Optional) Execution state: Comma-separated key-value pairs (ex: \"key1|value1,key2|value2\") (Default=empty)", cxxopts::value<string>())
      ("newLabelId", "(Optional) Execution state: Label id to be applied to content. (Default=none)", cxxopts::value<string>())
      ("assignmentMethod", "(Optional) Execution state: Assignment method for <newLabelId>. ['standard'|'privileged'|'auto'] (Default='standard')", cxxopts::value<string>())
      ("downgradeJustified", "(Optional) Execution state: Label downgrade has already been justified. (Default=false)")
      ("downgradeJustification", "(Optional) Execution state: Justification message if downgradeJustified is set.")
      ("templateId", "(Optional) Execution state: Id of template already protecting content. (Default=none)", cxxopts::value<string>())
      ("contentFormat", "(Optional) Execution state: Format of content. ['default'|'email'] (Default='default')", cxxopts::value<string>())
      ("dataState", "(Optional) Execution state: State of content. ['motion'|'use'|'rest'] (Default='rest')", cxxopts::value<string>())
      ("contentIdentifier", "(Optional) A unique string that identifies a a piece of content", cxxopts::value<string>())

      // Other options
      ("locale", "Set locale/language (default 'en-US')", cxxopts::value<string>())
      ("version", "Display version information.")
      ("h,help", "Display help information.");

    args.parse(argc, argv);

    if (argCount <= 1) {
      cout << args.help({ "" }) << endl;
      return 0;
    }

    if (args.count("help")) {
      cout << args.help({ "" }) << "\n\n";
      cout << "Examples:\n" <<
          "  List engines in storage cache:\n" <<
          "    upe_sample.exe --username <username> --contentIdentifier <filepath:filename> --useStorageCache --listEngines\n\n" <<
          "  List labels from override file:\n" <<
          "    upe_sample.exe --username <username> --contentIdentifier <filepath:filename> --listLabels --policyFile <policyFile>\n\n" <<
          "  List labels from server (token auth):\n" <<
          "    upe_sample.exe --username <username> --token <token> --contentIdentifier <filepath:filename> --listLabels\n\n" <<
          "  List labels from server (credential auth):\n" <<
          "    upe_sample.exe --username <username> --password <password> --contentIdentifier <filepath:filename> --listLabels\n\n" <<
          "  List labels from server, caching the engine:\n" <<
          "    upe_sample.exe --username <username> --token <token> --contentIdentifier <filepath:filename> --useStorageCache --listLabels\n\n" <<
          "  List labels from server, reusing a cached engine:\n" <<
          "    upe_sample.exe --username <username> --token <token> --contentIdentifier <filepath:filename> --useStorageCache --engineId <engineId> --listLabels\n\n" <<
          "  List labels from server, first simulating a policy change:\n" <<
          "    upe_sample.exe --username <username> --token <token> --contentIdentifier <filepath:filename> --simulatePolicyChange --listLabels\n\n" <<
          "  Show default label from server:\n" <<
          "    upe_sample.exe --username <username> --token <token> --contentIdentifier <filepath:filename> --showDefaultLabel\n\n" <<
          "  Compute current label given metadata:\n" <<
          "    upe_sample.exe --username <username> --password <password> --contentIdentifier <filepath:filename> --showLabel --metadata <metadata>\n\n" <<
          "  Compute actions - Apply a label:\n" <<
          "    upe_sample.exe --username <username> --token <token> --contentIdentifier <filepath:filename> --computeActions --newLabelId <newLabelId> --assignmentMethod <assignmentMethod> --contentFormat <contentFormat>\n\n" <<
          "  Compute actions - Apply a label to template-protected content:\n" <<
          "    upe_sample.exe --username <username> --token <token> --contentIdentifier <filepath:filename>--computeActions --newLabelId <newLabelId> --templateId <templateId>\n\n" <<
          endl;

      return 0;
    }

    if (args.count("version")) {
      cout << VER_FILE_VERSION_STR << endl;
      return 0;
    }

    string locale = "en-US";
    if (args.count("locale"))
      locale = args["locale"].as<string>();

    SampleActionType actionType = SampleActionType::Invalid;
    sample::upe::AuthenticationOptions auth;
    sample::upe::ProfileOptions profile;
    sample::upe::ExecutionStateOptions executionState;
    bool loadSensitivityTypes = false;

    // Parse required options
    if (args.count("username"))
      auth.username = args["username"].as<string>();
    if (args.count("contentIdentifier"))
      executionState.contentIdentifier = args["contentIdentifier"].as<string>();
    else 
      executionState.contentIdentifier = "";

    // Parse auth options
    if (args.count("password"))
      auth.password = args["password"].as<string>();
    if (args.count("clientId"))
      auth.clientId = args["clientId"].as<string>();
    if (args.count("token"))
      auth.token = args["token"].as<string>();

    if (!auth.password.empty())
      auth.type = sample::upe::AuthenticationType::UserCredentials;
    else if (!auth.token.empty())
      auth.type = sample::upe::AuthenticationType::Token;
    else
      auth.type = sample::upe::AuthenticationType::None;

    // Parse profile options
    if (args.count("useStorageCache"))
      profile.useStorageCache = true;
    if (args.count("simulatePolicyChange"))
      profile.simulatePolicyChange = true;
    if (args.count("engineId"))
      profile.engineId = args["engineId"].as<string>();
    if (args.count("policyFile")) {
      profile.policyType = sample::upe::PolicyType::File;
      profile.policyFile = args["policyFile"].as<string>();
    }

    // Parse action choice
    if (args.count("listEngines")) {
      actionType = SampleActionType::ListEngines;
    } else if (args.count("listLabels")) {
      actionType = SampleActionType::ListLabels;
    } else if (args.count("listSensitivityTypes")) {
      actionType = SampleActionType::ListSensitivityTypes;
      loadSensitivityTypes = true;
    } else if (args.count("showDefaultLabel")) {
      actionType = SampleActionType::ShowDefaultLabel;
    } else if (args.count("showLabel")) {
      actionType = SampleActionType::ShowLabel;
    } else if (args.count("showPolicyData")) {
      actionType = SampleActionType::ShowPolicyData;
    } else if (args.count("computeActions")) {
      actionType = SampleActionType::ComputeActions;
    } else {
      actionType = SampleActionType::Invalid;
    }

    // Parse execution state
    if (args.count("metadata")) {
      vector<string> metadataPairs = SplitString(args["metadata"].as<string>(), ',');
      for (const string& metadataPair : metadataPairs) {
        vector<string> keyValue = SplitString(metadataPair, '|');
        executionState.metadata[keyValue[0]] = keyValue[1];
      }
    }
    if (args.count("newLabelId"))
      executionState.newLabelId = args["newLabelId"].as<string>();
    if (args.count("assignmentMethod")) {
      string assignmentMethod = args["assignmentMethod"].as<string>();
      if (assignmentMethod == "standard") {
        executionState.assignmentMethod = mip::AssignmentMethod::STANDARD;
      } else if (assignmentMethod == "privileged") {
        executionState.assignmentMethod = mip::AssignmentMethod::PRIVILEGED;
      } else if (assignmentMethod == "auto") {
        executionState.assignmentMethod = mip::AssignmentMethod::AUTO;
      } else {
        cout << "ERROR: Invalid <assignmentMethod> value. Choose 'standard', 'privileged', or 'auto'" << endl;
        return -1;
      }
    }
    if (args.count("downgradeJustified"))
      executionState.isDowngradeJustified = true;
    if (args.count("dowgradeJustification"))
      executionState.downgradeJustification = args["downgradeJustification"].as<string>();
    if (args.count("templateId"))
      executionState.templateId = args["templateId"].as<string>();
    if (args.count("contentFormat")) {
      string assignmentMethod = args["contentFormat"].as<string>();
      if (assignmentMethod == "default") {
        executionState.contentFormat = mip::ContentFormat::DEFAULT;
      } else if (assignmentMethod == "email") {
        executionState.contentFormat = mip::ContentFormat::EMAIL;
      } else {
        cout << "ERROR: Invalid <contentFormat> value. Choose 'default' or 'email'" << endl;
        return -1;
      }
    }
    executionState.dataState = mip::DataState::USE;
    if (args.count("dataState")) {
      string dataState = args["dataState"].as<string>();
      if (dataState == "motion") {
        executionState.dataState = mip::DataState::MOTION;
      } else if (dataState == "use") {
        executionState.dataState = mip::DataState::USE;
      } else if (dataState == "rest") {
        executionState.dataState = mip::DataState::REST;
      } else {
        cout << "ERROR: Invalid <dataState> value. Choose 'motion', 'use', or 'rest'" << endl;
        return -1;
      }
    }

    if (!ValidateOptions(actionType, auth, profile))
      return -1;

    sample::upe::Action action(auth, profile, locale, upeSampleWorkingDirectory, loadSensitivityTypes);

    switch (actionType) {
    case SampleActionType::ListEngines:
      action.ListEngines();
      break;
    case SampleActionType::ListLabels:
      action.ListLabels();
      break;
    case SampleActionType::ListSensitivityTypes:
      action.ListSensitivityTypes();
      break;
    case SampleActionType::ShowDefaultLabel:
      action.ShowDefaultLabel();
      break;
    case SampleActionType::ShowLabel:
      action.ShowLabel(executionState);
      break;
    case SampleActionType::ShowPolicyData:
      action.ShowPolicyData();
      break;
    case SampleActionType::ComputeActions:
      action.ComputeActions(executionState);
      break;
    default:
      cout << "ERROR - Invalid action type" << endl;
    }
  } catch (const cxxopts::OptionException& ex) {
    cout << "ERROR - Failed to parse options: " << ex.what() << endl;
    return -1;
  } catch (const exception& ex) {
    cout << "ERROR - Unexpected exception: '" << ex.what() << "'" << endl;
    return -1;
  }

  return 0;
}

#ifdef _WIN32
int wmain(int argc, wchar_t *argv[]) {
  std::vector<std::string> args;
  for (int i = 0; i < argc; ++i)
    args.push_back(ConvertWStringToString(argv[i]));

  std::unique_ptr<char*[]> ptr(new char*[argc + 1]);
  for (int i = 0; i < argc; ++i)
    ptr[i] = const_cast<char*>(args[i].c_str());
  ptr[argc] = nullptr;

  return main_impl(argc, ptr.get());
}
#else
int main(int argc, char** argv) {
  return main_impl(argc, argv);
}
#endif