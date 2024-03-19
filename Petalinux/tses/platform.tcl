# 
# Usage: To re-create this platform project launch xsct with below options.
# xsct /home/daniel/Masterthesis/Petalinux/tses/platform.tcl
# 
# OR launch xsct and run below command.
# source /home/daniel/Masterthesis/Petalinux/tses/platform.tcl
# 
# To create the platform in a different location, modify the -out option of "platform create" command.
# -out option specifies the output directory of the platform project.

platform create -name {tses}\
-hw {/home/daniel/Masterthesis/GB_MASTER_UNIT/ModuleTop.xsa}\
-proc {ps7_cortexa9} -os {linux} -out {/home/daniel/Masterthesis/Petalinux}

platform write
platform active {tses}
