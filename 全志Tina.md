# Tina开发环境

编译 Tina Linux SDK ->pack->

```
---------------------------------------------
/home/lubancat/tina_f133-v1.0-wisesun-evb-20230110/tina_f133_v1.0_ws_board/out/f133-b-evb1/tina_f133-b-evb1_uart0.img


U-boot 编译 代码包根目录下输入命令 muboot,
u-boot 路径: lichee\brandy-2.0\u-boot-2018
内核编译 代码包根目录下输入命令 mkernel
Kernel 路径: lichee\linux-5.4
应用编译 切换到对应应用路径，使用命令 mm 进行编译， 清除中间文件编译使用
mm -B 命令

[2] Tina 系统
(1) 固件打包命令： pack
编译完成后在编译终端输入 pack 命令进行打包
(2) 分区配置文件:
Norflash 分区配置文件路径：\device\config\chips\f133\configs\evb1\sys_partition_nor.fex
其他存储介质配置文件路径:
\device\config\chips\f133\configs\evb1\sys_partition.fex
(4) 主要分区说明
boot-resource: 开机 logo 所在分区，根据开机 logo 大小配置
rootfs: 文件系统所在分区
rootfs_data：根目录分区
UDISK： 用户数据分区， 剩余空间会自动分配到此分区
```



Tina Linux SDK 主要由构建系统、配置工具、工具链、host 工具包、目标设备应用程序、文档、脚本、linux 内核、bootloader 部分组成，下文按照目录顺序介绍相关的组成组件。

```
Tina-SDK/
    ├── build
    ├── config
    ├── Config.in
    ├── device
    ├── dl
    ├── docs
    ├── lichee
    ├── Makefile
    ├── out
    ├── package
    ├── prebuilt
    ├── rules.mk
    ├── scripts
    ├── target
    ├── tmp
    ├── toolchain
    └── tools
    
build 目录存放 Tina Linux 的构建系统文件，此目录结构下主要是一系列基于 Makefile 规格编写的 mk 文件。主要的功能是：
1. 检测当前的编译环境是否满足 Tina Linux 的构建需求。
2. 生成 host 包编译规则。
3. 生成工具链的编译规则。
4. 生成 target 包的编译规则。
5. 生成 linux kernel 的编译规则。
6. 生成系统固件的生成规则。
build/
    ├── autotools.mk
    ├── aw-upgrade.mk
    ├── board.mk
    ├── cmake.mk
    ├── config.mk
    ├── debug.mk
    ├── depends.mk
    ├── device.mk
    ├── device_table.txt
    ├── download.mk
    ├── dumpvar.mk
    ├── envsetup.s
    
devices 目录用于存放方案的配置文件，包括内核配置，env 配置，分区表配置，sys_config.fex，
board.dts 等。
这些配置在旧版本上是保存于 target 目录下，新版本挪到 device 目录。
注意 defconfig 仍保存在 target 目录。    
device/
    └── config
    ├── chips
    ├── common
    └── rootfs_tar
快捷跳转命令：cconfigs

docs 目录主要存放用于开发的文档，以 markdown 格式书写。
   
lichee 目录主要存放 bootloader，内核，arisc，dsp 等代码。
lichee/
    ├── bootloader
    │ ├── uboot_2011_sunxi_spl
    │ └── uboot_2014_sunxi_spl
    ├── brandy
    │ ├── u-boot-2011.09
    │ └── u-boot-2014.07
    ├── brandy-2.0
    │ ├── spl
    │ ├── tools
    │ └── u-boot-2018
    ├── linux-3.4
    ├── linux-3.10
    ├── linux-4.4
    ├── linux-4.9
    ├── arisc
    └── dsp
快捷跳转命令：ckernel，cboot，cboot0，carisc

package 目录存放 target 机器上的软件包源码和编译规则，目录按照目标软件包的功能进行分类。
package/
    ├── allwinner
    ├── base-files
    ├── devel
    ├── dragonst
    ├── firmware
    ├── kernel
    ├── ......
    └── utils
    
prebuild 目录存放预编译交叉编译器，目录结构如下。
prebuilt/
    └── gcc
   	 	└── linux-x86
            ├── aarch64
            │ 	├── toolchain-sunxi-musl
            │ 	└── toolchain-sunxi-glibc
            ├── arm
            │ 	├── toolchain-sunxi-arm9-glibc
            │ 	├── toolchain-sunxi-arm9-musl
            │ 	├── toolchain-sunxi-glibc
            │ 	├── toolchain-sunxi-musl
            └── host
                └── host-toolchain.tx
                
scripts 目录用于存放一些构建编译相关的脚本

target 目录用于存放目标板相关的配置以及 sdk 和 toolchain 生成的规格。
target/
    ├── allwinner
    ├── Config.in
    ├── imagebuilder
    ├── Makefile
    ├── sdk
    └── toolchain
快捷跳转命令：cdevice    

toolchain 目录
toolchain 目录包含交叉工具链构建配置、规则。
toolchain/
    ├── binutils
    ├── Config.in
    ├── fortify-headers
    ├── gcc
    ├── gdb
    ├── glibc
    ├── info.mk
    ├── insight
    ├── kernel-headers
    ├── Makefile
    ├── musl
    └── wrappe
    
 tools 目录
 tools 目录用于存放 host 端工具的编译规则。
 tools/
    ├── autoconf
    ├── automake
    ├── aw_tools
    ├── b43-tools
    ├── ......
    
out 目录用于保存编译相关的临时文件和最终镜像文件，编译后自动生成此目录，例如编译方案
r328s2-perf1。
out/
    ├── r328s2-perf1
    └── host
```

