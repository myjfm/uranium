// Generated by the gRPC protobuf plugin.
// If you make any local change, they will be lost.
// source: uranium_admin.proto

#include "uranium_admin.pb.h"
#include "uranium_admin.grpc.pb.h"

#include <grpc++/impl/codegen/async_stream.h>
#include <grpc++/impl/codegen/async_unary_call.h>
#include <grpc++/impl/codegen/channel_interface.h>
#include <grpc++/impl/codegen/client_unary_call.h>
#include <grpc++/impl/codegen/method_handler_impl.h>
#include <grpc++/impl/codegen/rpc_service_method.h>
#include <grpc++/impl/codegen/service_type.h>
#include <grpc++/impl/codegen/sync_stream.h>
namespace uranium {
namespace admin {

static const char* UraniumAdminService_method_names[] = {
  "/uranium.admin.UraniumAdminService/CreateTable",
  "/uranium.admin.UraniumAdminService/UpdateTable",
  "/uranium.admin.UraniumAdminService/DropTable",
  "/uranium.admin.UraniumAdminService/GetTableOptions",
};

std::unique_ptr< UraniumAdminService::Stub> UraniumAdminService::NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options) {
  std::unique_ptr< UraniumAdminService::Stub> stub(new UraniumAdminService::Stub(channel));
  return stub;
}

UraniumAdminService::Stub::Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel)
  : channel_(channel), rpcmethod_CreateTable_(UraniumAdminService_method_names[0], ::grpc::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_UpdateTable_(UraniumAdminService_method_names[1], ::grpc::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_DropTable_(UraniumAdminService_method_names[2], ::grpc::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_GetTableOptions_(UraniumAdminService_method_names[3], ::grpc::RpcMethod::NORMAL_RPC, channel)
  {}

::grpc::Status UraniumAdminService::Stub::CreateTable(::grpc::ClientContext* context, const ::uranium::admin::TableOptions& request, ::uranium::common::Result* response) {
  return ::grpc::BlockingUnaryCall(channel_.get(), rpcmethod_CreateTable_, context, request, response);
}

::grpc::ClientAsyncResponseReader< ::uranium::common::Result>* UraniumAdminService::Stub::AsyncCreateTableRaw(::grpc::ClientContext* context, const ::uranium::admin::TableOptions& request, ::grpc::CompletionQueue* cq) {
  return new ::grpc::ClientAsyncResponseReader< ::uranium::common::Result>(channel_.get(), cq, rpcmethod_CreateTable_, context, request);
}

::grpc::Status UraniumAdminService::Stub::UpdateTable(::grpc::ClientContext* context, const ::uranium::admin::TableOptions& request, ::uranium::common::Result* response) {
  return ::grpc::BlockingUnaryCall(channel_.get(), rpcmethod_UpdateTable_, context, request, response);
}

::grpc::ClientAsyncResponseReader< ::uranium::common::Result>* UraniumAdminService::Stub::AsyncUpdateTableRaw(::grpc::ClientContext* context, const ::uranium::admin::TableOptions& request, ::grpc::CompletionQueue* cq) {
  return new ::grpc::ClientAsyncResponseReader< ::uranium::common::Result>(channel_.get(), cq, rpcmethod_UpdateTable_, context, request);
}

::grpc::Status UraniumAdminService::Stub::DropTable(::grpc::ClientContext* context, const ::uranium::common::TableName& request, ::uranium::common::Result* response) {
  return ::grpc::BlockingUnaryCall(channel_.get(), rpcmethod_DropTable_, context, request, response);
}

::grpc::ClientAsyncResponseReader< ::uranium::common::Result>* UraniumAdminService::Stub::AsyncDropTableRaw(::grpc::ClientContext* context, const ::uranium::common::TableName& request, ::grpc::CompletionQueue* cq) {
  return new ::grpc::ClientAsyncResponseReader< ::uranium::common::Result>(channel_.get(), cq, rpcmethod_DropTable_, context, request);
}

::grpc::Status UraniumAdminService::Stub::GetTableOptions(::grpc::ClientContext* context, const ::uranium::common::TableName& request, ::uranium::admin::GetTableOptionsResponse* response) {
  return ::grpc::BlockingUnaryCall(channel_.get(), rpcmethod_GetTableOptions_, context, request, response);
}

::grpc::ClientAsyncResponseReader< ::uranium::admin::GetTableOptionsResponse>* UraniumAdminService::Stub::AsyncGetTableOptionsRaw(::grpc::ClientContext* context, const ::uranium::common::TableName& request, ::grpc::CompletionQueue* cq) {
  return new ::grpc::ClientAsyncResponseReader< ::uranium::admin::GetTableOptionsResponse>(channel_.get(), cq, rpcmethod_GetTableOptions_, context, request);
}

UraniumAdminService::Service::Service() {
  (void)UraniumAdminService_method_names;
  AddMethod(new ::grpc::RpcServiceMethod(
      UraniumAdminService_method_names[0],
      ::grpc::RpcMethod::NORMAL_RPC,
      new ::grpc::RpcMethodHandler< UraniumAdminService::Service, ::uranium::admin::TableOptions, ::uranium::common::Result>(
          std::mem_fn(&UraniumAdminService::Service::CreateTable), this)));
  AddMethod(new ::grpc::RpcServiceMethod(
      UraniumAdminService_method_names[1],
      ::grpc::RpcMethod::NORMAL_RPC,
      new ::grpc::RpcMethodHandler< UraniumAdminService::Service, ::uranium::admin::TableOptions, ::uranium::common::Result>(
          std::mem_fn(&UraniumAdminService::Service::UpdateTable), this)));
  AddMethod(new ::grpc::RpcServiceMethod(
      UraniumAdminService_method_names[2],
      ::grpc::RpcMethod::NORMAL_RPC,
      new ::grpc::RpcMethodHandler< UraniumAdminService::Service, ::uranium::common::TableName, ::uranium::common::Result>(
          std::mem_fn(&UraniumAdminService::Service::DropTable), this)));
  AddMethod(new ::grpc::RpcServiceMethod(
      UraniumAdminService_method_names[3],
      ::grpc::RpcMethod::NORMAL_RPC,
      new ::grpc::RpcMethodHandler< UraniumAdminService::Service, ::uranium::common::TableName, ::uranium::admin::GetTableOptionsResponse>(
          std::mem_fn(&UraniumAdminService::Service::GetTableOptions), this)));
}

UraniumAdminService::Service::~Service() {
}

::grpc::Status UraniumAdminService::Service::CreateTable(::grpc::ServerContext* context, const ::uranium::admin::TableOptions* request, ::uranium::common::Result* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status UraniumAdminService::Service::UpdateTable(::grpc::ServerContext* context, const ::uranium::admin::TableOptions* request, ::uranium::common::Result* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status UraniumAdminService::Service::DropTable(::grpc::ServerContext* context, const ::uranium::common::TableName* request, ::uranium::common::Result* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status UraniumAdminService::Service::GetTableOptions(::grpc::ServerContext* context, const ::uranium::common::TableName* request, ::uranium::admin::GetTableOptionsResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}


}  // namespace uranium
}  // namespace admin

