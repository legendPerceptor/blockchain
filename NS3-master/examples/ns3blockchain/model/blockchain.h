//
// Created by Hython on 2020/2/20.
//

#ifndef NURAFT_BLOCKCHAIN_H
#define NURAFT_BLOCKCHAIN_H

#include <vector>
#include <map>
#include <memory>
#include <functional>
#include <optional>
#include "ns3/ipv4-address.h"


// ns3blockchain模块是第三个实验模块，之前已经修改了EBay的Raft算法
// 并将NS-3用CMAKE完成编译后，调整放入Raft子模块中
// 在完成了Raft和简易区块链的命令行展示程序后，尝试使用NS-3模拟区块链
// 这里将使用C++ 17进行编写，尽可能规避裸指针等会引起内存泄漏的问题
// 此处区块链参考比特币的区块链的设计，旨在用NS3进行模拟，区块链细节可以根据需要修改
namespace ns3::blockchain {
    using ns3::Ipv4Address;
    enum class Messages {
        INV = 0,
        REQUEST_TRANS,
        GET_HEADERS,
        HEADERS,
        GET_DATA,
        BLOCK,
        NO_MESSAGE,
        REPLY_TRANS,
        MSG_TRANS,
        RESULT_TRANS,
    };

    enum class MinerType {
        NORMAL_MINER,
        HYPERLEDGER_MINER,
    };

    enum class CommitterType{
        COMMITTER,
        ENDORSER,
        CLIENT,
        ORDER,
    };

    enum class ProtocolType{ //方便以后调整使用不同的协议
        STANDARD_PROTOCOL,
        SENDHEADERS,
    };

    enum class Cryptocurrency {
        ETHEREUM,
        HYPERLEDGER
    };

    enum class BlockchainRegion {
        NORTH_AMERICA,
        EUROPE,
        SOUTH_AMERICA,
        KOREA,
        JAPAN,
        AUSTRALIA,
        CHINA,
        OTHER,
    };

    struct nodeStatistics {
        int     nodeId;                         // blockchain node ID
        double  meanBlockReceiveTime;        // average of Block receive time
        double  meanBlockPropagationTime;    // average of Block propagation time
        double  meanBlockSize;               // average of Block Size
        int     totalBlocks;
        int     miner;                          //0-> node, 1->miner
        int     minerGeneratedBlocks;
        double  minerAverageBlockGenInterval;
        double  minerAverageBlockSize;
        double  hashRate;
        long    invReceivedBytes;
        long    invSentBytes;
        long    getHeadersReceivedBytes;
        long    getHeadersSentBytes;
        long    headersReceivedBytes;
        long    headersSentBytes;
        long    getDataReceivedBytes;
        long    getDataSentBytes;
        long    blockReceivedBytes;
        long    blockSentBytes;
        int     longestFork;
        int     blocksInForks;
        int     connections;
        int     minedBlocksInMainChain;
        long    blockTimeouts;
        int     nodeGeneratedTransaction;
        double  meanEndorsementTime;
        double  meanOrderingTime;
        double  meanValidationTime;
        double  meanLatency;
        int     nodeType;
        double  meanNumberofTransactions;
    };

    struct nodeInternetSpeed {
        double downloadSpeed;
        double uploadSpeed;
    };

    const char* getMessageName(Messages m);
    const char* getMinerType(MinerType m);
    const char* getCommitterType(CommitterType m);
    const char* getProtocolType(ProtocolType m);
    const char* getCryptocurrency(Cryptocurrency m);
    const char* getBlockchainRegion(BlockchainRegion m);
    BlockchainRegion getBlockchainEnum(uint32_t n);

    class Transaction {
    public:

        Transaction(int nodeId, int transId, double timeStamp);
        Transaction();
        virtual ~Transaction(void);

        int GetTransNodeId(void) const;
        void SetTransNodeId(int nodeId);

        int GetTransId(void) const;
        void SetTransId(int transId);

        int GetTransSizeByte(void) const;
        void SetTransSizeByte(int transSizeByte);

        double GetTransTimeStamp(void) const;
        void SetTransTimeStamp(double timeStamp);

        bool IsValidated(void) const;
        void SetValidation();

        int GetExecution(void) const;
        void SetExecution(int endoerserId);

