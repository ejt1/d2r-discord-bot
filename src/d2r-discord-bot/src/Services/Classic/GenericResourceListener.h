#pragma once

#include "Rpc/Service.h"

#include <external/v1/generic_resource.pb.h>

namespace d2rc {

namespace resources_v1 = classic::protocol::external::v1::generic_resource;

class GenericResourceListenerImpl : public resources_v1::GenericResourceListener {
 protected:
  asio::awaitable<bgs::rpc::Response> ReqGenericResource(std::shared_ptr<bgs::rpc::Channel> channel,
                                                         const bgs::protocol::Header& header,
                                                         const resources_v1::GenericResourceRequest& request);

  asio::awaitable<bgs::rpc::Response> SetAvailableResources(std::shared_ptr<bgs::rpc::Channel> channel,
                                                            const bgs::protocol::Header& header,
                                                            const resources_v1::SetAvailableResourcesRequest& request);
};

}  // namespace d2rc
