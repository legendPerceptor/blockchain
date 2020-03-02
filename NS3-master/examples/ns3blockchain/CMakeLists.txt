set(name test_blockchain1)
set(source_files test/test_blockchain1.cpp
        model/blockchain.cpp model/blockchainnode.cpp model/blockchainminer.cpp)
set(header_files model/blockchain.h model/blockchainnode.h model/blockchainminer.h)
set(libraries_to_link ${libcore} ${libnetanim} ${libmpi} ${libpoint-to-point} ${libinternet} ${libapplications})
build_example ("${name}" "${source_files}" "${header_files}" "${libraries_to_link}")