        Transaction& operator = (const Transaction &tranSource);     //Assignment Constructor

        friend bool operator == (const Transaction &tran1, const Transaction &tran2);


    protected:
        int m_nodeId;   //指定交易节点
        int m_transId;  //唯一指定交易
        int m_transSizeByte;//交易大小
        double m_timeStamp; //交易时间戳
        bool m_validation; //交易认证
        int m_execution;   //交易是否执行

    };

    class Block {
    public:
        Block(int blockHeight, int minerId, int nonce, int parentBlockMinerId, int blockSizeBytes,
              double timeStamp, double timeReceived, Ipv4Address receivedFromIpv4);
        Block();
        Block(const Block &blockSource);
        virtual ~Block(void);

        int GetBlockHeight(void) const;
        void SetBlockHeight(int blockHeight);

        int GetNonce(void) const;
        void SetNonce(int nonce);

        int GetMinerId(void) const;
        void SetMinerId(int minerId);

        int GetParentBlockMinerId(void) const;
        void SetParentBlockMinerId(int parentBlockMinerId);

        int GetBlockSizeBytes(void) const;
        void SetBlockSizeBytes(int blockSizeBytes);

        double GetTimeStamp(void) const;
        void SetTimeStamp(double timeStamp);

        double GetTimeReceived(void) const;

        Ipv4Address GetReceivedFromIpv4(void) const;
        void SetReceivedFromIpv4(Ipv4Address receivedFromIpv4);

        std::vector<Transaction> GetTransactions(void) const;
        void SetTransactions(const std::vector<Transaction> &transactions);
        /*
        * Checks if the block provided as the argument is the parent of this block object
        */
        bool IsParent (const Block &block) const;

        /*
        * Checks if the block provided as the argument is the child of this block object
        */
        bool IsChild (const Block &block) const;

        int GetTotalTransaction(void) const;

        Transaction ReturnTransaction(int nodeId, int transId);

        bool HasTransaction(Transaction &newTran) const;

        bool HasTransaction(int nodeId, int tranId) const;

        void AddTransaction(Transaction& newTrans);
        void AddTransaction(Transaction&& newTrans);

        void PrintAllTransactions(void);

        Block& operator = (const Block &blockSource);     //拷贝构造函数用到

        friend bool operator == (const Block &block1, const Block &block2);


    protected:
        int         m_blockHeight;                  //区块的高度，越高越新
        int         m_minerId;                      //计算出此区块的矿机的ID
        int         m_nonce;                        //此区块的随机值，用于生成哈希值
        int         m_parentBlockMinerId;           //上一个区块的矿机的ID
        int         m_blockSizeBytes;               //区块的大小，bytes作单位
        int         m_totalTransactions;            //区块内含的交易数
        double      m_timeStamp;                 //区块创建时的时间戳
        double      m_timeReceived;              //节点接受到区块时的时间戳
        Ipv4Address m_receivedFromIpv4;       //发送该区块节点的IP
        std::vector<Transaction> m_transactions;
    };

    class Blockchain {
    public:
        Blockchain();
        virtual ~Blockchain();

        int GetTotalBlocks() const;

        int GetNoOrphans() const;

        int GetBlockchainHeight() const;

        bool HasBlock(const Block& newBlock) const;
        bool HasBlock(int height, int minerID) const;

        Block ReturnBlock(int height, int minerID);

        bool IsOrphan(const Block &newBlock) const;
        bool IsOrphan(int height, int minerId) const;

        [[nodiscard]] std::optional<const Block*> GetBlockPointer(const Block& newBlock) const;

        std::optional<std::vector<const Block*>> GetChildrenPointers(const Block& block);
        std::optional<std::vector<const Block*>> GetOrphanChildrenPointers(const Block& block);

        std::optional<const Block*> GetParent(const Block& block);

        [[nodiscard]] std::optional<const Block*> GetCurrentTopBlock() const;

        void AddBlock(const Block& newBlock);


        void AddOrphan(const Block& newBlock);


        void RemoveOrphan(const Block& newBlock);

    protected:
        int m_totalBlocks;  //总区块个数
        std::vector<Block> m_orphans; //未上链的孤立块
        std::vector<std::vector<Block>> m_blocks; //分为N层的区块链，每层单独一链
    };
}


#endif //NURAFT_BLOCKCHAIN_H
