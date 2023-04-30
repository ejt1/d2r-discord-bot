#include "Services/SessionListener.h"

#include "BnetSession.h"
#include "Client.h"
#include "ClientRepository.h"
#include "RealmSession.h"

#include <bgs/low/pb/client/api/client/v2/attribute_types.pb.h>
#include <bgs/low/pb/client/api/client/v2/game_utilities_service.pb.h>
#include <external/v1/d2r_connection.pb.h>
#include <external/v1/game_version.pb.h>

namespace d2rc {

asio::awaitable<bgs::rpc::Response> SessionListenerImpl::OnSessionCreated(
    std::shared_ptr<bgs::rpc::Channel> channel, const bgs::protocol::Header& header,
    const session_v1::SessionCreatedNotification& request) {
  auto bgs_conn = std::dynamic_pointer_cast<BnetSession>(channel);
  co_await bgs_conn->OnSessionCreated(request);

  co_return bgs::rpc::Response();
}

}  // namespace d2rc
