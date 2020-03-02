//
// Created by Hython on 2020/2/20.
//

#ifndef NURAFT_BLOCKCHAINNODE_H
#define NURAFT_BLOCKCHAINNODE_H

#include "ns3/application.h"
#include "ns3/event-id.h"
#include "ns3/ptr.h"
#include "ns3/traced-callback.h"
#include "ns3/ipv4-address.h"
#include "blockchain.h"
#include "ns3/boolean.h"
#include "ns3/socket.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

namespace ns3::blockchain {
    using ns3::Ipv4Address;
    using ns3::Socket;
    using ns3::Packet;
    using ns3::TypeId;
    using ns3::Application;
    //这里体现用CMAKE构建工程的好处
    //我们先编译NS-3的内部核心模块，在本文件用到时都是编译好的模块
    //而不是声明ns3中的类，无需和NS3一起编译链接，
    //如果不用CMAKE 可能需要如下声明
    //class Address; class Socket;
    //且CMAKE可以自由选择构建哪些模块、哪些实例，删去不需要的实例
    //自由开关各个选项，比如MPI选项 加快编译速度和模块化程度
    class BlockchainNode: public Application {
    public:
        static TypeId GetTypeId();
        BlockchainNode();
        virtual ~BlockchainNode();

        Ptr<Socket> GetListeningSocket() const;
        std::vector<Ipv4Address> GetPeerAddress() const;

        void SetPeersAddresses(const std::vector<Ipv4Address>& peers);

        void SetPeersDownloadSpeeds(const std::map<Ipv4Address, double>& peerDownloadSpeeds);

        void SetPeersUploadSpeeds(const std::map<Ipv4Address, double>& peerUploadSpeeds);

        void SetNodeInternetSpeeds(const nodeInternetSpeed& internetSpeed);

        void SetNodeStats(std::unique_ptr<nodeStatistics> nodeStats);

        void SetProtocolType(ProtocolType protocolType);

        void SetCommitterType(CommitterType cType);

        void SetCreatingTransactionTime(int cTime);

    protected:

        virtual void DoDispose(); // Derived from Application

        virtual void StartApplication();
        virtual void StopApplication();

        // Handle a packet received by the application
        // param socket: the receiving socket
        void HandleRead(Ptr<Socket> socket);

        // Handle an incoming connection
        // param socket: the incoming connection socket
        // param from: the address the connection is from
        void HandleAccept(Ptr<Socket> socket, const Address& from);

        // handle an connection close
        void HandlePeerClose(Ptr<Socket> socket);

        // handle an connection error
        void HandlePeerError(Ptr<Socket> socket);

        void ReceivedBlockMessage(std::string &blockInfo, Address &from);

        // Called when a new block non-orphan block is received
        virtual void ReceiveBlock(const Block& block);

        // Send a BLOCK message as a response to a GET_DATA message
        // PARAM packetInfo: the info of the BLOCK message
        // PARAM from: the address the GET_DATA was received from
        void SendBlock(const std::string& packetInfo, Address& from);

        virtual void ReceivedHigherBlock(const Block& newBlock);

        void ValidateBlock(const Block& newBlock);

        void ValidateTransaction(const Block& newBlock);

        // Add the new block into the blockchain, advertises it to the peers and validates any orphan children
        void AfterBlockValidation(const Block& newBlock);


        void ValidateOrphanChildren(const Block& newBlock);

        // Advertise the newly validated block
        // param newBlock: the new block
        void AdvertiseNewBlock(const Block& newBlock);

        void AdvertiseNewTransaction(const Transaction &newTrans, Messages megType, Ipv4Address receivedFromIpv4);

        bool HasTransaction(int nodeId, int transId);

        bool HasReplyTransaction(int nodeId, int transId, int transExecution);

        bool HasMessageTransaction(int nodeId, int transId);

        bool HasResultTransaction(int nodeId, int transId);

        bool HasTransactionAndValidated(int nodeId, int transId);

        void CreateTransaction();

        void ScheduleNextTransaction();

        void ExecuteTransaction(const Transaction &newTrans, Ipv4Address receivedFromIpv4);

        void NotifyTransaction(const Transaction &newTrans);


        /*
             * Send a message to a peer
             * param receivedMessage : the type of the received message
             * param responseMessage : the type of the response message
             * param d : rapidjson document containing the info of the outgoing message
             * param outgoingSocket : the socket of the peer
             */
        void SendMessage( Messages receivedMessage,
                Messages responseMessage,
                rapidjson::Document &d, Ptr<Socket> outgoingSocket);

        /*
             * Send a message to a peer
             * param receivedMessage : the type of the received message
             * param responseMessage : the type of the response message
             * param d : rapidjson document containing the info of the outgoing message
             * param outgoingAddress : the address of the peer
             */
        void SendMessage( Messages receivedMessage,
                          Messages responseMessage,
                          rapidjson::Document &d, Address& outgoingAddress);
        /*
             * Send a message to a peer
             * param receivedMessage : the type of the received message
             * param responseMessage : the type of the response message
             * param packet : string containing the info of the outgoing message
             * param outgoingAddress : the address of the peer
             */
        void SendMessage( Messages receivedMessage,
                          Messages responseMessage,
                          std::string packet, Address& outgoingAddress);


        // Called when a timeout for a block expires
        // param blockHash: the block for which the timeout expired
        void InvTimeoutExpired(const std::string& blockHash);

