/*
 Copyright (c) 2022 Haobin Chen

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#include "oram_server.h"

#include <atomic>
#include <thread>

#include <spdlog/fmt/bin_to_hex.h>

#include "base/oram_utils.h"

std::atomic_bool server_running;

namespace partition_oram {
grpc::Status PartitionORAMService::init_oram(grpc::ServerContext* context,
                                             const InitOramRequest* request,
                                             google::protobuf::Empty* empty) {
  // TODO: implement me.
  return grpc::Status::OK;
}

grpc::Status PartitionORAMService::read_path(grpc::ServerContext* context,
                                             const ReadPathRequest* request,
                                             ReadPathResponse* response) {
  // TODO: implement me.
  return grpc::Status::OK;
}

grpc::Status PartitionORAMService::write_path(grpc::ServerContext* context,
                                              const WritePathRequest* request,
                                              WritePathResponse* response) {
  // TODO: implement me.
  return grpc::Status::OK;
}

grpc::Status PartitionORAMService::key_exchange(
    grpc::ServerContext* context, const KeyExchangeRequest* request,
    KeyExchangeResponse* response) {
  const std::string public_key_client = request->public_key_client();

  Status status;
  if ((status = cryptor_->sample_key_pair()) != Status::OK) {
    const std::string error_message = oram_utils::string_concat(
        "Failed to sample key pair! Error: ", error_list.at(status));
    return grpc::Status(grpc::StatusCode::INTERNAL, error_message);
  }

  if ((status = cryptor_->sample_session_key(public_key_client, 1)) !=
      Status::OK) {
    const std::string error_message = oram_utils::string_concat(
        "Failed to sample session key! Error: ", error_list.at(status));
    return grpc::Status(grpc::StatusCode::INTERNAL, error_message);
  }
  auto key_pair = cryptor_->get_key_pair();
  response->set_public_key_server(key_pair.first);

  auto session_key = std::move(cryptor_->get_session_key_pair());
  logger->info("The session key for receiving is {}.",
               spdlog::to_hex(session_key.first));
  logger->info("The session key for sending is {}.",
               spdlog::to_hex(session_key.second));

  return grpc::Status::OK;
}

grpc::Status PartitionORAMService::close_connection(
    grpc::ServerContext* context, const google::protobuf::Empty* request,
    google::protobuf::Empty* response) {
  logger->info("Closing connection...");
  server_running = false;
  return grpc::Status::OK;
}

grpc::Status PartitionORAMService::send_hello(grpc::ServerContext* context,
                                              const HelloMessage* request,
                                              google::protobuf::Empty* empty) {
  const std::string encrypted_message = request->content();
  const std::string iv = request->iv();
  std::string message;
  Status status;

  logger->info("Received encrypted message: {}.",
               spdlog::to_hex(encrypted_message));

  if ((status = cryptor_->decrypt((uint8_t*)encrypted_message.data(),
                                  encrypted_message.size(), (uint8_t*)iv.data(),
                                  &message)) != Status::OK) {
    const std::string error_message = oram_utils::string_concat(
        "Failed to verify Hello message! Error: ", error_list.at(status));
    return grpc::Status(grpc::StatusCode::INTERNAL, error_message);
  }

  logger->info("Successfully verified: {}.", message);
  return grpc::Status::OK;
}

ServerRunner::ServerRunner(const std::string& address, const std::string& port,
                           const std::string& key_path,
                           const std::string& crt_path)
    : address_(address), port_(port) {
  const std::string key_file = oram_utils::read_key_crt_file(key_path);
  const std::string crt_file = oram_utils::read_key_crt_file(crt_path);

  if (key_file.empty() || crt_file.empty()) {
    abort();
  }

  // Start to configure the SSL options.
  grpc::SslServerCredentialsOptions::PemKeyCertPair pkcp;
  pkcp.private_key = key_file;
  pkcp.cert_chain = crt_file;

  // Self-signed. NO CA.
  grpc::SslServerCredentialsOptions ssl_opts;
  ssl_opts.pem_root_certs = "";
  ssl_opts.pem_key_cert_pairs.emplace_back(pkcp);
  creds_ = grpc::SslServerCredentials(ssl_opts);

  service_ = std::make_unique<PartitionORAMService>();
  is_initialized = true;
}

void ServerRunner::run(void) {
  logger->info("Starting server...");

  if (!is_initialized) {
    logger->error("Server not initialized.");
    abort();
  }

  grpc::ServerBuilder builder;
  const std::string address =
      std::move(oram_utils::string_concat(address_, ":", port_));
  builder.AddListeningPort(address, creds_);
  builder.RegisterService(service_.get());

  std::shared_ptr<grpc::Server> server = builder.BuildAndStart();
  logger->info("Server started to listen on {}:{}.", address_, port_);
  server_running = true;

  // Initialize the cryptor.
  service_->cryptor_ = oram_crypto::Cryptor::get_instance();

  // Start a monitor thread.
  std::thread monitor_thread([&, this]() {
    while (server_running) {
      // Wake up every 100 miliseconds and check if the server is still
      // running.
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    server->Shutdown();
  });
  monitor_thread.detach();

  server->Wait();
}
}  // namespace partition_oram