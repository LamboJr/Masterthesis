transcript off
onbreak {quit -force}
onerror {quit -force}
transcript on

asim +access +r +m+GB_UNIT_design  -L xilinx_vip -L xpm -L xil_defaultlib -L axi_bram_ctrl_v4_1_8 -L lib_cdc_v1_0_2 -L proc_sys_reset_v5_0_13 -L axi_lite_ipif_v3_0_4 -L interrupt_control_v3_1_4 -L axi_gpio_v2_0_30 -L xilinx_vip -L unisims_ver -L unimacro_ver -L secureip -O2 xil_defaultlib.GB_UNIT_design xil_defaultlib.glbl

do {GB_UNIT_design.udo}

run

endsim

quit -force