        /*
         * Checks if a block has been received but not been validated yet (if it is included in m_receivedNotValidated)
        * parm blockHash : the block hash
        * return true : if the block has been received but not validated yet, false : otherwise
        */
        bool ReceivedButNotValidated(std::string blockHash);

        /*
         * Removes a block from m_receivedNotValidated
         * param blockHash : the block hash
         */
        void RemoveReceivedButNotvalidated(std::string blockHash);

        /*
         * Checks if the node has recieved only the headers of a particular block(if it is included in m_onlyHeadersReceived)
         * parm blockHash : the block hash
         * return true : if only the block headers have been received, false : otherwise
         */
        bool OnlyHeadersReceived (const std::string& blockHash);

        /*
         * Remove the first element from m_sendBlockTimes, when a block is sent
         */
        void RemoveSendTime();


        /*
         * Remove the first element from m_sendCompressedBlockTimes, when a compressd-block is sent
         */
        void RemoveCompressedBlockSendTime();

        /*
         * Removes the first element from m_receivedBlockTimes, when block is received
         */
        void RemoveReceiveTime();

        /*
         * Removes the first element from m_receiveCompressedBlockTime, when a compressed-block is received
         */
        void RemoveCompressedBlockReceiveTime();



        Ptr<Socket>     m_socket;                       //Listening socket
        Address         m_local;                        //Local address to bind to
        TypeId          m_tid;                          //Protocol TypeID
        int             m_numberOfPeers;                //Number of node's peers
        double          m_meanBlockReceiveTime;         //The mean time interval between two consecutive blocks (10~15sec)
        double          m_previousBlockReceiveTime;     //The time that the node received the previous block
        double          m_meanBlockPropagationTime;     //The mean time that the node has to wait in order to receive a newly mined block
        double          m_meanEndorsementTime;
        double          m_meanOrderingTime;
        double          m_meanValidationTime;
        double          m_meanLatency;
        double          m_meanBlockSize;                //The mean Block size
        Blockchain      m_blockchain;                   //The node's blockchain
        Time            m_invTimeoutMinutes;
        bool            m_isMiner;                      //True if the node is a miner
        double          m_downloadSpeed;                // Bytes/s
        double          m_uploadSpeed;                  // Bytes/s
        double          m_averageTransacionSize;        //The average transaction size, Needed for compressed blocks
        int             m_transactionIndexSize;         //The transaction index size in bytes. Needed for compressed blocks
        int             m_transactionId;
        EventId         m_nextTransaction;
        int             m_numberofEndorsers;
        int             m_totalEndorsement;
        int             m_totalOrdering;
        int             m_totalValidation;
        int             m_totalCreatedTransaction;
        int             m_creatingTransactionTime;

        std::vector<Transaction>                        m_transaction;
        std::vector<Transaction>                        m_notValidatedTransaction;
        std::vector<Transaction>                        m_replyTransaction;
        std::vector<Transaction>                        m_msgTransaction;
        std::vector<Transaction>                        m_resultTransaction;
        std::vector<Transaction>                        m_waitingEndorsers;
        std::vector<Ipv4Address>                        m_peersAddresses;                   // The address of peers
        std::map<Ipv4Address, double>                   m_peersDownloadSpeeds;              // The peerDownloadSpeeds of channels
        std::map<Ipv4Address, double>                   m_peersUploadSpeeds;                // The peerUploadSpeeds of channels
        std::map<Ipv4Address, Ptr<Socket>>              m_peersSockets;                     // The sockets of peers
        std::map<std::string, std::vector<Address>>     m_queueInv;
        std::map<std::string, EventId>                  m_invTimeouts;
        std::map<Address, std::string>                  m_bufferedData;                     // map holding the buffered data from previous handleRead events
        std::map<std::string, Block>                    m_receivedNotValidated;             // Vevtor holding the received but not yet validated blocks
        std::map<std::string, Block>                    m_onlyHeadersReceived;              // Vevtor holding the blocks that we know byt not received
        std::unique_ptr<nodeStatistics>                 m_nodeStats;                       // Struct holding the node stats
        std::vector<double>                             m_sendBlockTimes;                   // contains the times of the next sendBlock events
        std::vector<double>                             m_sendCompressedBlockTimes;         // contains the times of the next sendBlock events
        std::vector<double>                             m_receiveBlockTimes;                // contains the times of the next sendBlock events
        std::vector<double>                             m_receiveCompressedBlockTimes;      // contains the times of the next sendBlock events
        ProtocolType                               m_protocolType;                     // protocol type
        CommitterType                              m_committerType;

        const int       m_blockchainPort;           //8080
        const int       m_secondsPerMin;            //60
        const int       m_countBytes;               //The size of count variable in message, 4 Bytes
        const int       m_blockchainMessageHeader;  //The size of the Blockchain Message Header, 90 Bytes
        const int       m_inventorySizeBytes;       //The size of inventories in INV messages,36Bytes
        const int       m_getHeaderSizeBytes;       //The size of the GET_HEADERS message, 72bytes
        const int       m_headersSizeBytes;         //81Bytes
        const int       m_blockHeadersSizeBytes;     //81Bytes

        /*
         * Traced Callback: recevied packets, source address.
         */

        TracedCallback<Ptr<const Packet>, const Address &> m_rxTrace;
    };
}


#endif //NURAFT_BLOCKCHAINNODE_H