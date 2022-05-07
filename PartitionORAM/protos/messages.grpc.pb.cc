// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: messages.proto

#include "messages.pb.h"
#include "messages.grpc.pb.h"

#include <functional>
#include <grpcpp/impl/codegen/async_stream.h>
#include <grpcpp/impl/codegen/async_unary_call.h>
#include <grpcpp/impl/codegen/channel_interface.h>
#include <grpcpp/impl/codegen/client_unary_call.h>
#include <grpcpp/impl/codegen/client_callback.h>
#include <grpcpp/impl/codegen/message_allocator.h>
#include <grpcpp/impl/codegen/method_handler.h>
#include <grpcpp/impl/codegen/rpc_service_method.h>
#include <grpcpp/impl/codegen/server_callback.h>
#include <grpcpp/impl/codegen/server_callback_handlers.h>
#include <grpcpp/impl/codegen/server_context.h>
#include <grpcpp/impl/codegen/service_type.h>
#include <grpcpp/impl/codegen/sync_stream.h>
namespace partition_oram {

static const char* server_method_names[] = {
  "/partition_oram.server/init_oram",
  "/partition_oram.server/read_path",
  "/partition_oram.server/write_path",
  "/partition_oram.server/close_connection",
  "/partition_oram.server/key_exchange",
  "/partition_oram.server/send_hello",
};

std::unique_ptr< server::Stub> server::NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options) {
  (void)options;
  std::unique_ptr< server::Stub> stub(new server::Stub(channel, options));
  return stub;
}

