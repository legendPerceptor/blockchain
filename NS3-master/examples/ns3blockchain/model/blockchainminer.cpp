//
// Created by Hython on 2020/2/29.
//
#include "ns3/address.h"
#include "ns3/address-utils.h"
#include "ns3/log.h"
#include "ns3/inet-socket-address.h"
#include "ns3/inet6-socket-address.h"
#include "ns3/node.h"
#include "ns3/socket.h"
#include "ns3/udp-socket.h"
#include "ns3/simulator.h"
#include "ns3/socket-factory.h"
#include "ns3/packet.h"
#include "ns3/trace-source-accessor.h"
#include "ns3/udp-socket-factory.h"
#include "ns3/tcp-socket-factory.h"
#include "ns3/uinteger.h"
#include "ns3/double.h"
#include "blockchainminer.h"
#include <fstream>
#include <time.h>
#include <sys/time.h>
#include <random>

static double GetWallTime();

namespace ns3::blockchain {

    TypeId BlockchainMiner::GetTypeId() {
        return TypeId();
    }

    BlockchainMiner::BlockchainMiner() {

    }

    BlockchainMiner::~BlockchainMiner() {

    }

    double BlockchainMiner::GetFixedBlockTimeGeneration() const {
        return 0;
    }

    void BlockchainMiner::SetFixedBlockTimeGeneration(double fixedBlockTimeGeneration) {

    }

    uint32_t BlockchainMiner::GetFixedBlockSize() const {
        return 0;
    }

    void BlockchainMiner::SetFixedBlockSize(uint32_t fixedBlockSize) const {

    }

    double BlockchainMiner::GetBlockGenBinSize() const {
        return 0;
    }

    void BlockchainMiner::SetBlockGenBinSize(double m_blockGenBinSize) {

    }

    double BlockchainMiner::GetBlockGenParameter() const {
        return 0;
    }

    void BlockchainMiner::SetBlockGenParameter(double blockGenParameter) {

    }

    double BlockchainMiner::GetHashRate() const {
        return 0;
    }

    void BlockchainMiner::SetHashRate(double blockGenParameter) {

    }

    void BlockchainMiner::StartApplication(void) {
        BlockchainNode::StartApplication();
    }

    void BlockchainMiner::StopApplication(void) {
        BlockchainNode::StopApplication();
    }

    void BlockchainMiner::DoDispose(void) {
        BlockchainNode::DoDispose();
    }

    void BlockchainMiner::ScheduleNextMiningEvent(void) {

    }

    void BlockchainMiner::MineBlock(void) {

    }

    void BlockchainMiner::ReceivedHigherBlock(const Block &newBlock) {
        BlockchainNode::ReceivedHigherBlock(newBlock);
    }

    void BlockchainMiner::SendBlock(std::string packetInfo, Ptr<Socket> socket) {

    }
}