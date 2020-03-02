//
// Created by Hython on 2019/12/31.
//

#ifndef NURAFT_LEDGER_H
#define NURAFT_LEDGER_H

#include <string>
#include <utility>
#include <vector>


namespace ArcBlockChain{

    struct LedgerHeader{
        size_t seq; //序列号
        std::string prev_hash;//上一个区块的哈希值
        std::string hash;//当前区块的哈希值
        std::string timestamp; //时间戳
        size_t ts_amount; //区块交易数量
        std::string toString();
        size_t size() const;
    };

    struct transaction {
        std::string A;
        std::string B;
        std::string amount;
        std::string toString();
        size_t size() const;
        transaction(std::string a, std::string b,
                std::string amount):A(std::move(a)),
                B(std::move(b)),amount(std::move(amount)){

        }
        transaction(const std::string&a, const std::string&b, double amount):A(a),B(b),amount(std::to_string(amount)){}
    };

    struct Ledger {
        LedgerHeader head;
        std::vector<transaction> transactions;
        std::string toString();
        size_t size() const;
    };

    class LedgerManger{
    public:
        Ledger& getCurrent(){
            return current;
        }

        std::vector<Ledger> getBlockchain(){
            return blockchain;
        }

        std::string computeHash(Ledger& block);

        std::string getTimeStamp();

        Ledger makeBlockWithCurrent();
        void addBlock(Ledger& block);

        void addTransaction(const transaction& t);
        std::string getCurrentStatus();
        void printCurrentStatus();

        std::string getBlockInfo(int begin, int end);
        std::string getBlockInfo(int index);
        std::string getBlockInfo();
        std::string prev_hash;

        LedgerManger();
        int seq;
    private:
        Ledger current;

        std::vector<Ledger> blockchain;

    };

}



#endif //NURAFT_LEDGER_H
