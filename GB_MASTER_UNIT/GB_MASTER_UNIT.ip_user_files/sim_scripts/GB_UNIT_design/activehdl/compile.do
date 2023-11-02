transcript off
onbreak {quit -force}
onerror {quit -force}
transcript on

vlib work
vmap -link {}
vlib activehdl/xilinx_vip
vlib activehdl/axi_bram_ctrl_v4_1_8
vlib activehdl/xil_defaultlib
vlib activehdl/lib_cdc_v1_0_2
vlib activehdl/proc_sys_reset_v5_0_13

vlog -work xilinx_vip  -sv2k12 "+incdir+/tools/Xilinx/Vivado/2023.1/data/xilinx_vip/include" -l xilinx_vip -l axi_bram_ctrl_v4_1_8 -l xil_defaultlib -l lib_cdc_v1_0_2 -l proc_sys_reset_v5_0_13 \
"/tools/Xilinx/Vivado/2023.1/data/xilinx_vip/hdl/axi4stream_vip_axi4streampc.sv" \
"/tools/Xilinx/Vivado/2023.1/data/xilinx_vip/hdl/axi_vip_axi4pc.sv" \
"/tools/Xilinx/Vivado/2023.1/data/xilinx_vip/hdl/xil_common_vip_pkg.sv" \
"/tools/Xilinx/Vivado/2023.1/data/xilinx_vip/hdl/axi4stream_vip_pkg.sv" \
"/tools/Xilinx/Vivado/2023.1/data/xilinx_vip/hdl/axi_vip_pkg.sv" \
"/tools/Xilinx/Vivado/2023.1/data/xilinx_vip/hdl/axi4stream_vip_if.sv" \
"/tools/Xilinx/Vivado/2023.1/data/xilinx_vip/hdl/axi_vip_if.sv" \
"/tools/Xilinx/Vivado/2023.1/data/xilinx_vip/hdl/clk_vip_if.sv" \
"/tools/Xilinx/Vivado/2023.1/data/xilinx_vip/hdl/rst_vip_if.sv" \

vcom -work axi_bram_ctrl_v4_1_8 -93  \
"../../../../GB_MASTER_UNIT.gen/sources_1/bd/GB_UNIT_design/ipshared/85f5/hdl/axi_bram_ctrl_v4_1_rfs.vhd" \

vcom -work xil_defaultlib -93  \
"../../../bd/GB_UNIT_design/ip/GB_UNIT_design_axi_bram_ctrl_0_0/sim/GB_UNIT_design_axi_bram_ctrl_0_0.vhd" \

vcom -work lib_cdc_v1_0_2 -93  \
"../../../../GB_MASTER_UNIT.gen/sources_1/bd/GB_UNIT_design/ipshared/ef1e/hdl/lib_cdc_v1_0_rfs.vhd" \

vcom -work proc_sys_reset_v5_0_13 -93  \
"../../../../GB_MASTER_UNIT.gen/sources_1/bd/GB_UNIT_design/ipshared/8842/hdl/proc_sys_reset_v5_0_vh_rfs.vhd" \

vcom -work xil_defaultlib -93  \
"../../../bd/GB_UNIT_design/ip/GB_UNIT_design_rst_ps7_0_50M_0/sim/GB_UNIT_design_rst_ps7_0_50M_0.vhd" \
"../../../bd/GB_UNIT_design/sim/GB_UNIT_design.vhd" \

vlog -work xil_defaultlib \
"glbl.v"

