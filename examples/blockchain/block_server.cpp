//
// Created by Hython on 2019/12/31.
//

#include "blockchain_state_machine.h"
#include "in_memory_state_mgr.hxx"
#include "logger_wrapper.hxx"

#include "nuraft.hxx"

#include "test_common.h"

#include <iostream>
#include <sstream>

#include <stdio.h>

using namespace nuraft;
using namespace std;
namespace ArcBlockChain {

    static raft_params::return_method_type CALL_TYPE
            = raft_params::blocking;
//  = raft_params::async_handler;

#include "example_common.hxx"

    blockchain_state_machine* get_sm() {
        return static_cast<blockchain_state_machine*>( stuff.sm_.get() );
    }

    ptr<buffer> blockchain_state_machine::pre_commit(const ulong log_idx, buffer &data)  {
        // Nothing to do with pre-commit in this example.

        if(stuff.raft_instance_->is_leader()) {
            buffer_serializer bs(data);
            bs.put_i32(cur_value_);
            bs.pos(sizeof(int32) * 2);
            string prev_hash_global = globalLedgerManager.prev_hash;
            string original = bs.get_str();
            bs.pos(sizeof(int32) * 2);
//            cout<<"Pre commit (prev_hash): " << prev_hash_global<<"\n";
//            cout<<"Original (prev_hash): "<<original<<"\n";
            bs.put_str(prev_hash_global);

        }

        return nullptr;
    }

    ptr<buffer> blockchain_state_machine::commit(const ulong log_idx, buffer &data) {
        Ledger block;
        dec_log(data, block);
//        cout<<"In commit (block prev_hash):"<<block.head.prev_hash<<endl;
//        cout<<"In commit (block hash):"<<block.head.hash<<endl;
//        cout<<"In commit (block):\n"<<block.toString()<<endl;
        globalLedgerManager.addBlock(block);
        globalLedgerManager.prev_hash = block.head.hash;
        cur_value_ +=1;
        globalLedgerManager.seq = cur_value_;
        last_committed_idx_ = log_idx;

        // Return Raft log number as a return result.
        ptr<buffer> ret = buffer::alloc( sizeof(log_idx) );
        buffer_serializer bs(ret);
        bs.put_u64(log_idx);
        return ret;
    }


    void handle_result(ptr<TestSuite::Timer> timer,
                       raft_result& result,
                       ptr<std::exception>& err)
    {
        if (result.get_result_code() != cmd_result_code::OK) {
            // Something went wrong.
            // This means committing this log failed,
            // but the log itself is still in the log store.
            std::cout << "failed: " << result.get_result_code() << ", "
                      << TestSuite::usToString( timer->getTimeUs() )
                      << std::endl;
            return;
        }
        ptr<buffer> buf = result.get();
        uint64_t ret_value = buf->get_ulong();
        std::cout << "succeeded, "
                  << TestSuite::usToString( timer->getTimeUs() )
                  << ", return value: "
                  << ret_value
                  << ", state machine value: "
                  << get_sm()->get_current_value()
                  << "local blockchain status: \n"
                  << get_sm()->localLedgerManager.getCurrentStatus()
                  << "\nglobal blockchain status: \n"
                  << get_sm()->globalLedgerManager.getCurrentStatus()
                  << std::endl;
    }

    void append_log(const std::string& cmd,
                    const std::vector<std::string>& tokens)
    {

        Ledger t = get_sm()->localLedgerManager.makeBlockWithCurrent();
        if(stuff.raft_instance_->is_leader()){
            get_sm()->is_leader = true; // deprecated
            CALL_TYPE = raft_params::blocking;
        }else{
            get_sm()->is_leader = false; // deprecated
            CALL_TYPE = raft_params::async_handler;
        }
        // Serialize and generate Raft log to append.
        ptr<buffer> new_log = blockchain_state_machine::enc_log( {t} );

        // To measure the elapsed time.
        ptr<TestSuite::Timer> timer = cs_new<TestSuite::Timer>();

        // Do append.
        ptr<raft_result> ret = stuff.raft_instance_->append_entries( {new_log} );

        if (!ret->get_accepted()) {
            if(ret->get_result_code() == cmd_result_code::NOT_LEADER){
                std::cout<<"The request has been forwarded to the leader."<<std::endl;
            }else {
                // Log append rejected, usually because this node is not a leader.
                std::cout << "failed to replicate: "
                          << ret->get_result_code() << ", "
                          << TestSuite::usToString(timer->getTimeUs())
                          << std::endl;
                //return;
            }
            return;
        }
        // Log append accepted, but that doesn't mean the log is committed.
        // Commit result can be obtained below.

        if (CALL_TYPE == raft_params::blocking) {
            // Blocking mode:
            //   `append_entries` returns after getting a consensus,
            //   so that `ret` already has the result from state machine.
            ptr<std::exception> err(nullptr);
            handle_result(timer, *ret, err);

        } else if (CALL_TYPE == raft_params::async_handler) {
            // Async mode:
            //   `append_entries` returns immediately.
            //   `handle_result` will be invoked asynchronously,
            //   after getting a consensus.
            ret->when_ready( std::bind( handle_result,
                                        timer,
                                        std::placeholders::_1,
                                        std::placeholders::_2 ) );

        } else {
            assert(0);
        }
    }

