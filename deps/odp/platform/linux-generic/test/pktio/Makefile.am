dist_check_SCRIPTS = pktio_env \
		     pktio_run \
		     pktio_run_tap

if HAVE_PCAP
dist_check_SCRIPTS += pktio_run_pcap
endif
if netmap_support
dist_check_SCRIPTS += pktio_run_netmap
endif
if PKTIO_DPDK
dist_check_SCRIPTS += pktio_run_dpdk
endif

test_SCRIPTS = $(dist_check_SCRIPTS)
