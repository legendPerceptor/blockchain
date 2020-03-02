//
// Created by Hython on 2020/2/29.
//

#ifndef NURAFT_BLOCKCHAINMINER_H
#define NURAFT_BLOCKCHAINMINER_H

#include "blockchainnode.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include <random>

namespace ns3::blockchain {
    using ns3::Address;
    using ns3::Socket;
    using ns3::Packet;

    class BlockchainMiner: public BlockchainNode {
    public:
        static TypeId GetTypeId();

        BlockchainMiner();

        virtual ~BlockchainMiner();

        double GetFixedBlockTimeGeneration() const;

        void SetFixedBlockTimeGeneration(double fixedBlockTimeGeneration);

        uint32_t GetFixedBlockSize() const;

        void SetFixedBlockSize(uint32_t fixedBlockSize) const;

        double GetBlockGenBinSize() const;

        void SetBlockGenBinSize(double m_blockGenBinSize);

        double GetBlockGenParameter() const;

        void SetBlockGenParameter(double blockGenParameter);

        double GetHashRate() const;

        void SetHashRate(double blockGenParameter);

    protected:
        virtual void StartApplication(void);
        virtual void StopApplication(void);

        virtual void DoDispose(void);

        void ScheduleNextMiningEvent (void);

        virtual void MineBlock(void);

        virtual void ReceivedHigherBlock(const Block &newBlock);

        void SendBlock(std::string packetInfo, Ptr<Socket> socket);

        int                         m_noMiners;
        uint32_t                    m_fixedBlockSize;
        double                      m_fixedBlockTimeGeneration;
        EventId                     m_nextMiningEvent;
        std::default_random_engine  m_generator;

        double                      m_blockGenBinSize;
        double                      m_blockGenParameter;
        double                      m_nextBlockTime;
        double                      m_previousBlockGenerationTime;
        double                      m_minerAverageBlockGenInterval;
        int                         m_minerGeneratedBlocks;
        double                      m_hashRate;
        double                      m_meanNumberofTransactions;

        std::geometric_distribution<int>    m_blockGenTimeDistribution;

        int                         m_nextBlockSize;
        int                         m_maxBlockSize;
        double                      m_minerAverageBlockSize;


        const double                m_realAverageBlockGenIntervalSeconds;   // in seconds, 15 sec(Ethereum)
        double                      m_averageBlockGenIntervalSeconds;


        Cryptocurrency  m_cryptocurrency;

        double  m_timeStart;
        double  m_timeFinish;
        bool    m_fistToMine;


    };
}


#endif //NURAFT_BLOCKCHAINMINER_H
