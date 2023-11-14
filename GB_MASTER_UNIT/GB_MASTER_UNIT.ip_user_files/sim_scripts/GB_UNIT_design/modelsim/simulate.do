onbreak {quit -f}
onerror {quit -f}

vsim -voptargs="+acc"  -L xilinx_vip -L xpm -L xil_defaultlib -L axi_bram_ctrl_v4_1_8 -L lib_cdc_v1_0_2 -L proc_sys_reset_v5_0_13 -L axi_lite_ipif_v3_0_4 -L interrupt_control_v3_1_4 -L axi_gpio_v2_0_30 -L xilinx_vip -L unisims_ver -L unimacro_ver -L secureip -lib xil_defaultlib xil_defaultlib.GB_UNIT_design xil_defaultlib.glbl

set NumericStdNoWarnings 1
set StdArithNoWarnings 1

do {wave.do}

view wave
view structure
view signals

do {GB_UNIT_design.udo}

run 1000ns

quit -force
