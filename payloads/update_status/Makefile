ifndef PS5_PAYLOAD_SDK
$(error Set PS5_PAYLOAD_SDK to the extracted official SDK)
endif
all:
	$(PS5_PAYLOAD_SDK)/bin/prospero-clang -Wall -Wextra -Werror main.c -lSceRegMgr -o update-status.elf
