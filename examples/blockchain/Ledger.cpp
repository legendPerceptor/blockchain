//
// Created by Hython on 2019/12/31.
//

#include "Ledger.h"

#include "nuraft.hxx"
#include <sstream>
#include <cstdlib>
#include "SHA256.h"
#include <iostream>

using namespace std;
namespace ArcBlockChain {

    static const char alphanum[] =
            "0123456789"
            "!@#$%^&*"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz";

    static int alphaLength = sizeof(alphanum) - 1;

    static char genRandomChar()
    {
        return alphanum[rand() % alphaLength];
    }

    static string generateRandomHash(int length) {
        string ss;
        for(int i=0;i<length;i++) {
            ss += genRandomChar();
        }
        return ss;
    }

    std::string ArcBlockChain::LedgerHeader::toString() {
        ostringstream ss;
        ss << "(seq:" << seq << "|ts_amount:" << ts_amount << "|prev_hash:" << prev_hash << "|hash:" << hash<<")";
        return ss.str();
    }

    size_t ArcBlockChain::LedgerHeader::size() const {
        return sizeof(int32) * 5 + prev_hash.size() + hash.size() + timestamp.size();
    }

    std::string ArcBlockChain::transaction::toString() {
        ostringstream ss;
        ss << "["<<A<<"->"<<B<<", amount: "<<amount<<"]";
        return ss.str();
    }

    size_t ArcBlockChain::transaction::size() const {
        return sizeof(int32) * 3 + A.size() + B.size() + amount.size();
    }

    std::string Ledger::toString() {
        ostringstream ss;
        ss<<"{\nheader:\n\t"<<head.toString()<<"\ntransactions:\n";
        for(auto ii :transactions) {
            ss<<"\t"<<ii.toString()<<"\n";
        }
        ss<<"}";
        return ss.str();
    }

    size_t Ledger::size() const{
        size_t sz = head.size();
        for(auto ii : transactions) {
            sz += ii.size();
        }
        return sz;
    }

    std::string LedgerManger::computeHash(Ledger &block) {
        return sha256(block.toString());
    }

    Ledger LedgerManger::makeBlockWithCurrent() {
        current.head.prev_hash = prev_hash;
        current.head.ts_amount = current.transactions.size();
        current.head.seq = seq;
        seq+=1; //更新序列
        current.head.timestamp = getTimeStamp();
        // 加入随机因子，使得区块唯一
        current.head.hash = generateRandomHash(20);
        current.head.hash = computeHash(current);
        prev_hash = current.head.hash;

        blockchain.push_back(current);
        current = Ledger();

        return blockchain[blockchain.size()-1];

    }

    void LedgerManger::addBlock(Ledger &block) {
        blockchain.push_back(block);
    }

    std::string LedgerManger::getTimeStamp() {
        auto timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        return to_string(timestamp);
    }

    void LedgerManger::addTransaction(const transaction &t) {
        current.transactions.push_back(t);
    }

    string LedgerManger::getCurrentStatus() {
        ostringstream ss;
        ss<<"Number of blocks on the chain: "<<blockchain.size()<<"\n";
        ss<<"Sequence Number: " << seq <<"\nPrevious Hash:" << prev_hash<<"\n";
        ss<<"Number of current Transactions: "<<current.transactions.size()<<"\n";
        ss<<"Details of Transactions:"<<endl;
        for(auto ii :current.transactions){
            ss<<"\t"<<ii.toString()<<endl;
        }
        return ss.str();
    }

    void LedgerManger::printCurrentStatus() {
        cout<<getCurrentStatus()<<endl;
    }

    string LedgerManger::getBlockInfo(int begin, int end) {
        if(begin<0){begin=0;}
        if(end>blockchain.size()){end=blockchain.size();}
        if(begin>=end){
            return "Please make sure begin and end are in the appropriate range.";
        }
        ostringstream ss;
        ss<<"[\n";
        for(int i=begin;i<end;i++) {
            ss<<"\t"<<i<<":"<<blockchain[i].toString()<<"\n";
        }
        ss<<"]";
        return ss.str();
    }

    string LedgerManger::getBlockInfo(int index) {
        if(index<0 || index>blockchain.size()){
            return "Please make sure the index is in the appropriate range.";
        }
        return blockchain[index].toString();
    }

    LedgerManger::LedgerManger() {
        prev_hash= generateRandomHash(64);
    }

    std::string LedgerManger::getBlockInfo() {
        ostringstream ss;
        ss<<"[\n";

        for(int i=0;i<blockchain.size();i++) {
            ss<<"\t"<<i<<":"<<blockchain[i].toString()<<"\n";
        }
        ss<<"]";
        return ss.str();

    }
}