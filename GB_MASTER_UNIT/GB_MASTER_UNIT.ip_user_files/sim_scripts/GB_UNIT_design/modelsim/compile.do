vlib modelsim_lib/work
vlib modelsim_lib/msim

vlib modelsim_lib/msim/xilinx_vip
vlib modelsim_lib/msim/xpm
vlib modelsim_lib/msim/xil_defaultlib
vlib modelsim_lib/msim/axi_bram_ctrl_v4_1_8
vlib modelsim_lib/msim/lib_cdc_v1_0_2
vlib modelsim_lib/msim/proc_sys_reset_v5_0_13
vlib modelsim_lib/msim/axi_lite_ipif_v3_0_4
vlib modelsim_lib/msim/interrupt_control_v3_1_4
vlib modelsim_lib/msim/axi_gpio_v2_0_30

vmap xilinx_vip modelsim_lib/msim/xilinx_vip
vmap xpm modelsim_lib/msim/xpm
vmap xil_defaultlib modelsim_lib/msim/xil_defaultlib
vmap axi_bram_ctrl_v4_1_8 modelsim_lib/msim/axi_bram_ctrl_v4_1_8
vmap lib_cdc_v1_0_2 modelsim_lib/msim/lib_cdc_v1_0_2
vmap proc_sys_reset_v5_0_13 modelsim_lib/msim/proc_sys_reset_v5_0_13
vmap axi_lite_ipif_v3_0_4 modelsim_lib/msim/axi_lite_ipif_v3_0_4
vmap interrupt_control_v3_1_4 modelsim_lib/msim/interrupt_control_v3_1_4
vmap axi_gpio_v2_0_30 modelsim_lib/msim/axi_gpio_v2_0_30

vlog -work xilinx_vip -64 -incr -mfcu  -sv -L axi_vip_v1_1_14 -L smartconnect_v1_0 -L processing_system7_vip_v1_0_16 -L xilinx_vip "+incdir+/tools/Xilinx/Vivado/2023.1/data/xilinx_vip/include" \
"/tools/Xilinx/Vivado/2023.1/data/xilinx_vip/hdl/axi4stream_vip_axi4streampc.sv" \
"/tools/Xilinx/Vivado/2023.1/data/xilinx_vip/hdl/axi_vip_axi4pc.sv" \
"/tools/Xilinx/Vivado/2023.1/data/xilinx_vip/hdl/xil_common_vip_pkg.sv" \
"/tools/Xilinx/Vivado/2023.1/data/xilinx_vip/hdl/axi4stream_vip_pkg.sv" \
"/tools/Xilinx/Vivado/2023.1/data/xilinx_vip/hdl/axi_vip_pkg.sv" \
"/tools/Xilinx/Vivado/2023.1/data/xilinx_vip/hdl/axi4stream_vip_if.sv" \
"/tools/Xilinx/Vivado/2023.1/data/xilinx_vip/hdl/axi_vip_if.sv" \
"/tools/Xilinx/Vivado/2023.1/data/xilinx_vip/hdl/clk_vip_if.sv" \
"/tools/Xilinx/Vivado/2023.1/data/xilinx_vip/hdl/rst_vip_if.sv" \

vlog -work xpm -64 -incr -mfcu  -sv -L axi_vip_v1_1_14 -L smartconnect_v1_0 -L processing_system7_vip_v1_0_16 -L xilinx_vip "+incdir+../../../../GB_MASTER_UNIT.gen/sources_1/bd/GB_UNIT_design/ipshared/ec67/hdl" "+incdir+../../../../GB_MASTER_UNIT.gen/sources_1/bd/GB_UNIT_design/ipshared/aed8/hdl" "+incdir+../../../../GB_MASTER_UNIT.gen/sources_1/bd/GB_UNIT_design/ipshared/f0b6/hdl/verilog" "+incdir+../../../../GB_MASTER_UNIT.gen/sources_1/bd/GB_UNIT_design/ipshared/66be/hdl/verilog" "+incdir+/tools/Xilinx/Vivado/2023.1/data/xilinx_vip/include" \
"/tools/Xilinx/Vivado/2023.1/data/ip/xpm/xpm_cdc/hdl/xpm_cdc.sv" \
"/tools/Xilinx/Vivado/2023.1/data/ip/xpm/xpm_fifo/hdl/xpm_fifo.sv" \
"/tools/Xilinx/Vivado/2023.1/data/ip/xpm/xpm_memory/hdl/xpm_memory.sv" \

vcom -work xpm -64 -93  \
"/tools/Xilinx/Vivado/2023.1/data/ip/xpm/xpm_VCOMP.vhd" \

vcom -work xil_defaultlib -64 -93  \
"../../../bd/GB_UNIT_design/ip/GB_UNIT_design_processing_system7_0_0/GB_UNIT_design_processing_system7_0_0_sim_netlist.vhdl" \

vcom -work axi_bram_ctrl_v4_1_8 -64 -93  \
"../../../../GB_MASTER_UNIT.gen/sources_1/bd/GB_UNIT_design/ipshared/85f5/hdl/axi_bram_ctrl_v4_1_rfs.vhd" \

vcom -work xil_defaultlib -64 -93  \
"../../../bd/GB_UNIT_design/ip/GB_UNIT_design_axi_bram_ctrl_0_0/sim/GB_UNIT_design_axi_bram_ctrl_0_0.vhd" \
"/home/daniel/Masterthesis/GB_MASTER_UNIT/GB_MASTER_UNIT.gen/sources_1/bd/GB_UNIT_design/ip/GB_UNIT_design_axi_smc_0/GB_UNIT_design_axi_smc_0_sim_netlist.vhdl" \

vcom -work lib_cdc_v1_0_2 -64 -93  \
"../../../../GB_MASTER_UNIT.gen/sources_1/bd/GB_UNIT_design/ipshared/ef1e/hdl/lib_cdc_v1_0_rfs.vhd" \

vcom -work proc_sys_reset_v5_0_13 -64 -93  \
"../../../../GB_MASTER_UNIT.gen/sources_1/bd/GB_UNIT_design/ipshared/8842/hdl/proc_sys_reset_v5_0_vh_rfs.vhd" \

vcom -work xil_defaultlib -64 -93  \
"../../../bd/GB_UNIT_design/ip/GB_UNIT_design_rst_ps7_0_50M_0/sim/GB_UNIT_design_rst_ps7_0_50M_0.vhd" \
"../../../bd/GB_UNIT_design/sim/GB_UNIT_design.vhd" \

vcom -work axi_lite_ipif_v3_0_4 -64 -93  \
"../../../../GB_MASTER_UNIT.gen/sources_1/bd/GB_UNIT_design/ipshared/66ea/hdl/axi_lite_ipif_v3_0_vh_rfs.vhd" \

vcom -work interrupt_control_v3_1_4 -64 -93  \
"../../../../GB_MASTER_UNIT.gen/sources_1/bd/GB_UNIT_design/ipshared/a040/hdl/interrupt_control_v3_1_vh_rfs.vhd" \

vcom -work axi_gpio_v2_0_30 -64 -93  \
"../../../../GB_MASTER_UNIT.gen/sources_1/bd/GB_UNIT_design/ipshared/18b7/hdl/axi_gpio_v2_0_vh_rfs.vhd" \

vcom -work xil_defaultlib -64 -93  \
"../../../bd/GB_UNIT_design/ip/GB_UNIT_design_axi_gpio_0_0/sim/GB_UNIT_design_axi_gpio_0_0.vhd" \

vlog -work xil_defaultlib \
"glbl.v"

