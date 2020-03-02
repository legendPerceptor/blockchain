//
// Created by Hython on 2020/2/20.
//
#include "blockchain.h"
#include "ns3/application.h"
#include "ns3/ptr.h"
#include "ns3/event-id.h"
#include "ns3/log.h"
#include "ns3/traced-callback.h"


namespace ns3::blockchain {

    // Class Transaction
    // Function Definitions
    Transaction::Transaction(int nodeId, int transId, double timeStamp)
    {
        m_nodeId = nodeId;
        m_transId = transId;
        m_transSizeByte = 100;
        m_timeStamp = timeStamp;
        m_validation = false;
        m_execution = 0;
    }

    Transaction::Transaction():Transaction(0, 0, 0)
    {
    }

    Transaction::~Transaction()
    {
    }

    int Transaction::GetTransNodeId(void) const
    {
        return m_nodeId;
    }

    void Transaction::SetTransNodeId(int nodeId)
    {
        m_nodeId = nodeId;
    }

    int Transaction::GetTransId(void) const
    {
        return m_transId;
    }

    void Transaction::SetTransId(int transId)
    {
        m_transId = transId;
    }

    int Transaction::GetTransSizeByte(void) const
    {
        return m_transSizeByte;
    }

    void Transaction::SetTransSizeByte(int transSizeByte)
    {
        m_transSizeByte = transSizeByte;
    }

    double Transaction::GetTransTimeStamp(void) const
    {
        return m_timeStamp;
    }

    void Transaction::SetTransTimeStamp(double timeStamp)
    {
        m_timeStamp = timeStamp;
    }

    bool Transaction::IsValidated(void) const
    {
        return m_validation;
    }

    void Transaction::SetValidation()
    {
        m_validation = true;
    }

    int Transaction::GetExecution(void) const
    {
        return m_execution;
    }

    void Transaction::SetExecution(int endorserId)
    {
        m_execution = endorserId;

    }

    // 可以直接简化成 = default 这里这样写巩固一下C++基础，这是个简单拷贝
    Transaction& Transaction::operator= (const Transaction &tranSource)
    {
        m_nodeId = tranSource.m_nodeId;
        m_transId = tranSource.m_transId;
        m_transSizeByte = tranSource.m_transSizeByte;
        m_timeStamp = tranSource.m_timeStamp;
        m_validation = tranSource.m_validation;
        m_execution = tranSource.m_execution;

        return *this;
    }

    bool operator== (const Transaction &tran1, const Transaction &tran2)
    {
        return tran1.GetTransNodeId() == tran2.GetTransNodeId() && tran1.GetTransId() == tran2.GetTransId();
    }

    ///: Class Transaction Definitions END ---

    // Class Block
    // Function Definitions
    Block::Block(int blockHeight, int minerId, int nonce, int parentBlockMinerId, int blockSizeBytes,
                 double timeStamp, double timeReceived, Ipv4Address receivedFromIpv4)
    {
        m_blockHeight = blockHeight;
        m_minerId = minerId;
        m_nonce = nonce;
        m_parentBlockMinerId = parentBlockMinerId;
        m_blockSizeBytes = blockSizeBytes;
        m_timeStamp = timeStamp;
        m_timeReceived = timeReceived;
        m_receivedFromIpv4 = receivedFromIpv4;
        m_totalTransactions = 0;

    }

    Block::Block():Block(0,0,0,0,0,0,0, Ipv4Address("0.0.0.0"))
    {
    }

    // 拷贝构造函数，深度复制 Deep Copy
    Block::Block(const Block &blockSource) {
        m_blockHeight = blockSource.m_blockHeight;
        m_minerId = blockSource.m_minerId;
        m_nonce = blockSource.m_nonce;
        m_parentBlockMinerId = blockSource.m_parentBlockMinerId;
        m_blockSizeBytes = blockSource.m_blockSizeBytes;
        m_timeStamp = blockSource.m_timeStamp;
        m_timeReceived = blockSource.m_timeReceived;
        m_receivedFromIpv4 = blockSource.m_receivedFromIpv4;
        m_transactions = blockSource.m_transactions;
        m_totalTransactions = 0;

    }

    Block::~Block() {}

    int Block::GetBlockHeight() const {
        return m_blockHeight;
    }

    void Block::SetBlockHeight(int blockHeight) {
        m_blockHeight = blockHeight;
    }

    int Block::GetNonce() const {return m_nonce;}

