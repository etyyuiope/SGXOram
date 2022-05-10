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
#include "oram_controller.h"

#include <algorithm>
#include <cmath>

#include <spdlog/spdlog.h>
#include <spdlog/fmt/bin_to_hex.h>

#include "base/oram_crypto.h"
#include "base/oram_utils.h"

extern std::shared_ptr<spdlog::logger> logger;

namespace partition_oram {
// The ownership of ORAM main controller cannot be multiple.
// This cannot be shared.
std::unique_ptr<OramController> OramController::GetInstance() {
  std::unique_ptr<OramController> instance(new OramController());
  return instance;
}

PathOramController::PathOramController(uint32_t id, uint32_t block_num,
                                       uint32_t bucket_size)
    : id_(id), bucket_size_(bucket_size) {
  const size_t bucket_num = std::ceil(block_num * 1.0 / bucket_size);
  // Note that the level starts from 0.
  tree_level_ = std::ceil(LOG_BASE(bucket_num + 1, 2)) - 1;
  number_of_leafs_ = POW2(tree_level_);

  logger->info(
      "PathORAM Config:\n"
      "id: {}, number_of_leafs: {}, bucket_size: {}, tree_height: {}\n",
      id_, number_of_leafs_, bucket_size_, tree_level_);
}

Status PathOramController::PrintOramTree(void) {
  grpc::ClientContext context;
  PrintOramTreeRequest request;
  google::protobuf::Empty empty;

  request.set_id(id_);
  grpc::Status status = stub_->PrintOramTree(&context, request, &empty);

  if (!status.ok()) {
    logger->error("PrintOramTree failed: {}", status.error_message());
    return Status::kServerError;
  }

  return Status::kOK;
}

Status PathOramController::AccurateWriteBucket(uint32_t level, uint32_t offset,
                                               const p_oram_bucket_t& bucket) {
  grpc::ClientContext context;
  WritePathRequest request;
  WritePathResponse response;

  request.set_id(id_);
  request.set_level(level);
  request.set_offset(offset);
  request.set_type(Type::kInit);

  // Copy the buckets into the buffer of WriteBucketRequest.
  for (const auto& block : bucket) {
    std::string block_str;
    oram_utils::ConvertToString(&block, &block_str);
    request.add_bucket(block_str);
  }

  grpc::Status status = stub_->WritePath(&context, request, &response);

  if (!status.ok()) {
    return Status::kServerError;
  }

  return Status::kOK;
}

Status PathOramController::InitOram(void) {
  grpc::ClientContext context;
  InitOramRequest request;
  google::protobuf::Empty empty;

  request.set_id(id_);
  request.set_bucket_size(bucket_size_);
  request.set_bucket_num(number_of_leafs_);

  grpc::Status status = stub_->InitOram(&context, request, &empty);
  if (!status.ok()) {
    logger->error("InitOram failed: {}", status.error_message());
    return Status::kServerError;
  }

  return Status::kOK;
}

// The input vector of blocks should be sorted by their path id.
Status PathOramController::FillWithData(const std::vector<oram_block_t>& data) {
  // We organize all the data into buckets and then directly write them to the
  // server by invoking the WritePath method provided by the gRPC framework.
  // The data are organized level by level, and for best performance, we
  // initialize the ORAM tree from the leaf to the root. In other words, we
  // **GREEDILY** fill the buckets from the leaf to the root.

  // TODO: Implement me.
  // TODO: Initialize the tree while filling the position map.
  size_t p_data = 0;
  for (int i = tree_level_; i >= 0; i--) {
    // We pick bucket_size blocks from the data and organize them into a bucket.
    // The bucket is then sent to the server.
    const uint32_t level_size = POW2(i);
    const uint32_t span = POW2(tree_level_ - i);

    for (uint32_t j = 0; j < level_size; j++) {
      p_oram_bucket_t bucket_this_level;

      // This determined the range of the current bucket in terms of path.
      const uint32_t begin = j * span;
      const uint32_t end = begin + span - 1;

      // Organize into a bucket.
      for (size_t k = 0; k < bucket_size_; k++) {
        if (p_data >= data.size()) {
          break;
        }

        // Sample a random path for the block.
        uint32_t path;
        oram_utils::CheckStatus(
            oram_crypto::Cryptor::UniformRandom(begin, end, &path),
            "UniformRandom error");
        logger->debug("Path = {} for range [{}, {}]", path, begin, end);
        bucket_this_level.emplace_back(data[p_data]);
        // Update the position map.
        position_map_[data[p_data].header.block_id] = path;
        logger->debug("Block {} is stored at path {}",
                      data[p_data].header.block_id, path);
        p_data++;
      }

      oram_utils::PadStash(&bucket_this_level, bucket_size_);

      // Write the bucket to the server.
      oram_utils::CheckStatus(
          AccurateWriteBucket(i, j, bucket_this_level),
          "Failed to write bucket accurately when intializing the ORAM!");
    }
  }

  // Print the oram tree on the server side.
  grpc::ClientContext context;
  PrintOramTreeRequest request;
  google::protobuf::Empty empty;

  request.set_id(id_);

  // grpc::Status status = stub_->PrintOramTree(&context, request, &empty);
  // if (!status.ok()) {
  //   logger->error("PrintOramTree failed: {}", status.error_message());
  //   return Status::kServerError;
  // }

  return Status::kOK;
}

Status PathOramController::ReadBucket(uint32_t path, uint32_t level,
                                      p_oram_bucket_t* const bucket) {
  if (path >= number_of_leafs_ || level > tree_level_) {
    return Status::kInvalidArgument;
  }

  grpc::ClientContext context;

  // Then prepare for RPC call.
  ReadPathRequest request;
  ReadPathResponse response;
  request.set_path(path);
  request.set_level(level);
  grpc::Status status = stub_->ReadPath(&context, request, &response);

  if (!status.ok()) {
    return Status::kServerError;
  }

  const size_t bucket_size = response.bucket_size();
  // Then copy the bucket to the vector.
  for (size_t j = 0; j < bucket_size; j++) {
    oram_block_t* const block = (oram_block_t*)malloc(ORAM_BLOCK_SIZE);
    oram_utils::ConvertToBlock(response.bucket(j), block);
    bucket->emplace_back(*block);
    oram_utils::SafeFree(block);
  }

  return Status::kOK;
}

Status PathOramController::WriteBucket(uint32_t path, uint32_t level,
                                       const p_oram_bucket_t& bucket) {
  logger->info("WriteBucket: path: {}, level: {}", path, level);
  grpc::ClientContext context;
  WritePathRequest request;
  WritePathResponse response;

  request.set_path(path);
  request.set_level(level);
  request.set_type(Type::kNormal);

  // Copy the buckets into the buffer of WriteBucketRequest.
  for (const auto& block : bucket) {
    std::string block_str;
    oram_utils::ConvertToString(&block, &block_str);
    request.add_bucket(block_str);
  }

  grpc::Status status = stub_->WritePath(&context, request, &response);

  if (!status.ok()) {
    return Status::kServerError;
  }

  return Status::kOK;
}

p_oram_stash_t PathOramController::FindSubsetOf(uint32_t current_path) {
  p_oram_stash_t subset;

  auto iter = stash_->begin();
  while (iter != stash_->end()) {
    const uint32_t block_path = position_map_[iter->header.block_id];
    if (subset.size() < bucket_size_) {
      if (block_path == current_path) {
        subset.emplace_back(*iter);
        // Delete the current block and re-adjust the iterator.
        iter = stash_->erase(iter);
      } else {
        iter++;
      }
    } else {
      break;
    }
  }

  oram_utils::PadStash(&subset, bucket_size_);
  return subset;
}

Status PathOramController::Access(Operation op_type, uint32_t address,
                                  uint8_t* const data) {
  // First we do a sanity check.
  PANIC_IF(op_type == Operation::kInvalid, "Invalid operation.");

  // First, we sample a random new path for this block.
  uint32_t new_path;
  Status status =
      oram_crypto::Cryptor::UniformRandom(0, number_of_leafs_ - 1, &new_path);
  oram_utils::CheckStatus(status, "Failed to sample a new path.");

  // Next, we shall the get the real path of the current block.
  // @ref Stefanov's paper for full details.
  // Steps: 1-2
  // Randomly remap the position of block a to a new random position.
  // Let x denote the block’s old position.
  const uint32_t x = position_map_[address];
  logger->info("The address {} has the path {}.", address, x);
  // Update the position map.
  position_map_[address] = new_path;

  // Step 3-5: Read the whole path from the server into the stash.
  p_oram_path_t bucket_this_path;

  for (size_t i = 0; i <= tree_level_; i++) {
    p_oram_bucket_t bucket_this_level;
    Status status = ReadBucket(x, i, &bucket_this_level);
    oram_utils::CheckStatus(status,
                            oram_utils::StrCat("Failed to read bucket: ", x));
    bucket_this_path.emplace_back(bucket_this_level);
  }

  // Read all the blocks into the stash.
  for (size_t i = 0; i <= tree_level_; i++) {
    for (size_t j = 0; j < bucket_this_path[i].size(); j++) {
      oram_block_t block = bucket_this_path[i][j];

      // Check if the block is already in the stash.
      // If there is no such block, we add it to the stash.
      //
      // <=> S = S ∪ ReadBucket(P(x, l))
      auto iter = std::find_if(stash_->begin(), stash_->end(),
                               BlockEqual(block.header.block_id));
      if (iter == stash_->end() && block.header.type == BlockType::kNormal) {
        stash_->emplace_back(block);
      }
    }
  }

  // Step 6-9: Update block, if any.
  // If the access is a write, update the data stored for block a.
  oram_utils::PrintStash(*stash_);
  auto iter = std::find_if(stash_->begin(), stash_->end(), BlockEqual(address));
  PANIC_IF(iter == stash_->end(), "Failed to find the block in the stash.");

  // Update the block.
  if (op_type == Operation::kWrite) {
    memcpy(iter->data, data, ORAM_BLOCK_SIZE);
  } else {
    memcpy(data, iter->data, ORAM_BLOCK_SIZE);
  }

  // STEP 10-15: Write the path.
  //
  // Write the path back and possibly include some additional blocks from the
  // stash if they can be placed into the path. Buckets are greedily filled
  // with blocks in the stash in the order of leaf to root, ensuring that
  // blocks get pushed as deep down into the tree as possible. A block a? can
  // be placed in the bucket at level ? only if the path P(position[a']) to
  // the leaf of block a' intersects the path accessed P(x) at level l. In
  // other words, if P(x, l) = P(position[a'], l).

  // Prevent overflow for unsigned variable...
  for (size_t i = tree_level_ + 1; i >= 1; i--) {
    // Find a subset S' of stash such that the element in S' intersects with
    // the current old path of x. I.e., S' ← {(a', data') \in S : P(x, l) =
    // P(position[a'], l)} Select min(|S'|, Z) blocks. If |S'| < Z, then we
    // pad S' with dummy blocks. Expire all blocks in S that are in S'. Write
    // them back.
    p_oram_stash_t subset = std::move(FindSubsetOf(x));
    Status status = WriteBucket(x, i - 1, subset);
    oram_utils::CheckStatus(status, "Failed to write bucket.");
  }

  return Status::kOK;
}

Status OramController::Access(Operation op_type, uint32_t address,
                              oram_block_t* const data) {
  return Status::kOK;
}

Status OramController::Evict(EvictType evict_type) {
  // TODO: implement me.
  return Status::kOK;
}

Status OramController::Run(uint32_t block_num, uint32_t bucket_size) {
  logger->info("The Partition Oram Controller is running...");

  // Determine the size of each sub-ORAM and the number of slot number.
  const size_t squared = std::ceil(std::sqrt(block_num));
  partition_size_ = std::ceil(squared * (1 + kPartitionAdjustmentFactor));

  // Initialize all the slots.
  for (size_t i = 0; i < squared; i++) {
    p_oram_stash_t stash;
    stash.resize(partition_size_);
    slots_.emplace_back(stash);
  }

  for (size_t i = 0; i < squared; i++) {
    // We create the PathORAM controller for each slot.
    path_oram_controllers_.emplace_back(
        std::make_unique<PathOramController>(i, partition_size_, bucket_size));
    path_oram_controllers_.back()->SetStub(stub_);
    path_oram_controllers_.back()->SetStash(&slots_[i]);

    // Then invoke the intialization procedure.
    Status status = path_oram_controllers_.back()->InitOram();
    if (status != Status::kOK) {
      return status;
    }
  }

  return Status::kOK;
}

Status OramController::TestPathOram(uint32_t controller_id) {
  if (controller_id >= path_oram_controllers_.size()) {
    logger->error("The controller id is out of range.");
    return Status::kOutOfRange;
  }

  PathOramController* const controller =
      path_oram_controllers_[controller_id].get();

  const size_t level = controller->GetTreeLevel();
  const size_t tree_size = (POW2(level + 1) - 1) * bucket_size_;
  controller->FillWithData(
      std::move(oram_utils::SampleRandomBucket(partition_size_, tree_size)));
  controller->PrintOramTree();

  uint8_t* const data = (uint8_t*)malloc(DEFAULT_ORAM_DATA_SIZE);
  memset(data, 0, DEFAULT_ORAM_DATA_SIZE);

  for (size_t r = 0; r < 100; r++) {
    for (size_t i = 0; i < partition_size_; i++) {
      Status status = controller->Access(Operation::kRead, i, data);
      oram_utils::CheckStatus(status, "Failed to read block.");

      PANIC_IF((data[0] != i), "Failed to read the correct block.");
    }
  }

  oram_utils::SafeFree(data);
}

}  // namespace partition_oram