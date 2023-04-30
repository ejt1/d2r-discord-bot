#include "Services/Classic/DiabloCloneExternalListener.h"

#include "Client.h"
#include "DiabloClone.h"
#include "Transport/Connection.h"

namespace d2rc {

asio::awaitable<bgs::rpc::Response> DiabloCloneExternalListenerImpl::DiabloCloneMessageTier(
    std::shared_ptr<bgs::rpc::Channel> channel, const bgs::protocol::Header& header,
    const dclone_v1::DiabloCloneMessageTierRequest& request) {
  auto realm = std::dynamic_pointer_cast<RealmSession>(channel);
  Region& region = realm->GetRegion();

  DiabloClone* dclone = DiabloClone::GetInstance();
  for (size_t i = 0; i < request.tiers_size(); ++i) {
    auto& tier = request.tiers(i);
    dclone->SetStatus(region, tier.game_type(), tier.progression());
  }

  bgs::protocol::NoData response;
  co_return bgs::rpc::Response(response);
}

}  // namespace d2rc