server::Stub::Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options)
  : channel_(channel), rpcmethod_init_oram_(server_method_names[0], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_read_path_(server_method_names[1], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_write_path_(server_method_names[2], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_close_connection_(server_method_names[3], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_key_exchange_(server_method_names[4], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_send_hello_(server_method_names[5], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  {}

::grpc::Status server::Stub::init_oram(::grpc::ClientContext* context, const ::partition_oram::InitOramRequest& request, ::google::protobuf::Empty* response) {
  return ::grpc::internal::BlockingUnaryCall< ::partition_oram::InitOramRequest, ::google::protobuf::Empty, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_init_oram_, context, request, response);
}

void server::Stub::async::init_oram(::grpc::ClientContext* context, const ::partition_oram::InitOramRequest* request, ::google::protobuf::Empty* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::partition_oram::InitOramRequest, ::google::protobuf::Empty, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_init_oram_, context, request, response, std::move(f));
}

void server::Stub::async::init_oram(::grpc::ClientContext* context, const ::partition_oram::InitOramRequest* request, ::google::protobuf::Empty* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_init_oram_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::google::protobuf::Empty>* server::Stub::PrepareAsyncinit_oramRaw(::grpc::ClientContext* context, const ::partition_oram::InitOramRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::google::protobuf::Empty, ::partition_oram::InitOramRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_init_oram_, context, request);
}

::grpc::ClientAsyncResponseReader< ::google::protobuf::Empty>* server::Stub::Asyncinit_oramRaw(::grpc::ClientContext* context, const ::partition_oram::InitOramRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncinit_oramRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status server::Stub::read_path(::grpc::ClientContext* context, const ::partition_oram::ReadPathRequest& request, ::partition_oram::ReadPathResponse* response) {
  return ::grpc::internal::BlockingUnaryCall< ::partition_oram::ReadPathRequest, ::partition_oram::ReadPathResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_read_path_, context, request, response);
}

void server::Stub::async::read_path(::grpc::ClientContext* context, const ::partition_oram::ReadPathRequest* request, ::partition_oram::ReadPathResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::partition_oram::ReadPathRequest, ::partition_oram::ReadPathResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_read_path_, context, request, response, std::move(f));
}

void server::Stub::async::read_path(::grpc::ClientContext* context, const ::partition_oram::ReadPathRequest* request, ::partition_oram::ReadPathResponse* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_read_path_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::partition_oram::ReadPathResponse>* server::Stub::PrepareAsyncread_pathRaw(::grpc::ClientContext* context, const ::partition_oram::ReadPathRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::partition_oram::ReadPathResponse, ::partition_oram::ReadPathRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_read_path_, context, request);
}

::grpc::ClientAsyncResponseReader< ::partition_oram::ReadPathResponse>* server::Stub::Asyncread_pathRaw(::grpc::ClientContext* context, const ::partition_oram::ReadPathRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncread_pathRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status server::Stub::write_path(::grpc::ClientContext* context, const ::partition_oram::WritePathRequest& request, ::partition_oram::WritePathResponse* response) {
  return ::grpc::internal::BlockingUnaryCall< ::partition_oram::WritePathRequest, ::partition_oram::WritePathResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_write_path_, context, request, response);
}

void server::Stub::async::write_path(::grpc::ClientContext* context, const ::partition_oram::WritePathRequest* request, ::partition_oram::WritePathResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::partition_oram::WritePathRequest, ::partition_oram::WritePathResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_write_path_, context, request, response, std::move(f));
}

void server::Stub::async::write_path(::grpc::ClientContext* context, const ::partition_oram::WritePathRequest* request, ::partition_oram::WritePathResponse* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_write_path_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::partition_oram::WritePathResponse>* server::Stub::PrepareAsyncwrite_pathRaw(::grpc::ClientContext* context, const ::partition_oram::WritePathRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::partition_oram::WritePathResponse, ::partition_oram::WritePathRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_write_path_, context, request);
}

::grpc::ClientAsyncResponseReader< ::partition_oram::WritePathResponse>* server::Stub::Asyncwrite_pathRaw(::grpc::ClientContext* context, const ::partition_oram::WritePathRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncwrite_pathRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status server::Stub::close_connection(::grpc::ClientContext* context, const ::google::protobuf::Empty& request, ::google::protobuf::Empty* response) {
  return ::grpc::internal::BlockingUnaryCall< ::google::protobuf::Empty, ::google::protobuf::Empty, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_close_connection_, context, request, response);
}

void server::Stub::async::close_connection(::grpc::ClientContext* context, const ::google::protobuf::Empty* request, ::google::protobuf::Empty* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::google::protobuf::Empty, ::google::protobuf::Empty, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_close_connection_, context, request, response, std::move(f));
}

void server::Stub::async::close_connection(::grpc::ClientContext* context, const ::google::protobuf::Empty* request, ::google::protobuf::Empty* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_close_connection_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::google::protobuf::Empty>* server::Stub::PrepareAsyncclose_connectionRaw(::grpc::ClientContext* context, const ::google::protobuf::Empty& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::google::protobuf::Empty, ::google::protobuf::Empty, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_close_connection_, context, request);
}

::grpc::ClientAsyncResponseReader< ::google::protobuf::Empty>* server::Stub::Asyncclose_connectionRaw(::grpc::ClientContext* context, const ::google::protobuf::Empty& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncclose_connectionRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status server::Stub::key_exchange(::grpc::ClientContext* context, const ::partition_oram::KeyExchangeRequest& request, ::partition_oram::KeyExchangeResponse* response) {
  return ::grpc::internal::BlockingUnaryCall< ::partition_oram::KeyExchangeRequest, ::partition_oram::KeyExchangeResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_key_exchange_, context, request, response);
}

void server::Stub::async::key_exchange(::grpc::ClientContext* context, const ::partition_oram::KeyExchangeRequest* request, ::partition_oram::KeyExchangeResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::partition_oram::KeyExchangeRequest, ::partition_oram::KeyExchangeResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_key_exchange_, context, request, response, std::move(f));
}

void server::Stub::async::key_exchange(::grpc::ClientContext* context, const ::partition_oram::KeyExchangeRequest* request, ::partition_oram::KeyExchangeResponse* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_key_exchange_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::partition_oram::KeyExchangeResponse>* server::Stub::PrepareAsynckey_exchangeRaw(::grpc::ClientContext* context, const ::partition_oram::KeyExchangeRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::partition_oram::KeyExchangeResponse, ::partition_oram::KeyExchangeRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_key_exchange_, context, request);
}

::grpc::ClientAsyncResponseReader< ::partition_oram::KeyExchangeResponse>* server::Stub::Asynckey_exchangeRaw(::grpc::ClientContext* context, const ::partition_oram::KeyExchangeRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsynckey_exchangeRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status server::Stub::send_hello(::grpc::ClientContext* context, const ::partition_oram::HelloMessage& request, ::google::protobuf::Empty* response) {
  return ::grpc::internal::BlockingUnaryCall< ::partition_oram::HelloMessage, ::google::protobuf::Empty, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_send_hello_, context, request, response);
}

