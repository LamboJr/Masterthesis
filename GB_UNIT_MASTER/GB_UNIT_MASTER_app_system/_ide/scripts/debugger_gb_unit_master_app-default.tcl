# Usage with Vitis IDE:
# In Vitis IDE create a Single Application Debug launch configuration,
# change the debug type to 'Attach to running target' and provide this 
# tcl script in 'Execute Script' option.
# Path of this script: /home/daniel/Masterthesis/GB_UNIT_MASTER/GB_UNIT_MASTER_app_system/_ide/scripts/debugger_gb_unit_master_app-default.tcl
# 
# 
# Usage with xsct:
# To debug using xsct, launch xsct and run below command
# source /home/daniel/Masterthesis/GB_UNIT_MASTER/GB_UNIT_MASTER_app_system/_ide/scripts/debugger_gb_unit_master_app-default.tcl
# 
connect -url tcp:127.0.0.1:3121
targets -set -nocase -filter {name =~"APU*"}
rst -system
after 3000
targets -set -filter {jtag_cable_name =~ "Digilent Zybo Z7 210351B105D5A" && level==0 && jtag_device_ctx=="jsn-Zybo Z7-210351B105D5A-23727093-0"}
fpga -file /home/daniel/Masterthesis/GB_UNIT_MASTER/GB_UNIT_MASTER_app/_ide/bitstream/ModuleTop.bit
targets -set -nocase -filter {name =~"APU*"}
loadhw -hw /home/daniel/Masterthesis/GB_UNIT_MASTER/ModuleTop/export/ModuleTop/hw/ModuleTop.xsa -mem-ranges [list {0x40000000 0xbfffffff}] -regs
configparams force-mem-access 1
targets -set -nocase -filter {name =~"APU*"}
source /home/daniel/Masterthesis/GB_UNIT_MASTER/GB_UNIT_MASTER_app/_ide/psinit/ps7_init.tcl
ps7_init
ps7_post_config
targets -set -nocase -filter {name =~ "*A9*#0"}
dow /home/daniel/Masterthesis/GB_UNIT_MASTER/GB_UNIT_MASTER_app/Debug/GB_UNIT_MASTER_app.elf
configparams force-mem-access 0
targets -set -nocase -filter {name =~ "*A9*#0"}
con
