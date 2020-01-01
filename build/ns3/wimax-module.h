#ifdef NS3_MODULE_COMPILATION 
    error "Do not include ns3 module aggregator headers from other modules these are meant only for end user scripts." 
#endif 
#ifndef NS3_MODULE_WIMAX
    // Module headers: 
    #include <ns3/wimax-channel.h>
    #include <ns3/wimax-net-device.h>
    #include <ns3/bs-net-device.h>
    #include <ns3/ss-net-device.h>
    #include <ns3/cid.h>
    #include <ns3/cid-factory.h>
    #include <ns3/ofdm-downlink-frame-prefix.h>
    #include <ns3/wimax-connection.h>
    #include <ns3/ss-record.h>
    #include <ns3/mac-messages.h>
    #include <ns3/dl-mac-messages.h>
    #include <ns3/ul-mac-messages.h>
    #include <ns3/wimax-phy.h>
    #include <ns3/simple-ofdm-wimax-phy.h>
    #include <ns3/simple-ofdm-wimax-channel.h>
    #include <ns3/send-params.h>
    #include <ns3/service-flow.h>
    #include <ns3/ss-manager.h>
    #include <ns3/connection-manager.h>
    #include <ns3/wimax-mac-header.h>
    #include <ns3/wimax-mac-queue.h>
    #include <ns3/crc8.h>
    #include <ns3/service-flow-manager.h>
    #include <ns3/bs-uplink-scheduler.h>
    #include <ns3/bs-uplink-scheduler-simple.h>
    #include <ns3/bs-uplink-scheduler-mbqos.h>
    #include <ns3/bs-uplink-scheduler-rtps.h>
    #include <ns3/ul-job.h>
    #include <ns3/bs-scheduler.h>
    #include <ns3/bs-scheduler-simple.h>
    #include <ns3/bs-scheduler-rtps.h>
    #include <ns3/service-flow-record.h>
    #include <ns3/snr-to-block-error-rate-record.h>
    #include <ns3/snr-to-block-error-rate-manager.h>
    #include <ns3/simple-ofdm-send-param.h>
    #include <ns3/ss-service-flow-manager.h>
    #include <ns3/bs-service-flow-manager.h>
    #include <ns3/cs-parameters.h>
    #include <ns3/ipcs-classifier-record.h>
    #include <ns3/wimax-tlv.h>
    #include <ns3/ipcs-classifier.h>
    #include <ns3/bvec.h>
    #include <ns3/wimax-mac-to-mac-header.h>
    #include <ns3/wimax-helper.h>
#endif 