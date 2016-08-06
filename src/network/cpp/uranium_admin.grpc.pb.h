// Generated by the gRPC protobuf plugin.
// If you make any local change, they will be lost.
// source: uranium_admin.proto
// Original file comments:
// Copyright (c) 2016, myjfm(mwxjmmyjfm@gmail.com).  All rights reserved.
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree. An additional grant
// of patent rights can be found in the PATENTS file in the same directory.
//
#ifndef GRPC_uranium_5fadmin_2eproto__INCLUDED
#define GRPC_uranium_5fadmin_2eproto__INCLUDED

#include "uranium_admin.pb.h"

#include <grpc++/impl/codegen/async_stream.h>
#include <grpc++/impl/codegen/async_unary_call.h>
#include <grpc++/impl/codegen/proto_utils.h>
#include <grpc++/impl/codegen/rpc_method.h>
#include <grpc++/impl/codegen/service_type.h>
#include <grpc++/impl/codegen/status.h>
#include <grpc++/impl/codegen/stub_options.h>
#include <grpc++/impl/codegen/sync_stream.h>

namespace grpc {
class CompletionQueue;
class Channel;
class RpcService;
class ServerCompletionQueue;
class ServerContext;
}  // namespace grpc

namespace uranium {
namespace admin {

class UraniumAdminService GRPC_FINAL {
 public:
  class StubInterface {
   public:
    virtual ~StubInterface() {}
    virtual ::grpc::Status CreateTable(::grpc::ClientContext* context, const ::uranium::admin::TableOptions& request, ::uranium::common::Result* response) = 0;
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::uranium::common::Result>> AsyncCreateTable(::grpc::ClientContext* context, const ::uranium::admin::TableOptions& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::uranium::common::Result>>(AsyncCreateTableRaw(context, request, cq));
    }
    virtual ::grpc::Status UpdateTable(::grpc::ClientContext* context, const ::uranium::admin::TableOptions& request, ::uranium::common::Result* response) = 0;
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::uranium::common::Result>> AsyncUpdateTable(::grpc::ClientContext* context, const ::uranium::admin::TableOptions& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::uranium::common::Result>>(AsyncUpdateTableRaw(context, request, cq));
    }
    virtual ::grpc::Status DropTable(::grpc::ClientContext* context, const ::uranium::admin::DropTableRequest& request, ::uranium::common::Result* response) = 0;
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::uranium::common::Result>> AsyncDropTable(::grpc::ClientContext* context, const ::uranium::admin::DropTableRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::uranium::common::Result>>(AsyncDropTableRaw(context, request, cq));
    }
    virtual ::grpc::Status GetTableOptions(::grpc::ClientContext* context, const ::uranium::admin::GetTableOptionsRequest& request, ::uranium::admin::GetTableOptionsResponse* response) = 0;
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::uranium::admin::GetTableOptionsResponse>> AsyncGetTableOptions(::grpc::ClientContext* context, const ::uranium::admin::GetTableOptionsRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::uranium::admin::GetTableOptionsResponse>>(AsyncGetTableOptionsRaw(context, request, cq));
    }
  private:
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::uranium::common::Result>* AsyncCreateTableRaw(::grpc::ClientContext* context, const ::uranium::admin::TableOptions& request, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::uranium::common::Result>* AsyncUpdateTableRaw(::grpc::ClientContext* context, const ::uranium::admin::TableOptions& request, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::uranium::common::Result>* AsyncDropTableRaw(::grpc::ClientContext* context, const ::uranium::admin::DropTableRequest& request, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::uranium::admin::GetTableOptionsResponse>* AsyncGetTableOptionsRaw(::grpc::ClientContext* context, const ::uranium::admin::GetTableOptionsRequest& request, ::grpc::CompletionQueue* cq) = 0;
  };
  class Stub GRPC_FINAL : public StubInterface {
   public:
    Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel);
    ::grpc::Status CreateTable(::grpc::ClientContext* context, const ::uranium::admin::TableOptions& request, ::uranium::common::Result* response) GRPC_OVERRIDE;
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::uranium::common::Result>> AsyncCreateTable(::grpc::ClientContext* context, const ::uranium::admin::TableOptions& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::uranium::common::Result>>(AsyncCreateTableRaw(context, request, cq));
    }
    ::grpc::Status UpdateTable(::grpc::ClientContext* context, const ::uranium::admin::TableOptions& request, ::uranium::common::Result* response) GRPC_OVERRIDE;
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::uranium::common::Result>> AsyncUpdateTable(::grpc::ClientContext* context, const ::uranium::admin::TableOptions& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::uranium::common::Result>>(AsyncUpdateTableRaw(context, request, cq));
    }
    ::grpc::Status DropTable(::grpc::ClientContext* context, const ::uranium::admin::DropTableRequest& request, ::uranium::common::Result* response) GRPC_OVERRIDE;
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::uranium::common::Result>> AsyncDropTable(::grpc::ClientContext* context, const ::uranium::admin::DropTableRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::uranium::common::Result>>(AsyncDropTableRaw(context, request, cq));
    }
    ::grpc::Status GetTableOptions(::grpc::ClientContext* context, const ::uranium::admin::GetTableOptionsRequest& request, ::uranium::admin::GetTableOptionsResponse* response) GRPC_OVERRIDE;
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::uranium::admin::GetTableOptionsResponse>> AsyncGetTableOptions(::grpc::ClientContext* context, const ::uranium::admin::GetTableOptionsRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::uranium::admin::GetTableOptionsResponse>>(AsyncGetTableOptionsRaw(context, request, cq));
    }

   private:
    std::shared_ptr< ::grpc::ChannelInterface> channel_;
    ::grpc::ClientAsyncResponseReader< ::uranium::common::Result>* AsyncCreateTableRaw(::grpc::ClientContext* context, const ::uranium::admin::TableOptions& request, ::grpc::CompletionQueue* cq) GRPC_OVERRIDE;
    ::grpc::ClientAsyncResponseReader< ::uranium::common::Result>* AsyncUpdateTableRaw(::grpc::ClientContext* context, const ::uranium::admin::TableOptions& request, ::grpc::CompletionQueue* cq) GRPC_OVERRIDE;
    ::grpc::ClientAsyncResponseReader< ::uranium::common::Result>* AsyncDropTableRaw(::grpc::ClientContext* context, const ::uranium::admin::DropTableRequest& request, ::grpc::CompletionQueue* cq) GRPC_OVERRIDE;
    ::grpc::ClientAsyncResponseReader< ::uranium::admin::GetTableOptionsResponse>* AsyncGetTableOptionsRaw(::grpc::ClientContext* context, const ::uranium::admin::GetTableOptionsRequest& request, ::grpc::CompletionQueue* cq) GRPC_OVERRIDE;
    const ::grpc::RpcMethod rpcmethod_CreateTable_;
    const ::grpc::RpcMethod rpcmethod_UpdateTable_;
    const ::grpc::RpcMethod rpcmethod_DropTable_;
    const ::grpc::RpcMethod rpcmethod_GetTableOptions_;
  };
  static std::unique_ptr<Stub> NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options = ::grpc::StubOptions());

