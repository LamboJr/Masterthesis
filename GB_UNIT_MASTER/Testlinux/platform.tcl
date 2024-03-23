# 
# Usage: To re-create this platform project launch xsct with below options.
# xsct /home/daniel/Masterthesis/GB_UNIT_MASTER/Testlinux/platform.tcl
# 
# OR launch xsct and run below command.
# source /home/daniel/Masterthesis/GB_UNIT_MASTER/Testlinux/platform.tcl
# 
# To create the platform in a different location, modify the -out option of "platform create" command.
# -out option specifies the output directory of the platform project.

platform create -name {Testlinux}\
-hw {/home/daniel/Masterthesis/GB_MASTER_UNIT/ModuleTop.xsa}\
-proc {ps7_cortexa9} -os {linux} -out {/home/daniel/Masterthesis/GB_UNIT_MASTER}

platform write
platform active {Testlinux}
platform generate
platform active {Testlinux}
