# 
# Usage: To re-create this platform project launch xsct with below options.
# xsct /home/daniel/Masterthesis/GB_UNIT_MASTER/ModuleTop/platform.tcl
# 
# OR launch xsct and run below command.
# source /home/daniel/Masterthesis/GB_UNIT_MASTER/ModuleTop/platform.tcl
# 
# To create the platform in a different location, modify the -out option of "platform create" command.
# -out option specifies the output directory of the platform project.

platform create -name {ModuleTop}\
-hw {/home/daniel/Masterthesis/GB_MASTER_UNIT/ModuleTop.xsa}\
-out {/home/daniel/Masterthesis/GB_UNIT_MASTER}

platform write
domain create -name {standalone_ps7_cortexa9_0} -display-name {standalone_ps7_cortexa9_0} -os {standalone} -proc {ps7_cortexa9_0} -runtime {cpp} -arch {32-bit} -support-app {hello_world}
platform generate -domains 
platform active {ModuleTop}
domain active {zynq_fsbl}
domain active {standalone_ps7_cortexa9_0}
platform generate -quick
platform generate
platform config -updatehw {/home/daniel/Masterthesis/GB_MASTER_UNIT/ModuleTop.xsa}
platform generate -domains 
platform active {ModuleTop}
platform config -updatehw {/home/daniel/Masterthesis/GB_MASTER_UNIT/ModuleTop.xsa}
platform generate -domains 
platform config -updatehw {/home/daniel/Masterthesis/GB_MASTER_UNIT/ModuleTop.xsa}
platform generate -domains 
platform config -updatehw {/home/daniel/Masterthesis/GB_MASTER_UNIT/ModuleTop.xsa}
platform generate -domains 
platform config -updatehw {/home/daniel/Masterthesis/GB_MASTER_UNIT/ModuleTop.xsa}
platform generate -domains 
platform clean
platform generate
platform generate
platform active {ModuleTop}
platform config -updatehw {/home/daniel/Masterthesis/GB_MASTER_UNIT/ModuleTop.xsa}
platform generate -domains 
platform active {ModuleTop}
platform config -updatehw {/home/daniel/Masterthesis/GB_MASTER_UNIT/ModuleTop.xsa}
platform config -updatehw {/home/daniel/Masterthesis/GB_MASTER_UNIT/ModuleTop.xsa}
platform generate -domains 
platform generate
platform clean
platform generate
platform clean
platform generate
platform clean
platform generate
platform clean
platform generate
platform clean
platform generate
platform active {ModuleTop}
platform config -updatehw {/home/daniel/Masterthesis/GB_MASTER_UNIT/ModuleTop.xsa}
platform config -updatehw {/home/daniel/Masterthesis/GB_MASTER_UNIT/ModuleTop.xsa}
platform clean
platform generate
platform config -updatehw {/home/daniel/Masterthesis/GB_MASTER_UNIT/ModuleTop.xsa}
platform config -updatehw {/home/daniel/Masterthesis/GB_MASTER_UNIT/ModuleTop.xsa}
platform generate -domains 
platform active {ModuleTop}
platform config -updatehw {/home/daniel/Masterthesis/GB_MASTER_UNIT/ModuleTop.xsa}
platform generate -domains standalone_ps7_cortexa9_0,zynq_fsbl 
platform config -updatehw {/home/daniel/Masterthesis/GB_MASTER_UNIT/ModuleTop.xsa}
platform generate -domains 
platform config -updatehw {/home/daniel/Masterthesis/GB_MASTER_UNIT/ModuleTop.xsa}
platform generate -domains 
platform config -updatehw {/home/daniel/Masterthesis/GB_MASTER_UNIT/ModuleTop.xsa}
platform generate -domains 
platform config -updatehw {/home/daniel/Masterthesis/GB_MASTER_UNIT/ModuleTop.xsa}
platform generate -domains 
platform config -updatehw {/home/daniel/Masterthesis/GB_MASTER_UNIT/ModuleTop.xsa}
platform config -updatehw {/home/daniel/Masterthesis/GB_MASTER_UNIT/ModuleTop.xsa}
platform generate -domains 
platform config -updatehw {/home/daniel/Masterthesis/GB_MASTER_UNIT/ModuleTop.xsa}
platform generate -domains 
platform active {ModuleTop}
platform config -updatehw {/home/daniel/Masterthesis/GB_MASTER_UNIT/ModuleTop.xsa}
platform generate -domains 
platform config -updatehw {/home/daniel/Masterthesis/GB_MASTER_UNIT/ModuleTop.xsa}
platform generate -domains 
platform config -updatehw {/home/daniel/Masterthesis/GB_MASTER_UNIT/ModuleTop.xsa}
platform generate -domains 
platform config -updatehw {/home/daniel/Masterthesis/GB_MASTER_UNIT/ModuleTop.xsa}
platform generate -domains 
platform config -updatehw {/home/daniel/Masterthesis/GB_MASTER_UNIT/ModuleTop.xsa}
platform generate -domains 
platform config -updatehw {/home/daniel/Masterthesis/GB_MASTER_UNIT/ModuleTop.xsa}
platform generate -domains 
platform config -updatehw {/home/daniel/Masterthesis/GB_MASTER_UNIT/ModuleTop.xsa}
platform generate -domains 
platform active {ModuleTop}
platform config -updatehw {/home/daniel/Masterthesis/GB_MASTER_UNIT/ModuleTop.xsa}
platform generate -domains standalone_ps7_cortexa9_0 
platform config -updatehw {/home/daniel/Masterthesis/GB_MASTER_UNIT/ModuleTop.xsa}
platform generate -domains 
platform config -updatehw {/home/daniel/Masterthesis/GB_MASTER_UNIT/ModuleTop.xsa}
platform generate -domains 
platform config -updatehw {/home/daniel/Masterthesis/GB_MASTER_UNIT/ModuleTop.xsa}
platform generate -domains 
platform config -updatehw {/home/daniel/Masterthesis/GB_MASTER_UNIT/ModuleTop.xsa}
platform generate -domains 
platform config -updatehw {/home/daniel/Masterthesis/GB_MASTER_UNIT/ModuleTop.xsa}
platform generate -domains 
platform active {ModuleTop}
platform config -updatehw {/home/daniel/Masterthesis/GB_MASTER_UNIT/ModuleTop.xsa}
platform generate -domains 
platform config -updatehw {/home/daniel/Masterthesis/GB_MASTER_UNIT/ModuleTop.xsa}
platform generate -domains 
platform config -updatehw {/home/daniel/Masterthesis/GB_MASTER_UNIT/ModuleTop.xsa}
platform generate -domains 
platform config -updatehw {/home/daniel/Masterthesis/GB_MASTER_UNIT/ModuleTop.xsa}
platform generate -domains 
platform config -updatehw {/home/daniel/Masterthesis/GB_MASTER_UNIT/ModuleTop.xsa}
platform generate -domains 
platform config -updatehw {/home/daniel/Masterthesis/GB_MASTER_UNIT/ModuleTop.xsa}
platform generate -domains 
platform config -updatehw {/home/daniel/Masterthesis/GB_MASTER_UNIT/ModuleTop.xsa}
platform generate -domains 
platform config -updatehw {/home/daniel/Masterthesis/GB_MASTER_UNIT/ModuleTop.xsa}
platform generate -domains 
platform config -updatehw {/home/daniel/Masterthesis/GB_MASTER_UNIT/ModuleTop.xsa}
platform generate -domains 
platform config -updatehw {/home/daniel/Masterthesis/GB_MASTER_UNIT/ModuleTop.xsa}
platform generate -domains 
platform config -updatehw {/home/daniel/Masterthesis/GB_MASTER_UNIT/ModuleTop.xsa}
platform generate -domains 
platform active {ModuleTop}
platform config -updatehw {/home/daniel/Masterthesis/GB_MASTER_UNIT/ModuleTop.xsa}
platform generate -domains standalone_ps7_cortexa9_0 
platform config -updatehw {/home/daniel/Masterthesis/GB_MASTER_UNIT/ModuleTop.xsa}
platform active {ModuleTop}
platform config -updatehw {/home/daniel/Masterthesis/GB_MASTER_UNIT/ModuleTop.xsa}
platform generate -domains 
platform active {ModuleTop}
platform config -updatehw {/home/daniel/Masterthesis/GB_MASTER_UNIT/ModuleTop.xsa}
platform generate -domains 
platform active {ModuleTop}
platform config -updatehw {/home/daniel/Masterthesis/GB_MASTER_UNIT/ModuleTop.xsa}
platform generate -domains standalone_ps7_cortexa9_0 
platform config -updatehw {/home/daniel/Masterthesis/GB_MASTER_UNIT/ModuleTop.xsa}
platform generate -domains 
platform active {ModuleTop}
platform config -updatehw {/home/daniel/Masterthesis/GB_MASTER_UNIT/ModuleTop.xsa}
platform generate -domains standalone_ps7_cortexa9_0 
platform config -updatehw {/home/daniel/Masterthesis/GB_MASTER_UNIT/ModuleTop.xsa}
platform generate -domains 
platform config -updatehw {/home/daniel/Masterthesis/GB_MASTER_UNIT/ModuleTop.xsa}
platform generate -domains 
platform active {ModuleTop}
platform config -updatehw {/home/daniel/Masterthesis/GB_MASTER_UNIT/ModuleTop.xsa}
platform generate -domains 
platform active {ModuleTop}
platform config -updatehw {/home/daniel/Masterthesis/GB_MASTER_UNIT/ModuleTop.xsa}
platform generate -domains 
platform config -updatehw {/home/daniel/Masterthesis/GB_MASTER_UNIT/ModuleTop.xsa}
platform config -updatehw {/home/daniel/Masterthesis/GB_MASTER_UNIT/ModuleTop.xsa}
platform generate -domains 
platform config -updatehw {/home/daniel/Masterthesis/GB_MASTER_UNIT/ModuleTop.xsa}
platform generate -domains 
platform active {ModuleTop}
platform config -updatehw {/home/daniel/Masterthesis/GB_MASTER_UNIT/ModuleTop.xsa}
platform generate
platform config -updatehw {/home/daniel/Masterthesis/GB_MASTER_UNIT/ModuleTop.xsa}
platform config -updatehw {/home/daniel/Masterthesis/GB_MASTER_UNIT/ModuleTop.xsa}
platform generate -domains 
platform config -updatehw {/home/daniel/Masterthesis/GB_MASTER_UNIT/ModuleTop.xsa}
platform generate -domains 
platform config -updatehw {/home/daniel/Masterthesis/GB_MASTER_UNIT/ModuleTop.xsa}
platform generate -domains 
platform active {ModuleTop}
platform config -updatehw {/home/daniel/Masterthesis/GB_MASTER_UNIT/ModuleTop.xsa}
platform generate
platform config -updatehw {/home/daniel/Masterthesis/GB_MASTER_UNIT/ModuleTop.xsa}
platform generate -domains 
platform active {ModuleTop}
platform config -updatehw {/home/daniel/Masterthesis/GB_MASTER_UNIT/ModuleTop.xsa}
platform config -updatehw {/home/daniel/Masterthesis/GB_MASTER_UNIT/ModuleTop.xsa}
platform generate
platform active {ModuleTop}
platform config -updatehw {/home/daniel/Masterthesis/GB_MASTER_UNIT/ModuleTop.xsa}
platform config -updatehw {/home/daniel/Masterthesis/GB_MASTER_UNIT/ModuleTop.xsa}
platform generate
platform config -updatehw {/home/daniel/Masterthesis/GB_MASTER_UNIT/ModuleTop.xsa}
platform generate -domains 
platform active {ModuleTop}
platform config -updatehw {/home/daniel/Masterthesis/GB_MASTER_UNIT/ModuleTop.xsa}
platform generate -domains 
platform clean
platform clean
platform generate
platform clean
platform generate
platform clean
platform generate
platform generate -domains standalone_ps7_cortexa9_0,zynq_fsbl 
platform active {ModuleTop}
platform config -updatehw {/home/daniel/Masterthesis/GB_MASTER_UNIT/ModuleTop.xsa}
platform generate -domains 
platform active {ModuleTop}
platform config -updatehw {/home/daniel/Masterthesis/GB_MASTER_UNIT/ModuleTop.xsa}
platform generate
platform active {ModuleTop}
platform config -updatehw {/home/daniel/Masterthesis/GB_MASTER_UNIT/ModuleTop.xsa}
platform generate -domains 
platform config -updatehw {/home/daniel/Masterthesis/GB_MASTER_UNIT/ModuleTop.xsa}
platform generate -domains 
platform active {ModuleTop}
bsp reload
bsp config stdin "ps7_uart_1"
bsp config xil_interrupt "false"
bsp reload
bsp reload
domain active {zynq_fsbl}
bsp reload
platform active {ModuleTop}
platform active {ModuleTop}
platform generate
platform active {ModuleTop}
platform config -updatehw {/home/daniel/Masterthesis/GB_MASTER_UNIT/ModuleTop.xsa}
platform generate -domains 
platform config -updatehw {/home/daniel/Masterthesis/GB_MASTER_UNIT/ModuleTop.xsa}
platform generate -domains 
platform config -updatehw {/home/daniel/Masterthesis/GB_MASTER_UNIT/ModuleTop.xsa}
platform generate -domains 
platform config -updatehw {/home/daniel/Masterthesis/GB_MASTER_UNIT/ModuleTop.xsa}
platform generate -domains 
platform active {ModuleTop}
platform config -updatehw {/home/daniel/Masterthesis/GB_MASTER_UNIT/ModuleTop.xsa}
platform generate
platform config -updatehw {/home/daniel/Masterthesis/GB_MASTER_UNIT/ModuleTop.xsa}
platform generate -domains 
platform config -updatehw {/home/daniel/Masterthesis/GB_MASTER_UNIT/ModuleTop.xsa}
platform generate -domains 
platform config -updatehw {/home/daniel/Masterthesis/GB_MASTER_UNIT/ModuleTop.xsa}
platform generate -domains 
platform config -updatehw {/home/daniel/Masterthesis/GB_MASTER_UNIT/ModuleTop.xsa}
platform generate -domains 
