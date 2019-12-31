//
// Created by Hython on 2019/12/31.
//

#include "Ledger.h"
#include <iostream>
using namespace ArcBlockChain;
using namespace std;

int main() {

    LedgerManger ledgerManger;

    transaction t1("Harry", "Jenny", "124.24");
    transaction t2("Daisy","Coulson",421.53);
    transaction t3("John","Mack",14.25);

    ledgerManger.addTransaction(t1);
    ledgerManger.addTransaction(t2);
    ledgerManger.addTransaction(t3);
    ledgerManger.printCurrentStatus();
    ledgerManger.makeBlockWithCurrent();
    cout<<ledgerManger.getBlockInfo(0)<<endl;

    transaction r4("Yoyo","Simmons",126.32);
    ledgerManger.addTransaction(t2);
    ledgerManger.addTransaction(r4);
    ledgerManger.printCurrentStatus();
    ledgerManger.makeBlockWithCurrent();
    cout<<ledgerManger.getBlockInfo(0,2)<<endl;


    return 0;
}