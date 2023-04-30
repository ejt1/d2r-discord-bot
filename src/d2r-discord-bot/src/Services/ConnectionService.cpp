#include "Services/ConnectionService.h"

#include "Rpc/Channel.h"

namespace d2rc {

asio::awaitable<bgs::rpc::Response> ConnectionServiceImpl::Echo(std::shared_ptr<bgs::rpc::Channel> channel,
                                                                const bgs::protocol::Header& header,
                                                                const connection_v1::EchoRequest& request) {
  connection_v1::EchoResponse response;
  co_return bgs::rpc::Response(response);
}

}  // namespace d2rc
