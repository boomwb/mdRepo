HT82V38是一个完整的CCD成像应用的模拟信号处理器。它具有3通道架构，设计用于采样和处理三线性彩色CCD阵列的输出。每个通道包括一个输入钳夹、相关双采样器（CDS）、偏移DAC和可编程增益放大器（PGA），多路复用到一个高性能的16位A/D转换器。CDS放大器可被禁用，用于传感器，如接触图像传感器（CIS）和CMOS主动像素传感器，CMOS主动像素传感器不需要CDS。

SHA 是sample and hold amplifier 的缩写。是最常用的AD转换方式之一。

简单讲，就是采样后保持一段时间再采样，可以在保持的时间内做AD转换，

防止数据被新的输入破坏，但这种方法并不能区分噪声和信号。

CDS是correlated double sampling的缩写，是一种降噪AD转换方式。

每次采样都要把输入和参考电压做减法，相当于是两次采样一次输出。在图形这里主要用于降噪点。

 

**16位数字输出被多路复用成一个8位输出字，并使用两个读取周期进行访问**。内部寄存器通过三线线串行接口进行编程，并提供增益、偏移量和操作模式的调整。

 

 

 

 ![image-20240515141409803](https://raw.githubusercontent.com/boomwb/mdRepo/main/202405151414862.png)

 

 

 

 

 

![image-20240515141427475](https://raw.githubusercontent.com/boomwb/mdRepo/main/202405151414518.png)



![image-20240515141453578](https://raw.githubusercontent.com/boomwb/mdRepo/main/202405151414669.png)

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

决定最大额定特征：

Supply Voltage ..........................VSS 0.3V to VSS +4.3V 

Storage Temperature ........................... -50 度 to 125度 

Input Voltage .............................VSS 0.3V to VDD+0.3V 

Operating Temperature ..............................0 to 70 



交流特征

直流特征

 

时序规范

![image-20240515141525059](https://raw.githubusercontent.com/boomwb/mdRepo/main/202405151415132.png)



 ![image-20240515141543586](https://raw.githubusercontent.com/boomwb/mdRepo/main/202405151415639.png)



时序图：

 

 

 

 

8个8 bit的内部寄存器

配置寄存器控制芯片的工作模式和偏置电压

MUX寄存器控制采样通道的顺序

PGA寄存器和补偿寄存器各有3个，分别对红、绿、蓝3个通道做增益控制（可编程增益放大器）和信号补偿（偏移量）

![image-20240515141611674](https://raw.githubusercontent.com/boomwb/mdRepo/main/202405151416729.png)

![img](G:\JianYun\我的坚果云\Typora_Work\Typora_Pic\wps6.jpg)

 

配置寄存器

![img](G:\JianYun\我的坚果云\Typora_Work\Typora_Pic\wps7.jpg) 

配置寄存器控制工作模式和偏置水平。

D6控制参考钳位电压。将此位低变化偏移设置为高Z，允许从外部电源驱动偏移。

D5位将配置HT82V38为三路(高)的操作模式。

D4设置为1则工作在CDS模式，置为0将启用SHA模式。

D3位应始终设置为0

D2控制断电模式。设置位D2=1将使HT82V38进入一个非常低功率的睡眠模式。当HT82V处于电源关闭状态时，所有寄存器内容都被保留。

D1控制全尺寸输入范围。D1=1，全比例输入范围为2V，D1=0全比例输入范围为1.6V。

D0控制HT82V38的输出模式。设置位D0高将启用一个单字节输出模式，其中只有16位ADC中的8个msb将被输出。如果位D0设置为低，则16位ADC输出被复用成两个字节。

 

 

 

多路复用寄存器

 

 

 

 

 

 

![img](G:\JianYun\我的坚果云\Typora_Work\Typora_Pic\wps8.jpg) 

 

 

 

 

复用寄存器控制采样信道顺序。

D8位应该始终设置较低。

D7是在3通道模式下工作时使用的。设置位D7高将对MUX进行排序，首先采样红色通道，然后是绿色通道，然后是蓝色通道。在这种模式下，CDSCLK2上升边总是会重置MUX，以首先对红色通道进行采样（参见时序图）。当位D7设置为低时，通道顺序反转为蓝色第一、绿色第二和红色三。CDSCLK2上升边缘脉冲总是重置MUX以首先采样蓝色通道。

位D6、D5和D4在1通道模式下操作时使用。位D6被设置为较高，以采样红色通道。位D5被设置为较高，以采样绿色通道。位D4被设置为较高，以采样蓝色通道。

MUX在1通道模式下将保持静止。

位D3到位D0控制4位DAC钳位电压，从0.45V到2.7V。

 

 

 

![img](G:\JianYun\我的坚果云\Typora_Work\Typora_Pic\wps9.jpg) 

有三个PGA寄存器（RGB）用于分别编程红色、绿色和蓝色通道的增益。

每个寄存器中的位D8、D7和D6必须设置为0，

位D5到D0以64个增量控制增益范围。（0011 1111）非线性的

PGA寄存器的编码是直二进制的，所有零字对应于最小增益设置（1x），所有一个字对应于最大增益设置（5.85x）。PGA的增益范围从1（0 dB）到5.85（15.3 dB）![img](G:\JianYun\我的坚果云\Typora_Work\Typora_Pic\wps10.jpg)

 

 

 

 

 

 

有三个PGA寄存器用于分别编程红色、绿色和蓝色通道中的偏移量。

 

 

 

![img](G:\JianYun\我的坚果云\Typora_Work\Typora_Pic\wps11.jpg) 

位D8到D0控制从250mV到+250mV的偏移范围。

D8作为符号位。0表示正，1表示负号；

0.98*255；

 

 

 