    void print_status(const std::string& cmd,
                      const std::vector<std::string>& tokens)
    {
        ptr<log_store> ls = stuff.smgr_->load_log_store();
        std::cout
                << "my server id: " << stuff.server_id_ << std::endl
                << "leader id: " << stuff.raft_instance_->get_leader() << std::endl
                << "Raft log range: "
                << ls->start_index()
                << " - " << (ls->next_slot() - 1) << std::endl
                << "last committed index: "
                << stuff.raft_instance_->get_committed_log_idx() << std::endl
                << "state machine value: "
                << get_sm()->get_current_value() << std::endl
                << "local LedgerManager status:\n"
                << get_sm()->localLedgerManager.getCurrentStatus()<<std::endl
                << "global LedgerManger status:\n"
                << get_sm()->globalLedgerManager.getCurrentStatus()<<std::endl;
    }

    void help(const std::string& cmd,
              const std::vector<std::string>& tokens)
    {
        std::cout
                << "add transaction:\n"
                << "    e.g.) ts A B 100\n"
                << "add server: add <server id> <address>:<port>\n"
                << "    e.g.) add 2 127.0.0.1:20000\n"
                << "\n"
                << "get current server status: st (or stat)\n"
                << "\n"
                << "get the list of members: ls (or list)\n"
                << "\n";
    }


    void add_transaction(const std::string& cmd,
                         const std::vector<std::string>& tokens) {

        if(tokens.size()!=4){
            std::cerr<<"Please provide correct format of a transaction.\n"
            <<"    e.g.) ts A B 100"<<endl;
            return;
        }
        std::string A, B, amount;
        A = tokens[1];
        B = tokens[2];
        amount = tokens[3];

        get_sm()->localLedgerManager.addTransaction(transaction(A,B,amount));
    }

    void show_blocks(const std::string& cmd,
                     const std::vector<std::string>& tokens){
        LedgerManger* ledgerManger;
        if(tokens[1] =="global"){
            ledgerManger = &get_sm()->globalLedgerManager;
        }else if (tokens[1]=="local") {
            ledgerManger = &get_sm()->localLedgerManager;
        }else {
            return;
        }
        std::string blockinfo;
        try{
            if(tokens.size()==4){
                blockinfo = ledgerManger->getBlockInfo(stoi(tokens[2]),stoi(tokens[3]));
            }else if(tokens.size()==3) {
                blockinfo = ledgerManger->getBlockInfo(stoi(tokens[2]));
            }else if(tokens.size()==2) {
                blockinfo = ledgerManger->getBlockInfo();
            }
            cout<<blockinfo<<endl;
        }catch (const std::invalid_argument& ia){
            std::cerr<<"Please provide valid indexes.\n"<<ia.what()<<std::endl;

        }catch (const std::out_of_range& oor) {
            std::cerr << "The indexes you provide is out of range.\n" << oor.what() << std::endl;
        }catch (const std::exception& f) {
            std::cerr << "exception happened during show_blocks\n"<<f.what() <<std::endl;
        }

    }

    bool do_cmd(const std::vector<std::string>& tokens) {
        if (!tokens.size()) return true;

        const std::string& cmd = tokens[0];

        if (cmd == "q" || cmd == "exit") {
            stuff.launcher_.shutdown(5);
            stuff.reset();
            return false;

        } else if ( cmd == "ts" || cmd == "transfer" ) {
            // e.g.) +1
            add_transaction(cmd,tokens);

        } else if (cmd == "bb" || cmd=="buildblock"){
            append_log(cmd, tokens);
        } else if (cmd == "show") {
            show_blocks(cmd, tokens);
        }
        else if ( cmd == "add" ) {
            // e.g.) add 2 localhost:12345
            add_server(cmd, tokens);

        } else if ( cmd == "st" || cmd == "stat" ) {
            print_status(cmd, tokens);

        } else if ( cmd == "ls" || cmd == "list" ) {
            server_list(cmd, tokens);

        } else if ( cmd == "h" || cmd == "help" ) {
            help(cmd, tokens);
        }
        return true;
    }

}; // namespace ArcBlockChain;
using namespace ArcBlockChain;

int main(int argc, char** argv) {
    if (argc < 3) usage(argc, argv);

    set_server_info(argc, argv);

    std::cout << "    -- Basic Blockchain Application with Raft --" << std::endl;
    std::cout << "    Author: Yuanjian LIU       Version 0.1.0" << std::endl;
    std::cout << "    Server ID:    " << stuff.server_id_ << std::endl;
    std::cout << "    Endpoint:     " << stuff.endpoint_ << std::endl;
    init_raft( cs_new<blockchain_state_machine>() );
    loop("Blockchain");

    return 0;
}