    void Block::SetNonce(int nonce) {
        m_nonce = nonce;
    }

    int Block::GetMinerId(void) const {
        return m_minerId;
    }

    void Block::SetMinerId(int minerId) {
        m_minerId = minerId;
    }

    int Block::GetParentBlockMinerId(void) const {
        return m_parentBlockMinerId;
    }

    void Block::SetParentBlockMinerId(int parentBlockMinerId) {
        m_parentBlockMinerId = parentBlockMinerId;
    }

    int Block::GetBlockSizeBytes(void) const {
        return m_blockSizeBytes;
    }

    void Block::SetBlockSizeBytes(int blockSizeBytes) {
        m_blockSizeBytes = blockSizeBytes;
    }

    double Block::GetTimeStamp(void) const {
        return m_timeStamp;
    }

    void Block::SetTimeStamp(double timeStamp) {
        m_timeStamp = timeStamp;
    }

    double Block::GetTimeReceived(void) const {
        return m_timeReceived;
    }

    Ipv4Address Block::GetReceivedFromIpv4(void) const {
        return m_receivedFromIpv4;
    }

    void Block::SetReceivedFromIpv4(Ipv4Address receivedFromIpv4) {
        m_receivedFromIpv4 = receivedFromIpv4;
    }

    std::vector<Transaction> Block::GetTransactions(void) const {
        return m_transactions;
    }

    void Block::SetTransactions(const std::vector<Transaction> &transactions) {
        m_transactions = transactions;
    }

    bool Block::IsParent(const Block &block) const {
        return GetBlockHeight()==block.GetBlockHeight() - 1 && GetMinerId() == block.GetParentBlockMinerId();
    }

    bool Block::IsChild(const Block &block) const {
        return GetBlockHeight()==block.GetBlockHeight()+1 && GetParentBlockMinerId()==block.GetMinerId();
    }

    int Block::GetTotalTransaction(void) const {
        return m_totalTransactions;
    }

    //Possibly Cannot find one, Use Option (in Rust) as the return type is better
    //There is a proposal in C++ Conf that std::expected will be included
    Transaction Block::ReturnTransaction(int nodeId, int transId) {
        for(auto const &tran: m_transactions) {
            if (tran.GetTransNodeId() == nodeId && tran.GetTransId() == transId) {
                return tran;
            }
        }
        //这里简单化处理如果没找到返回默认对象
        return Transaction();
    }

    bool Block::HasTransaction(Transaction &newTran) const {
        for(auto const &tran: m_transactions)
        {
            if(tran == newTran)
            {
                return true;
            }
        }

        return false;
    }

    bool Block::HasTransaction(int nodeId, int tranId) const {
        for(auto const &tran: m_transactions)
        {
            if(tran.GetTransNodeId() == nodeId && tran.GetTransId() == tranId)
            {
                return true;
            }
        }
        return false;
    }

    void Block::AddTransaction(Transaction &newTrans) {
        m_transactions.push_back(newTrans);
        m_totalTransactions++;
    }
    // If the value passed in is an rvalue, just move it in
    // This function is to improve the efficiency
    void Block::AddTransaction(Transaction &&newTrans) {
        m_transactions.push_back(std::move(newTrans));
        m_totalTransactions++;
    }


    void Block::PrintAllTransactions(void) {
        if(m_transactions.size() != 0)
        {
            for(auto const &tran: m_transactions)
            {
                std::cout<<"[Blockheight: " <<m_blockHeight << "] Transaction nodeId: "
                         << tran.GetTransNodeId() << " transId : " << tran.GetTransId() << "\n";
            }
        }
        else
        {
            std::cout<<"[Blockheight: " <<m_blockHeight << "]  do not have transactions\n";
        }
    }

    // It's a trivial copy, just use default
    Block &Block::operator=(const Block &blockSource) = default;

    bool operator== (const Block& block1, const Block& block2){
        return block1.GetBlockHeight() == block2.GetBlockHeight() && block1.GetMinerId() == block2.GetMinerId()
    }
    ///: Class Block Function Definitions END ---

    // Tool Function Definitions
    const char *getMessageName(Messages m) {
        switch(m)
        {
            case Messages::INV: return "INV";
            case Messages::REQUEST_TRANS: return "REQUEST_TRANS";
            case Messages::GET_HEADERS: return "GET_HEADERS";
            case Messages::HEADERS: return "HEADERS";
            case Messages::GET_DATA: return "GET_DATA";
            case Messages::BLOCK: return "BLOCK";
            case Messages::NO_MESSAGE: return "NO_MESSAGE";
            case Messages::REPLY_TRANS: return "REPLY_TRANS";
            case Messages::MSG_TRANS: return "MSG_TRANS";
            case Messages::RESULT_TRANS: return "RESULT_TRANS";
        }
        return nullptr;
    }

