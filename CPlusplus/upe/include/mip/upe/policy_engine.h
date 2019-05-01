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
 * @brief This file contains the PolicyEngine class which includes the PolicyEngine::Settings class.
 * 
 * @file policy_engine.h
 */

#ifndef API_MIP_UPE_POLICY_ENGINE_H_
#define API_MIP_UPE_POLICY_ENGINE_H_

#include <memory>
#include <string>
#include <vector>

#include "mip/common_types.h"
#include "mip/upe/action.h"
#include "mip/upe/content_label.h"
#include "mip/upe/execution_state.h"
#include "mip/upe/label.h"
#include "mip/upe/policy_handler.h"
#include "mip/upe/sensitivity_types_rule_package.h"

namespace mip {

/**
 * @brief This class provides an interface for all engine functions.
 */
class PolicyEngine {
public:
  /**
   * @brief Defines the settings associated with a PolicyEngine. 
   */
  class Settings {
  public:
    /**
     * @brief PolicyEngine::Settings constructor for loading an existing engine.
     *
     * @param engineId Set it to the unique engine ID generated by AddEngineAsync or one self-generated. When loading an
     * existing engine, reuse the ID else a new engine will be created.
     * @param clientData customizable client data that can be stored with the engine when unloaded, can be retrieved from
     * a loaded engine.
     * @param locale engine localizable output will be provided in this locale.
     * @param Optional flag indicating when the engine is loaded should load also custom sensitivity types, 
     * if true OnPolicyChange Observer on the profile will be invoked on updates to custom sensitivity types as well as policy changes.
     * if false ListSensitivityTypes call will always return an empty list.
     */
    Settings(
        const std::string& engineId,
        const std::string& clientData,
        const std::string& locale = "en-US",
        bool loadSensitivityTypes = false)
      : mEngineId(engineId),
        mClientData(clientData),
        mLocale(locale),
        mIsLoadSensitivityTypesEnabled(loadSensitivityTypes){
    }

    /**
     * @brief PolicyEngine::Settings constructor for creating a new engine.
     *
     * @param identity Identity info of the user associated with the new engine.
     * @param clientData customizable client data that can be stored with the engine when unloaded, can be retrieved from
     * a loaded engine.
     * @param locale engine localizable output will be provided in this locale.
     * @param Optional flag indicating when the engine is loaded should load also custom sensitivity types, 
     * if true OnPolicyChange Observer on the profile will be invoked on updates to custom sensitivity types as well as policy changes.
     * if false ListSensitivityTypes call will always return an empty list.
     */
    Settings(
        const Identity& identity,
        const std::string& clientData,
        const std::string& locale = "en-US",
        bool loadSensitivityTypes = false)
      : mIdentity(identity),
        mClientData(clientData),
        mLocale(locale),
        mIsLoadSensitivityTypesEnabled(loadSensitivityTypes) {
    }
    /**
     * @brief Get the engine ID.
     * 
     * @return a unique string identifying the engine.
     */
    const std::string& GetEngineId() const { return mEngineId; }
    /**
     * @brief Set the engine ID.
     * 
     * @param id engine ID.
     */
    void SetEngineId(const std::string& id) { mEngineId = id; }
    /**
     * @brief Get the Identity object.
     * 
     * @return a reference to the identity in the settings object.
     * @see mip::Identity
     */
    const Identity& GetIdentity() const { return mIdentity; }
    /**
     * @brief Set the Identity object.
     * 
     * @param identity the unique identity of a user.
     * @see mip::Identity
     */
    void SetIdentity(const Identity& identity) { mIdentity = identity; }
    /**
     * @brief Get the Client Data set in the settings.
     * 
     * @return a string of data specified by the client.
     */
    const std::string& GetClientData() const { return mClientData; }
    /**
     * @brief Set the Client Data string. 
     * 
     * @param clientData user specified data.
     */
    void SetClientData(const std::string& clientData) { mClientData = clientData; }
    /**
     * @brief Get the Locale set in the settings.
     * 
     * @return the locale.
     */
    const std::string& GetLocale() const { return mLocale; }
    /**
     * @brief Set the custom settings, used for feature gating and testing.
     * 
     * @param customSettings List of name/value pairs.
     */
    void SetCustomSettings(const std::vector<std::pair<std::string, std::string>>& customSettings) {
      mCustomSettings = customSettings;
    }
    /**
     * @brief Get the custom settings, used for feature gating and testing.
     * 
     * @return List of name/value pairs.
     */
    const std::vector<std::pair<std::string, std::string>>& GetCustomSettings() const {
      return mCustomSettings;
    }
    /**
     * @brief Set the session ID, used for client defined telemetry. 
     * 
     * @param sessionId a unique string that connects telemetry events.
     */
    void SetSessionId(const std::string& sessionId) {
      mSessionId = sessionId;
    }
    /**
     * @brief Get the session ID, a unique identifier.
     * 
     * @return the session ID.
     */
    const std::string& GetSessionId() const {
      return mSessionId;
    }
    /**
     * @brief Get the the flag indicating if load sensitivity labels is enabled.
     * 
     * @return true if enabled else false.
     */
    bool IsLoadSensitivityTypesEnabled() const {
      return mIsLoadSensitivityTypesEnabled;
    }

