#!/bin/bash -f
# ****************************************************************************
# Vivado (TM) v2023.1 (64-bit)
#
# Filename    : compile.sh
# Simulator   : Xilinx Vivado Simulator
# Description : Script for compiling the simulation design source files
#
# Generated by Vivado on Fri Nov 03 11:34:09 CET 2023
# SW Build 3865809 on Sun May  7 15:04:56 MDT 2023
#
# IP Build 3864474 on Sun May  7 20:36:21 MDT 2023
#
# usage: compile.sh
#
# ****************************************************************************
set -Eeuo pipefail
# compile Verilog/System Verilog design sources
echo "xvlog --incr --relax -L axi_vip_v1_1_14 -L smartconnect_v1_0 -L processing_system7_vip_v1_0_16 -L xilinx_vip -prj UART_TB_vlog.prj"
xvlog --incr --relax -L axi_vip_v1_1_14 -L smartconnect_v1_0 -L processing_system7_vip_v1_0_16 -L xilinx_vip -prj UART_TB_vlog.prj 2>&1 | tee compile.log

# compile VHDL design sources
echo "xvhdl --incr --relax -prj UART_TB_vhdl.prj"
xvhdl --incr --relax -prj UART_TB_vhdl.prj 2>&1 | tee -a compile.log

echo "Waiting for jobs to finish..."
echo "No pending jobs, compilation finished."
