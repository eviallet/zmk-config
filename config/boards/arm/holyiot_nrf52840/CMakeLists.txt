if(CONFIG_BOARD_HOLYIOT_NRF52840)
    project(zmk)
    target_sources(app PRIVATE board.c) 
endif(CONFIG_BOARD_HOLYIOT_NRF52840)