    /**
    * @brief Optionally sets the cloud endpoint base URL
    * 
    * @param cloudEndpointBaseUrl the base URL used by all service requests (for example,
    * "https://dataservice.protection.outlook.com")
    */
    void SetCloudEndpointBaseUrl(const std::string& cloudEndpointBaseUrl) {
      mCloudEndpointBaseUrl = cloudEndpointBaseUrl;
    }

    /**
    * @brief Gets the cloud base URL used by all service requests, if specified
    * 
    * @return base URL
    */
    const std::string& GetCloudEndpointBaseUrl() const {
      return mCloudEndpointBaseUrl;
    }

  private:
    std::string mEngineId;
    Identity mIdentity;
    std::string mClientData;
    std::vector<std::pair<std::string, std::string>> mCustomSettings;
    std::string mLocale;
    std::string mSessionId;
    bool mIsLoadSensitivityTypesEnabled;
    std::string mCloudEndpointBaseUrl;
  };

  /**
   * @brief Get the policy engine Settings. 
   * 
   * @return policy engine settings.
   * @see mip::PolicyEngine::Settings
   */
  virtual const Settings& GetSettings() const = 0;

  /**
   * @brief list the sensitivity labels associated with the policy engine.
   * 
   * @return a list of sensitivity labels.
   */
  virtual const std::vector<std::shared_ptr<Label>>& ListSensitivityLabels() = 0;

  /**
   * @brief list the sensitivity types associated with the policy engine.
   * 
   * @return a list of sensitivity labels. empty if LoadSensitivityTypesEnabled was false (@see PolicyEngine::Settings).
   */
  virtual const std::vector<std::shared_ptr<SensitivityTypesRulePackage>>& ListSensitivityTypes() const = 0;

  /**
   * @brief Provide a url for looking up more information about the policy/labels.
   * 
   * @return a url in string format.
   */
  virtual const std::string& GetMoreInfoUrl() const = 0;

  /**
   * @brief Checks if the policy dictates that a document must be labeled or not.
   * 
   * @return true if labeling is mandatory, else false. 
   */
  virtual bool IsLabelingRequired() const = 0; 

  /**
   * @brief Get the default sensitivity label. 
   * 
   * @return default sensitivity label if exists, nullptr if there is no default label set.
   */
  virtual std::shared_ptr<Label> GetDefaultSensitivityLabel() = 0;

  /**
   * @brief Create a Policy Handler to execute policy-related functions on a file's execution state.
   * 
   * @param A bool representing whether audit discovery is enabled or not.
   * 
   * @return Policy Handler.
   * 
   * @note Application needs to keep the policy handler object for the lifetime of the document.
   */
  virtual std::shared_ptr<PolicyHandler> CreatePolicyHandler(bool isAuditDiscoveryEnabled) = 0;

  /**
   * @brief Logs an application specific event to the audit pipeline.
   *
   * @param level of the log level: Info/Error/Warning.
   * @param eventType a description of the type of event.
   * @param eventData the data associated with the event.
   */
  virtual void SendApplicationAuditEvent(
      const std::string& level,
      const std::string& eventType,
      const std::string& eventData) = 0;

  /**
   * @brief Gets policy data XML which describes the settings, labels, and rules associated with this policy.
   * 
   * @return Policy data XML.
   */
  virtual const std::string& GetPolicyDataXml() const= 0;

  /**
   * @brief Gets a list of custom settings.
   * 
   * @return a vector of custom settings.
   */
  virtual const std::vector<std::pair<std::string, std::string>>& GetCustomSettings() const = 0;

  /**
   * @brief Gets the policy ID
   *
   * @return a string that represnt the policy ID
   */
  virtual const std::string& GetPolicyId() const = 0;

  /**
   * @brief Gets if the policy has automatic or recommendation rules
   *
   * @return a bool that will tell if there any automatic or recommandation rules in 
   *  the policy
   */
  virtual bool HasClassificationRules() const = 0;
  
  /**
   * @brief Gets the time when the policy was last fetched
   *
   * @return the time when the policy was last fetched
   */
  virtual std::chrono::time_point<std::chrono::system_clock> GetLastPolicyFetchTime() const = 0;


  /** @cond DOXYGEN_HIDE */
  virtual ~PolicyEngine() { }

protected:
/** @cond DOXYGEN_HIDE */
  PolicyEngine() {}
  /** @endcond */
};

}  // namespace mip

#endif  // API_MIP_UPE_POLICY_ENGINE_H_