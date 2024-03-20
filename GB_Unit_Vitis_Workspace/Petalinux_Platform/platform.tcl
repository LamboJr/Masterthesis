# 
# Usage: To re-create this platform project launch xsct with below options.
# xsct /home/daniel/Masterthesis/GB_Unit_Vitis_Workspace/Petalinux_Platform/platform.tcl
# 
# OR launch xsct and run below command.
# source /home/daniel/Masterthesis/GB_Unit_Vitis_Workspace/Petalinux_Platform/platform.tcl
# 
# To create the platform in a different location, modify the -out option of "platform create" command.
# -out option specifies the output directory of the platform project.

platform create -name {Petalinux_Platform}\
-hw {/home/daniel/Masterthesis/GB_MASTER_UNIT/ModuleTop.xsa}\
-proc {ps7_cortexa9} -os {linux} -out {/home/daniel/Masterthesis/GB_Unit_Vitis_Workspace}

platform write
platform active {Petalinux_Platform}
domain config -sysroot {/home/daniel/PetaLinuxProjects/ZynqLinux/images/linux/sdk/sysroots/cortexa9t2hf-neon-xilinx-linux-gnueabi/usr}
domain config -sysroot {/home/daniel/PetaLinuxProjects/ZynqLinux/images/linux/sdk/sysroots/cortexa9t2hf-neon-xilinx-linux-gnueabi}
platform write
platform generate
platform active {Petalinux_Platform}
library -lib-path {} -inc-path {/home/daniel/Masterthesis/GB_Unit_Vitis_Workspace/Petalinux_Platform/export/Petalinux_Platform/sw/Petalinux_Platform/linux_domain/sysroot/cortexa9t2hf-neon-xilinx-linux-gnueabi/usr/lib}
platform write
platform generate -domains 
platform generate
