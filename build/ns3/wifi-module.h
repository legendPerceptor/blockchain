#ifdef NS3_MODULE_COMPILATION 
    error "Do not include ns3 module aggregator headers from other modules these are meant only for end user scripts." 
#endif 
#ifndef NS3_MODULE_WIFI
    // Module headers: 
    #include <ns3/wifi-utils.h>
    #include <ns3/wifi-information-element.h>
    #include <ns3/wifi-information-element-vector.h>
    #include <ns3/wifi-net-device.h>
    #include <ns3/wifi-mode.h>
    #include <ns3/ssid.h>
    #include <ns3/wifi-preamble.h>
    #include <ns3/wifi-phy-standard.h>
    #include <ns3/yans-wifi-phy.h>
    #include <ns3/spectrum-wifi-phy.h>
    #include <ns3/wifi-phy-tag.h>
    #include <ns3/tx-vector-tag.h>
    #include <ns3/yans-wifi-channel.h>
    #include <ns3/wifi-phy.h>
    #include <ns3/wifi-spectrum-phy-interface.h>
    #include <ns3/wifi-spectrum-signal-parameters.h>
    #include <ns3/interference-helper.h>
    #include <ns3/wifi-remote-station-info.h>
    #include <ns3/wifi-remote-station-manager.h>
    #include <ns3/ap-wifi-mac.h>
    #include <ns3/sta-wifi-mac.h>
    #include <ns3/adhoc-wifi-mac.h>
    #include <ns3/arf-wifi-manager.h>
    #include <ns3/aarf-wifi-manager.h>
    #include <ns3/ideal-wifi-manager.h>
    #include <ns3/constant-rate-wifi-manager.h>
    #include <ns3/amrr-wifi-manager.h>
    #include <ns3/onoe-wifi-manager.h>
    #include <ns3/rraa-wifi-manager.h>
    #include <ns3/aarfcd-wifi-manager.h>
    #include <ns3/cara-wifi-manager.h>
    #include <ns3/minstrel-wifi-manager.h>
    #include <ns3/minstrel-ht-wifi-manager.h>
    #include <ns3/wifi-mac.h>
    #include <ns3/regular-wifi-mac.h>
    #include <ns3/infrastructure-wifi-mac.h>
    #include <ns3/supported-rates.h>
    #include <ns3/error-rate-model.h>
    #include <ns3/yans-error-rate-model.h>
    #include <ns3/nist-error-rate-model.h>
    #include <ns3/dsss-error-rate-model.h>
    #include <ns3/wifi-mac-queue.h>
    #include <ns3/txop.h>
    #include <ns3/wifi-phy-header.h>
    #include <ns3/wifi-mac-header.h>
    #include <ns3/wifi-mac-trailer.h>
    #include <ns3/wifi-phy-state-helper.h>
    #include <ns3/qos-utils.h>
    #include <ns3/qos-txop.h>
    #include <ns3/msdu-aggregator.h>
    #include <ns3/amsdu-subframe-header.h>
    #include <ns3/mgt-headers.h>
    #include <ns3/status-code.h>
    #include <ns3/capability-information.h>
    #include <ns3/channel-access-manager.h>
    #include <ns3/mac-tx-middle.h>
    #include <ns3/mac-rx-middle.h>
    #include <ns3/mac-low.h>
    #include <ns3/mac-low-transmission-parameters.h>
    #include <ns3/originator-block-ack-agreement.h>
    #include <ns3/ctrl-headers.h>
    #include <ns3/block-ack-agreement.h>
    #include <ns3/block-ack-manager.h>
    #include <ns3/block-ack-cache.h>
    #include <ns3/block-ack-window.h>
    #include <ns3/snr-tag.h>
    #include <ns3/ht-capabilities.h>
    #include <ns3/parf-wifi-manager.h>
    #include <ns3/aparf-wifi-manager.h>
    #include <ns3/rrpaa-wifi-manager.h>
    #include <ns3/wifi-tx-vector.h>
    #include <ns3/ampdu-subframe-header.h>
    #include <ns3/mpdu-aggregator.h>
    #include <ns3/ampdu-tag.h>
    #include <ns3/wifi-radio-energy-model.h>
    #include <ns3/wifi-tx-current-model.h>
    #include <ns3/vht-capabilities.h>
    #include <ns3/erp-information.h>
    #include <ns3/ht-operation.h>
    #include <ns3/vht-operation.h>
    #include <ns3/dsss-parameter-set.h>
    #include <ns3/edca-parameter-set.h>
    #include <ns3/he-capabilities.h>
    #include <ns3/frame-capture-model.h>
    #include <ns3/simple-frame-capture-model.h>
    #include <ns3/preamble-detection-model.h>
    #include <ns3/threshold-preamble-detection-model.h>
    #include <ns3/qos-blocked-destinations.h>
    #include <ns3/he-operation.h>
    #include <ns3/he-configuration.h>
    #include <ns3/extended-capabilities.h>
    #include <ns3/cf-parameter-set.h>
    #include <ns3/wifi-mac-queue-item.h>
    #include <ns3/wifi-psdu.h>
    #include <ns3/wifi-phy-state.h>
    #include <ns3/wifi-phy-listener.h>
    #include <ns3/block-ack-type.h>
    #include <ns3/wifi-mpdu-type.h>
    #include <ns3/ht-configuration.h>
    #include <ns3/vht-configuration.h>
    #include <ns3/obss-pd-algorithm.h>
    #include <ns3/constant-obss-pd-algorithm.h>
    #include <ns3/wifi-radio-energy-model-helper.h>
    #include <ns3/athstats-helper.h>
    #include <ns3/wifi-helper.h>
    #include <ns3/yans-wifi-helper.h>
    #include <ns3/spectrum-wifi-helper.h>
    #include <ns3/wifi-mac-helper.h>
#endif 