    const char *getMinerType(MinerType m) {
        switch(m)
        {
            case MinerType::NORMAL_MINER: return "ETHEREUM";
            case MinerType::HYPERLEDGER_MINER: return "HYPERLEDGER";
        }

        return nullptr;
    }

    const char *getCommitterType(CommitterType m) {
        switch(m)
        {
            case CommitterType::COMMITTER: return "COMMITTER";
            case CommitterType::ENDORSER: return "ENDORSER";
            case CommitterType::CLIENT: return "CLIENT";
            case CommitterType::ORDER: return "ORDER";
        }
        return nullptr;
    }

    const char *getProtocolType(ProtocolType m) {
        switch(m)
        {
            case ProtocolType::STANDARD_PROTOCOL: return "STANDARD_PROTOCOL";
            case ProtocolType::SENDHEADERS: return "SENDHEADERS";
        }
        return nullptr;
    }

    const char *getCryptocurrency(Cryptocurrency m) {
        switch(m) {
            case Cryptocurrency::ETHEREUM: return "ETHEREUM";
            case Cryptocurrency::HYPERLEDGER: return "HYPERLEDGER";
        }
        return nullptr;
    }

    const char *getBlockchainRegion(BlockchainRegion m) {
        switch(m)
        {
            case BlockchainRegion::NORTH_AMERICA: return "NORTH_AMERICA";
            case BlockchainRegion::EUROPE: return "EUROPE";
            case BlockchainRegion::SOUTH_AMERICA: return "SOUTH_AMERICA";
            case BlockchainRegion::KOREA: return "KOREA";
            case BlockchainRegion::JAPAN: return "JAPAN";
            case BlockchainRegion::AUSTRALIA: return "AUSTRALIA";
            case BlockchainRegion::CHINA: return "CHINA";
            case BlockchainRegion::OTHER: return "OTHER";
        }
        return nullptr;
    }

    BlockchainRegion getBlockchainEnum(uint32_t n) {
        switch(n)
        {
            case 0: return BlockchainRegion::NORTH_AMERICA;
            case 1: return BlockchainRegion::EUROPE;
            case 2: return BlockchainRegion::SOUTH_AMERICA;
            case 3: return BlockchainRegion::KOREA;
            case 4: return BlockchainRegion::JAPAN;
            case 5: return BlockchainRegion::AUSTRALIA;
            case 6: return BlockchainRegion::CHINA;
            case 7: return BlockchainRegion::OTHER;
            default:
                return BlockchainRegion::OTHER;
        }

    }

    ///: Tool Function Definitions END ---


    // Class Blockchain
    // Function Definitions


    Blockchain::Blockchain() {
        m_totalBlocks = 0;
        AddBlock(Block());
    }

    Blockchain::~Blockchain() {

    }

    int Blockchain::GetTotalBlocks() const {
        return m_totalBlocks;
    }

    int Blockchain::GetNoOrphans() const {
        return m_orphans.size();
    }

    int Blockchain::GetBlockchainHeight() const {
        //C++ 17 feature: if-init expression
        //C++ 17 feature: access optional object
        if( auto topblock = GetCurrentTopBlock(); topblock)
            return topblock.value()->GetBlockHeight();
        else {
            std::cerr<<"Cannot get the height of the Blockchain."<<std::endl;
            std::cerr<<"The Blockchain has no blocks at all."<<std::endl;
            return -1;
        }
    }

    bool Blockchain::HasBlock(const Block &newBlock) const {
        if(newBlock.GetBlockHeight() > GetCurrentTopBlock().value()->GetBlockHeight()){
            return false;
        }else {
            for(auto const &block: m_blocks[newBlock.GetBlockHeight()])
            {
                if(block == newBlock)
                {
                    return true;
                }
            }
        }
        return false;
    }

    bool Blockchain::HasBlock(int height, int minerID) const {
        if(height > GetCurrentTopBlock().value()->GetBlockHeight()) {
            return false;
        } else {
            for(auto const &block: m_blocks[newBlock.GetBlockHeight()])
            {
                if(block == newBlock)
                {
                    return true;
                }
            }
        }
        return false;
    }

