# 
# Usage: To re-create this platform project launch xsct with below options.
# xsct /home/daniel/Masterthesis/Petalinux/Petalinux_Platorm/platform.tcl
# 
# OR launch xsct and run below command.
# source /home/daniel/Masterthesis/Petalinux/Petalinux_Platorm/platform.tcl
# 
# To create the platform in a different location, modify the -out option of "platform create" command.
# -out option specifies the output directory of the platform project.

platform create -name {Petalinux_Platorm}\
-hw {/home/daniel/PetaLinuxProjects/ModuleTop.xsa}\
-proc {ps7_cortexa9} -os {linux} -out {/home/daniel/Masterthesis/Petalinux}

platform write
platform active {Petalinux_Platorm}
platform active {Petalinux_Platorm}
domain active {zynq_fsbl}
bsp reload
platform active {Petalinux_Platorm}
domain active {zynq_fsbl}
bsp reload
platform active {Petalinux_Platorm}
domain active {zynq_fsbl}
bsp reload
bsp reload
domain active {linux_domain}
domain config -bif {/home/daniel/PetaLinuxProjects/ZynqLinux/images/linux/bootgen.bif}
platform write
domain config -boot {/home/daniel/PetaLinuxProjects/ZynqLinux/images/linux}
platform write
platform generate
domain config -bif {/home/daniel/Test/images/bootgen.bif}
platform write
domain config -boot {/home/daniel/Test/images}
platform write
domain config -image {/home/daniel/Test/images}
platform write
domain config -rootfs {/home/daniel/Test/images/rootfs.ext4}
platform write
domain config -sysroot {/home/daniel/Test/images}
domain config -sysroot {/home/daniel/PetaLinuxProjects/ZynqLinux}
domain config -sysroot {/home/daniel/PetaLinuxProjects/ZynqLinux/pre-built/linux/implementation}
domain config -sysroot {/home/daniel/PetaLinuxProjects/ZynqLinux/project-spec/meta-user}
domain config -sysroot {/home/daniel/Masterthesis/Petalinux/Petalinux_Platorm/resources/Petalinux_Platorm}
platform generate -domains 
platform active {Petalinux_Platorm}
platform active {Petalinux_Platorm}
domain active {zynq_fsbl}
bsp reload
bsp reload
platform config -updatehw {/home/daniel/PetaLinuxProjects/ModuleTop.xsa}
domain active {linux_domain}
domain config -bif {}
platform write
domain config -boot {}
platform write
domain config -rootfs {}
platform write
domain config -image {}
platform write
platform generate
platform generate
platform active {Petalinux_Platorm}
domain create -name {Standalone_APP} -os {standalone} -proc {ps7_cortexa9_0} -arch {32-bit} -display-name {Standalone_APP} -desc {} -runtime {cpp}
platform generate -domains 
platform write
domain -report -json
bsp reload
platform generate -domains Standalone_APP 
platform active {Petalinux_Platorm}
domain active {zynq_fsbl}
bsp reload
domain active {Standalone_APP}
bsp reload
platform generate
platform generate
platform active {Petalinux_Platorm}
domain active {zynq_fsbl}
bsp reload
domain active {Standalone_APP}
bsp reload
platform generate -domains 
domain active {linux_domain}
domain config -generate-bif
platform write
domain config -bif {}
platform write
domain config -rootfs {/home/daniel/PetaLinuxProjects/ZynqLinux/images/linux/rootfs.tar.gz}
platform write
domain config -boot {/home/daniel/PetaLinuxProjects/ZynqLinux/images/linux}
platform write
platform generate -domains 
domain config -bif {/home/daniel/PetaLinuxProjects/ZynqLinux/images/linux/bootgen.bif}
platform write
domain config -image {/home/daniel/Test/images}
platform write
platform generate -domains 
platform clean
platform generate
platform active {Petalinux_Platorm}
domain config -bif {}
platform write
domain config -boot {}
platform write
domain config -rootfs {}
platform write
domain config -image {}
platform write
domain active {zynq_fsbl}
bsp reload
domain active {Standalone_APP}
bsp reload
platform generate -domains 
platform clean
platform generate
platform clean
platform generate
platform active {Petalinux_Platorm}
domain remove Standalone_APP
platform generate -domains 
platform write
platform generate -domains 
platform active {Petalinux_Platorm}
domain config -sysroot {/home/daniel/PetaLinuxProjects/ZynqLinux/images/linux/sdk/sysroots/cortexa9t2hf-neon-xilinx-linux-gnueabi}
platform write
platform generate -domains 
domain config -sysroot {}
platform write
platform generate -domains 
domain config -bootmode {qspi}
platform write
platform generate -domains 
platform active {Petalinux_Platorm}
domain active {zynq_fsbl}
bsp reload
platform clean
domain active {linux_domain}
domain config -bootmode {sd}
platform write
platform generate
domain config -sysroot {/home/daniel/PetaLinuxProjects/ZynqLinux/images/linux/sdk/sysroots/cortexa9t2hf-neon-xilinx-linux-gnueabi}
platform write
platform clean
platform generate
platform active {Petalinux_Platorm}
platform generate -domains 
platform generate -domains 
platform generate
platform active {Petalinux_Platorm}
platform active {Petalinux_Platorm}
catch {platform remove tses}
platform active {Petalinux_Platorm}
