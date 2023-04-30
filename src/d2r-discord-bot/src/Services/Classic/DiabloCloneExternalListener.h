#pragma once

#include "Rpc/Service.h"

#include <external/v1/diablo_clone.pb.h>

namespace d2rc {

namespace dclone_v1 = classic::protocol::external::v1::diablo_clone;

class DiabloCloneExternalListenerImpl : public dclone_v1::DiabloCloneExternalListener {
 protected:
  asio::awaitable<bgs::rpc::Response> DiabloCloneMessageTier(std::shared_ptr<bgs::rpc::Channel> channel,
                                                             const bgs::protocol::Header& header,
                                                             const dclone_v1::DiabloCloneMessageTierRequest& request);
};

}  // namespace d2rc
