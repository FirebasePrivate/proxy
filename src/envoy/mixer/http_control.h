/* Copyright 2016 Google Inc. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include "precompiled/precompiled.h"

#include "common/common/logger.h"
#include "common/http/headers.h"
#include "envoy/http/access_log.h"
#include "include/client.h"

namespace Http {
namespace Mixer {

// Store data from Check to report
struct HttpRequestData {
  ::istio::mixer_client::Attributes attributes;
};
typedef std::shared_ptr<HttpRequestData> HttpRequestDataPtr;

// The mixer client class to control HTTP requests.
// It has Check() to validate if a request can be processed.
// At the end of request, call Report().
class HttpControl final : public Logger::Loggable<Logger::Id::http> {
 public:
  // The constructor.
  HttpControl(const std::string& mixer_server);

  // Make mixer check call.
  void Check(HttpRequestDataPtr request_data, HeaderMap& headers,
             ::istio::mixer_client::DoneFunc on_done);

  // Make mixer report call.
  void Report(HttpRequestDataPtr request_data,
              const HeaderMap* response_headers,
              const AccessLog::RequestInfo& request_info,
              ::istio::mixer_client::DoneFunc on_done);

 private:
  void FillCheckAttributes(const HeaderMap& header_map,
                           ::istio::mixer_client::Attributes* attr);

  // The mixer client
  std::unique_ptr<::istio::mixer_client::MixerClient> mixer_client_;
  // Target service
  std::string target_service_;
};

}  // namespace Mixer
}  // namespace Http