    Block Blockchain::ReturnBlock(int height, int minerID) {
        std::vector<Block>::iterator block_it;

        if(height <= GetBlockchainHeight() && height >= 0)
        {
            for(block_it = m_blocks[height].begin(); block_it < m_blocks[height].end(); block_it++)
            {
                if(block_it->GetBlockHeight() == height && block_it->GetMinerId() == minerId)
                {
                    return *block_it;
                }
            }
        }

        for(block_it = m_orphans.begin(); block_it < m_orphans.end(); block_it++)
        {
            if(block_it->GetBlockHeight()==height && block_it->GetMinerId()==minerId)
            {
                return *block_it;
            }
        }


        return Block();
    }

    bool Blockchain::IsOrphan(const Block &newBlock) const {
        for(auto const &block: m_orphans)
        {
            if(block == newBlock)
            {
                return true;
            }
        }
        return false;
    }

    bool Blockchain::IsOrphan(int height, int minerId) const {
        for(auto const &block: m_orphans)
        {
            if(block.GetBlockHeight()==height && block.GetMinerId()==minerId)
            {
                return true;
            }
        }
        return false;
    }

    std::optional<const Block*> Blockchain::GetBlockPointer(const Block &newBlock) const {
        for(auto const &block : m_blocks[newBlock.GetBlockHeight()])
        {
            if(block == newBlock)
            {
                return &block;
            }
        }

        return {};
    }

    std::optional<std::vector<const Block*>> Blockchain::GetChildrenPointers(const Block &block) {
        std::vector<const Block *> children;
        std::vector<Block>::iterator block_it;
        int childrenHeight = block.GetBlockHeight() + 1;

        if(childrenHeight > GetBlockchainHeight())
        {
            return children;
        }

        for(block_it = m_blocks[childrenHeight].begin(); block_it < m_blocks[childrenHeight].end(); block_it++)
        {
            if(block.IsParent(*block_it))
            {
                children.push_back(&(*block_it));
            }
        }

        return children;
    }

    std::optional<std::vector<const Block*>> Blockchain::GetOrphanChildrenPointers(const Block &block) {
        std::vector<const Block*> children;
        std::vector<Block>::iterator block_it;
        for(block_it = m_orphans.begin(); block_it < m_orphans.end(); block_it++) {
            if(block.IsParent(*block_it)) {
                children.push_back(&(*block_it));
            }
        }
        return children;
    }

    // Optional
    std::optional<const Block*> Blockchain::GetParent(const Block &block) {

        std::vector<Block>::iterator block_it;
        int parentHeight = block.GetBlockHeight() - 1;

        if(parentHeight > GetBlockchainHeight() || parentHeight < 0)
            return {};

        for(block_it = m_blocks[parentHeight].begin();block_it!=m_blocks[parentHeight].end();block_it++) {
            if(block.IsChild(*block_it)) {
                return &(*block_it);
            }
        }
        return {};
    }

    std::optional<const Block*> Blockchain::GetCurrentTopBlock() const {
        if(m_blocks.empty()) return {};
        return &m_blocks[m_blocks.size()-1][0];
    }

    void Blockchain::AddBlock(const Block &newBlock) {
        if(m_blocks.empty()) {
            m_blocks.emplace_back(1, newBlock);
        }else if(newBlock.GetBlockHeight() > GetCurrentTopBlock().value()->GetBlockHeight()) {
            // C++17： 这里GetCurrentTopBlock一定不会为空，
            // 编译器不知道，我们知道，因此简单使用.value()取值，不处理错误
            int dummyRows = newBlock.GetBlockHeight() - GetCurrentTopBlock().value()->GetBlockHeight() -1;
            for(int i=0;i<dummyRows;i++){
                m_blocks.emplace_back();
            }
            m_blocks.emplace_back(1, newBlock);
        }else {
            m_blocks[newBlock.GetBlockHeight()].push_back(newBlock);
        }
        m_totalBlocks++;
    }



    void Blockchain::AddOrphan(const Block &newBlock) {
        m_orphans.push_back(newBlock);
    }


    void Blockchain::RemoveOrphan(const Block &newBlock) {
        std::vector<Block>::iterator block_it;

        for(block_it = m_orphans.begin(); block_it<m_orphans.end(); block_it++) {
            if(newBlock==*block_it) {
                break;
            }
        }
        if(block_it == m_orphans.end()) {
            return;
        }else {
            m_orphans.erase(block_it);
        }
    }
}