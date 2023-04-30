#include "Services/Classic/GenericResourceListener.h"

#include "Rpc/Channel.h"

namespace d2rc {

asio::awaitable<bgs::rpc::Response> GenericResourceListenerImpl::ReqGenericResource(
    std::shared_ptr<bgs::rpc::Channel> channel, const bgs::protocol::Header& header,
    const resources_v1::GenericResourceRequest& request) {
  co_return bgs::rpc::ErrorCode::OK;
}

asio::awaitable<bgs::rpc::Response> GenericResourceListenerImpl::SetAvailableResources(
    std::shared_ptr<bgs::rpc::Channel> channel, const bgs::protocol::Header& header,
    const resources_v1::SetAvailableResourcesRequest& request) {
  classic::protocol::external::v1::generic_resource::SetAvailableResourcesResponse response;

  classic::protocol::external::v1::generic_resource::GenericResourceRequest generic_request;
  generic_request.set_locale("enUS");
  auto* motd = generic_request.add_data();
  motd->set_name("motd");
  motd->set_unknown2("");
  auto* motd_title = generic_request.add_data();
  motd_title->set_name("motd_title");
  motd_title->set_unknown2("");

  resources_v1::GenericResourceListener_Stub stub(channel);
  co_await stub.ReqGenericResource(generic_request);

  co_return bgs::rpc::Response(response);
}

}  // namespace d2rc
