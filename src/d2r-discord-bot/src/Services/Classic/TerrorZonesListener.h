#pragma once

#include "Rpc/Service.h"

#include <external/v1/terror_zones.pb.h>

namespace d2rc {

namespace terror_v1 = classic::protocol::external::v1::terror_zones;

class TerrorZonesListenerImpl : public terror_v1::TerrorZonesListener {
 protected:
  asio::awaitable<bgs::rpc::Response> TerrorZonesMessage(std::shared_ptr<bgs::rpc::Channel> channel,
                                                         const bgs::protocol::Header& header,
                                                         const terror_v1::TerrorZonesMessageRequest& request) override;
};

}  // namespace d2rc
