transcript off
onbreak {quit -force}
onerror {quit -force}
transcript on

asim +access +r +m+GB_UNIT_design  -L xilinx_vip -L axi_bram_ctrl_v4_1_8 -L xil_defaultlib -L lib_cdc_v1_0_2 -L proc_sys_reset_v5_0_13 -L xilinx_vip -L unisims_ver -L unimacro_ver -L secureip -O5 xil_defaultlib.GB_UNIT_design xil_defaultlib.glbl

do {GB_UNIT_design.udo}

run 1000ns

endsim

quit -force
