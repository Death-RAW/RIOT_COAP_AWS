# Default Makefile, for host native GNRC-based networking

# name of your application
APPLICATION = gcoap_example

# If no BOARD is found in the environment, use this default:
BOARD ?= iotlab-m3

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(CURDIR)/../..

# Increase from default for confirmable block2 follow-on requests
GCOAP_RESEND_BUFS_MAX ?= 2
CFLAGS += -DCONFIG_GCOAP_RESEND_BUFS_MAX=$(GCOAP_RESEND_BUFS_MAX)

USEMODULE += lps331ap
# Include packages that pull up and auto-init the link layer.
# NOTE: 6LoWPAN will be included if IEEE802.15.4 devices are present
USEMODULE += netdev_default
USEMODULE += auto_init_gnrc_netif
# Specify the mandatory networking modules
USEMODULE += gnrc_icmpv6_error
USEMODULE += gnrc_ipv6_default
USEMODULE += gnrc_ipv6_router_default
USEMODULE += gnrc_rpl
USEMODULE += auto_init_gnrc_rpl
USEMODULE += gnrc_icmpv6_echo
#USEMODULE += gnrc_udp_cmd
# Add also the shell, some shell commands

USEMODULE += shell
USEMODULE += shell_commands
USEMODULE += ps
USEMODULE += netstats_l2
USEMODULE += netstats_ipv6
USEMODULE += netstats_rpl

USEMODULE += sock_dns              # include DNS client
USEMODULE += gnrc_ipv6_nib_dns     # include RDNSS option handling
# This application dumps received packets to STDIO using the pktdump module
# Additional networking modules that can be dropped if not needed

USEMODULE += gcoap
# Required by gcoap example
USEMODULE += od
USEMODULE += fmt
# Include packages that pull up and auto-init the link layer.
# NOTE: 6LoWPAN will be included if IEEE802.15.4 devices are present


# Activate ICMPv6 error messages

# Specify the mandatory networking modules for IPv6 and UDP

USEMODULE += gnrc_udp
# Add a routing protocol
USEMODULE += gnrc_pktdump


# Add also the shell, some shell commands



# Import lps331ap driver module


# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
DEVELHELP ?= 1

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

include $(RIOTBASE)/Makefile.include

# Set a custom channel if needed
include $(RIOTMAKE)/default-radio-settings.inc.mk