  class Service : public ::grpc::Service {
   public:
    Service();
    virtual ~Service();
    virtual ::grpc::Status CreateTable(::grpc::ServerContext* context, const ::uranium::admin::TableOptions* request, ::uranium::common::Result* response);
    virtual ::grpc::Status UpdateTable(::grpc::ServerContext* context, const ::uranium::admin::TableOptions* request, ::uranium::common::Result* response);
    virtual ::grpc::Status DropTable(::grpc::ServerContext* context, const ::uranium::admin::DropTableRequest* request, ::uranium::common::Result* response);
    virtual ::grpc::Status GetTableOptions(::grpc::ServerContext* context, const ::uranium::admin::GetTableOptionsRequest* request, ::uranium::admin::GetTableOptionsResponse* response);
  };
  template <class BaseClass>
  class WithAsyncMethod_CreateTable : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithAsyncMethod_CreateTable() {
      ::grpc::Service::MarkMethodAsync(0);
    }
    ~WithAsyncMethod_CreateTable() GRPC_OVERRIDE {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status CreateTable(::grpc::ServerContext* context, const ::uranium::admin::TableOptions* request, ::uranium::common::Result* response) GRPC_FINAL GRPC_OVERRIDE {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestCreateTable(::grpc::ServerContext* context, ::uranium::admin::TableOptions* request, ::grpc::ServerAsyncResponseWriter< ::uranium::common::Result>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(0, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  template <class BaseClass>
  class WithAsyncMethod_UpdateTable : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithAsyncMethod_UpdateTable() {
      ::grpc::Service::MarkMethodAsync(1);
    }
    ~WithAsyncMethod_UpdateTable() GRPC_OVERRIDE {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status UpdateTable(::grpc::ServerContext* context, const ::uranium::admin::TableOptions* request, ::uranium::common::Result* response) GRPC_FINAL GRPC_OVERRIDE {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestUpdateTable(::grpc::ServerContext* context, ::uranium::admin::TableOptions* request, ::grpc::ServerAsyncResponseWriter< ::uranium::common::Result>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(1, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  template <class BaseClass>
  class WithAsyncMethod_DropTable : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithAsyncMethod_DropTable() {
      ::grpc::Service::MarkMethodAsync(2);
    }
    ~WithAsyncMethod_DropTable() GRPC_OVERRIDE {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status DropTable(::grpc::ServerContext* context, const ::uranium::admin::DropTableRequest* request, ::uranium::common::Result* response) GRPC_FINAL GRPC_OVERRIDE {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestDropTable(::grpc::ServerContext* context, ::uranium::admin::DropTableRequest* request, ::grpc::ServerAsyncResponseWriter< ::uranium::common::Result>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(2, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  template <class BaseClass>
  class WithAsyncMethod_GetTableOptions : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithAsyncMethod_GetTableOptions() {
      ::grpc::Service::MarkMethodAsync(3);
    }
    ~WithAsyncMethod_GetTableOptions() GRPC_OVERRIDE {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status GetTableOptions(::grpc::ServerContext* context, const ::uranium::admin::GetTableOptionsRequest* request, ::uranium::admin::GetTableOptionsResponse* response) GRPC_FINAL GRPC_OVERRIDE {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestGetTableOptions(::grpc::ServerContext* context, ::uranium::admin::GetTableOptionsRequest* request, ::grpc::ServerAsyncResponseWriter< ::uranium::admin::GetTableOptionsResponse>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(3, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  typedef WithAsyncMethod_CreateTable<WithAsyncMethod_UpdateTable<WithAsyncMethod_DropTable<WithAsyncMethod_GetTableOptions<Service > > > > AsyncService;
  template <class BaseClass>
  class WithGenericMethod_CreateTable : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithGenericMethod_CreateTable() {
      ::grpc::Service::MarkMethodGeneric(0);
    }
    ~WithGenericMethod_CreateTable() GRPC_OVERRIDE {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status CreateTable(::grpc::ServerContext* context, const ::uranium::admin::TableOptions* request, ::uranium::common::Result* response) GRPC_FINAL GRPC_OVERRIDE {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
  };
  template <class BaseClass>
  class WithGenericMethod_UpdateTable : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithGenericMethod_UpdateTable() {
      ::grpc::Service::MarkMethodGeneric(1);
    }
    ~WithGenericMethod_UpdateTable() GRPC_OVERRIDE {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status UpdateTable(::grpc::ServerContext* context, const ::uranium::admin::TableOptions* request, ::uranium::common::Result* response) GRPC_FINAL GRPC_OVERRIDE {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
  };
  template <class BaseClass>
  class WithGenericMethod_DropTable : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithGenericMethod_DropTable() {
      ::grpc::Service::MarkMethodGeneric(2);
    }
    ~WithGenericMethod_DropTable() GRPC_OVERRIDE {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status DropTable(::grpc::ServerContext* context, const ::uranium::admin::DropTableRequest* request, ::uranium::common::Result* response) GRPC_FINAL GRPC_OVERRIDE {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
  };
  template <class BaseClass>
  class WithGenericMethod_GetTableOptions : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithGenericMethod_GetTableOptions() {
      ::grpc::Service::MarkMethodGeneric(3);
    }
    ~WithGenericMethod_GetTableOptions() GRPC_OVERRIDE {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status GetTableOptions(::grpc::ServerContext* context, const ::uranium::admin::GetTableOptionsRequest* request, ::uranium::admin::GetTableOptionsResponse* response) GRPC_FINAL GRPC_OVERRIDE {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
  };
};

}  // namespace admin
}  // namespace uranium


#endif  // GRPC_uranium_5fadmin_2eproto__INCLUDED