void server::Stub::async::send_hello(::grpc::ClientContext* context, const ::partition_oram::HelloMessage* request, ::google::protobuf::Empty* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::partition_oram::HelloMessage, ::google::protobuf::Empty, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_send_hello_, context, request, response, std::move(f));
}

void server::Stub::async::send_hello(::grpc::ClientContext* context, const ::partition_oram::HelloMessage* request, ::google::protobuf::Empty* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_send_hello_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::google::protobuf::Empty>* server::Stub::PrepareAsyncsend_helloRaw(::grpc::ClientContext* context, const ::partition_oram::HelloMessage& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::google::protobuf::Empty, ::partition_oram::HelloMessage, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_send_hello_, context, request);
}

::grpc::ClientAsyncResponseReader< ::google::protobuf::Empty>* server::Stub::Asyncsend_helloRaw(::grpc::ClientContext* context, const ::partition_oram::HelloMessage& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncsend_helloRaw(context, request, cq);
  result->StartCall();
  return result;
}

server::Service::Service() {
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      server_method_names[0],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< server::Service, ::partition_oram::InitOramRequest, ::google::protobuf::Empty, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](server::Service* service,
             ::grpc::ServerContext* ctx,
             const ::partition_oram::InitOramRequest* req,
             ::google::protobuf::Empty* resp) {
               return service->init_oram(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      server_method_names[1],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< server::Service, ::partition_oram::ReadPathRequest, ::partition_oram::ReadPathResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](server::Service* service,
             ::grpc::ServerContext* ctx,
             const ::partition_oram::ReadPathRequest* req,
             ::partition_oram::ReadPathResponse* resp) {
               return service->read_path(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      server_method_names[2],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< server::Service, ::partition_oram::WritePathRequest, ::partition_oram::WritePathResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](server::Service* service,
             ::grpc::ServerContext* ctx,
             const ::partition_oram::WritePathRequest* req,
             ::partition_oram::WritePathResponse* resp) {
               return service->write_path(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      server_method_names[3],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< server::Service, ::google::protobuf::Empty, ::google::protobuf::Empty, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](server::Service* service,
             ::grpc::ServerContext* ctx,
             const ::google::protobuf::Empty* req,
             ::google::protobuf::Empty* resp) {
               return service->close_connection(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      server_method_names[4],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< server::Service, ::partition_oram::KeyExchangeRequest, ::partition_oram::KeyExchangeResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](server::Service* service,
             ::grpc::ServerContext* ctx,
             const ::partition_oram::KeyExchangeRequest* req,
             ::partition_oram::KeyExchangeResponse* resp) {
               return service->key_exchange(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      server_method_names[5],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< server::Service, ::partition_oram::HelloMessage, ::google::protobuf::Empty, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](server::Service* service,
             ::grpc::ServerContext* ctx,
             const ::partition_oram::HelloMessage* req,
             ::google::protobuf::Empty* resp) {
               return service->send_hello(ctx, req, resp);
             }, this)));
}

server::Service::~Service() {
}

::grpc::Status server::Service::init_oram(::grpc::ServerContext* context, const ::partition_oram::InitOramRequest* request, ::google::protobuf::Empty* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status server::Service::read_path(::grpc::ServerContext* context, const ::partition_oram::ReadPathRequest* request, ::partition_oram::ReadPathResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status server::Service::write_path(::grpc::ServerContext* context, const ::partition_oram::WritePathRequest* request, ::partition_oram::WritePathResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status server::Service::close_connection(::grpc::ServerContext* context, const ::google::protobuf::Empty* request, ::google::protobuf::Empty* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status server::Service::key_exchange(::grpc::ServerContext* context, const ::partition_oram::KeyExchangeRequest* request, ::partition_oram::KeyExchangeResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status server::Service::send_hello(::grpc::ServerContext* context, const ::partition_oram::HelloMessage* request, ::google::protobuf::Empty* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}


}  // namespace partition_oram

