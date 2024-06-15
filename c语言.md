Typora+PicGo+github上传图片

https://juejin.cn/post/6844903768782290957

# 位运算(&、|、^、~、>>、<<)

![image-20240516113825147](https://raw.githubusercontent.com/boomwb/mdRepo/main/img/202405161138244.png)

# 进制输出

```
%o
%x
%d
```

# 关键字:

**static静态变量**

```
一、 static全局变量与普通的全局变量有什么区别 ?
全局变量(外部变量)的说明之前再冠以static 就构成了静态的全局变量。
   全局变量本身就是静态存储方式， 静态全局变量当然也是静态存储方式。 这两者在存储方式上并无不同。
这两者的区别在于非静态全局变量的作用域是整个源程序， 当一个源程序由多个源文件组成时，非静态的全局变量在各个源文件中都是有效的。 而静态全局变量则限制了其作用域， 即只在定义该变量的源文件内有效， 在同一源程序的其它源文件中不能使用它。由于静态全局变量的作用域局限于一个源文件内，只能为该源文件内的函数公用，因此可以避免在其它源文件中引起错误。
	static全局变量只初使化一次，防止在其他文件单元中被引用;
	
 二、static局部变量和普通局部变量有什么区别 ？
   把局部变量改变为静态变量后是改变了它的存储方式即改变了它的生存期。把全局变量改变为静态变量后是改变了它的作用域，限制了它的使用范围。
  static局部变量只被初始化一次，下一次依据上一次结果值
  
 所有未加static前缀的全局变量和函数都具有全局可见性，其它的源文件也能访问。如果加了static，就会对其它源文件隐藏。
   static的第二个作用是保持变量内容的持久。存储在静态数据区的变量会在程序刚开始运行时就完成初始化，也是唯一的一次初始化。共有两种变量存储在静态存储区：全局变量和static变量，只不过和全局变量比起来，static可以控制变量的可见范围，说到底static还是用来隐藏的.
   #include <stdio.h>
 
     int fun(void){   
         static int count = 10;    // 事实上此赋值语句从来没有执行过
         return count--;
     }
 
     int count = 1;
 
     int main(void)
     {    
         printf("global\t\tlocal static\n");
         for(; count <= 10; ++count)
             printf("%d\t\t%d\n", count, fun());    
 
         return 0;
     }
 结果为:
 global    local static
 1			10
 2			9
 3			8
 4			7
 5			6
 6			5
 7			4
 8			3
 9			2
 10			1
 
     
   static的第三个作用是默认初始化为0。其实全局变量也具备这一属性，因为全局变量也存储在静态数据区。在静态数据区，内存中所有的字节默认值都是0x00，某些时候这一特点可以减少程序员的工作量。比如初始化一个稀疏矩阵，我们可以一个一个地把所有元素都置0，然后把不是0的几个元素赋值。如果定义成静态的，就省去了一开始置0的操作。再比如要把一个字符数组当字符串来用，但又觉得每次在字符数组末尾加’\0’太麻烦。如果把字符串定义成静态的，就省去了这个麻烦，因为那里本来就是’\0’。
	
```

```
在 C 语言中，static 关键字的作用如下：
1. static 修饰的静态局部变量直到程序运行结束以后才释放，延长了局部变量的生命周期。
2. static 的修饰全局变量只能在本文件中访问，不能在其它文件中访问。
3. static 修饰的函数只能在本文件中调用，不能被其他文件调用。
```



**extern**

```
extern int a;
常在.h文件中
显式的声明了a的存储空间是在程序的其他地方分配的，在文件中其他位置或者其他文件中寻找a这个变量。

static 表示静态的变量，限制此变量作用域在一个源文件内，其他文件不能用extern来引用此变量，不仅适用于变量，函数也可以.
```

**const char * **

```

一.const char *s  ||   char const *s
是指向常量的指针,*s是不变的, s是可以改变的
s所指向的数据(即*s)由于const的修饰而不可通过指针s去修改。

二.char *const s
s是个不可修改的指针，但可通过指针s去修改s所指向的数据(即*s)。

```





## 函数指针(钩子函数/回调函数)

指向函数的指针变量

钩子函数/回调函数其实是个 **指针**

```
int (*p)(int a,int b)

函数指针类型:
int (*)(int,int)

用法:
int add(int a,int b)
{
	return a+b;
}
int (*p)(int,int)=NULL;
p=add;
p(10,20); //通过指针调用



```

**注意1:**函数指针所指向的函数一定要保持函数的返回值类型，函数参数个数，类型一致。

**注意2 typedef定义可以简化函数指针的定义:**



## 指针函数

函数的返回值是一个指针, 还是一个函数

```
int *p(int a,int b);
函数p(int,int)的返回值为int *类型
```



## 大小端

#### 一、什么是大小端？

对于一个由2个字节组成的16位整数，在[内存](https://so.csdn.net/so/search?q=内存&spm=1001.2101.3001.7020)中存储这两个字节有两种方法：一种是将低序字节存储在起始地址，这称为小端(little-endian)字节序；另一种方法是将高序字节存储在起始地址，这称为大端(big-endian)字节序。

[百度](https://baidu.com)

<img src="https://raw.githubusercontent.com/boomwb/mdRepo/main/img/202405151527664.png" alt="image-20240515152705614" style="zoom:67%;" />

假如现有一32位int型数0x12345678，那么其MSB(Most Significant Byte，最高有效字节)为0x12，其LSB (Least Significant Byte，最低有效字节)为0x78，在CPU内存中有两种存放方式：（假设从地址0x4000开始存放）

<img src="https://raw.githubusercontent.com/boomwb/mdRepo/main/img/202405151527213.png" alt="image-20240515152730173" style="zoom:67%;" />

**总结：**

**大端是高字节存放到内存的低地址**

**小端是高字节存放到内存的高地址**

#### 二、如何确定大小端

有些CPU公司用大端（譬如C51单片机）；有些CPU用小端（譬如ARM）。（大部分是用小端模式，大端模式的不算多）。于是乎我们写代码时，当不知道当前环境是用大端模式还是小端模式时就需要用代码来检测当前系统的大小端。下面给出用程序判断大小端的两种方法：

```
#include <stdio.h>
 
// 共用体中很重要的一点：a和b都是从u1的低地址开始存放的。
// 假设u1所在的4字节地址分别是：0、1、2、3的话，那么a自然就是0、1、2、3；
// b所在的地址是0而不是3.
 
union myunion
{
	int a;
	char b;
};
 
// 如果是小端模式则返回1，小端模式则返回0
int is_little_endian(void)
{
	union myunion u1;
	u1.a = 0x12345678;				// 地址0的那个字节内是0x78（小端）或者0x12（大端）
    if(0x78 == u1.b)
        return 1;
    else if(0x12 == u1.b)
	    return 0;
}
 
int is_little_endian2(void)
{
	int a = 0x12345678;
	char b = *((char *)(&a));		// 指针方式其实就是共用体的本质
	if(0x78 == b)
        return 1;
    else if(0x12 == b)
	    return 0;
}
 
 
int main(void)
{
	int i = is_little_endian2();
	//int i = is_little_endian();
	if (i == 1)
	{
		printf("小端模式\n");
	}
	else
	{
		printf("大端模式\n");
	}
	
	return 0;
}

```



下面给出三种确定大小端错误的方案:

```
	// 强制类型转换
int a;
char b;
a = 1;
b = (char)a;
printf("b = %d.\n", b);
```

```
	// 移位
int a, b;
a = 1;
b = a >> 1;
printf("b = %d.\n", b);
```

```
	// 位与
int a = 1;
int b = a & 0xff;		// 也可以写成：char b = a & 0x01;
printf("b = %d.\n", b);
```

位与、移位、强制类型转换等运算是编译器提供的运算，这个运算是高于内存层次的（或者说这些运算在二进制层次具有可移植性，也就是说&的时候一定是高字节&高字节，低字节&低字节，和二进制存储无关）。

#### **怎么测试我的电脑是小端模式还是大端模式呢？**

1.将int 48存起来，然后取得其地址，再将这个地址转为char* 这时候，如果是小端存储，那么char*指针就指向48；
48对应的ASCII码为字符‘0’；

```
void judge_bigend_littleend1()
{
    int i = 48;
    int* p = &i;
    char c = 0;
    c = *((char*)p);

    if (c == '0')
        printf("小端\n");
    else
        printf("大端\n");
}
```

2.定义变量int i=1;将 i 的地址拿到，强转成char*型，这时候就取到了 i 的低地址，这时候如果是1就是小端存储，如果是0就是大端存储.

```
void judge_bigend_littleend2()
{
    int i = 1;
    char c = (*(char*)&i);

    if (c)
        printf("小端\n");
    else
        printf("大端\n");
}
```

3.定义联合体，一个成员是多字节，一个是单字节，给多字节的成员赋一个最低一个字节不为0，其他字节为0 的值，再用第二个成员来判断，如果第二个字节不为0，就是小端，若为0，就是大端。

```
void judge_bigend_littleend3()
{
    union
    {
        int i;
        char c;
    }un;
    un.i = 1;

    if (un.c == 1)
        printf("小端\n");
    else
        printf("大端\n");
}
```

### age.1

```
编程:
要将一个字符数组char buf[2] = {0x2C, 0x01}中的元素合并成一个单独的十进制数，假设数组中存储的是一个大端序（big-endian）的数值，
那么我们可以按照权重合并每个字节。在这个特定的例子中，0x2C是高位字节，0x01是低位字节。
大端序意味着数值的高位字节存储在内存的低地址上，而数值的低位字节存储在高地址上。考虑到这一点，我们可以将高位字节左移8位（即乘以256），然后与低位字节进行按位或操作，以合成完整的数值。
#include <stdio.h>
int main() {
    char buf[2] = {0x2C, 0x01}; 
    // 将char强制转换为unsigned char以避免符号扩展，然后进行合成操作
    unsigned int num = ((unsigned char)buf[0] << 8) | (unsigned char)buf[1];   
    printf("合成的十进制数是: %u\n", num);
    return 0;
}
```





# 结构体

## 字节对齐

当声明一个结构体时，编译器通常会按照特定的内存对齐规则来布局成员，这些规则旨在提高访问内存的效率。在大多数系统上，数据类型的自然对齐是它的大小，即 `int` 类型通常需要对齐到4字节边界，`char` 类型对齐到1字节边界

```
struct Info 
{
	char i;
	int j;
}III;
	-> 8
typedef struct _userinfo//保存的信息
{
	unsigned char id;
	unsigned char port;;		
	struct Info III;
}USERINFO;
	-> 12

```

1. **`char i`**: 1字节大小，不需要额外对齐。
2. **内存填充** (Padding): 由于 `int` 类型通常需要在4字节对齐的边界上，编译器可能会在 `char i` 之后插入3个字节的内存填充，以确保 `int j` 从4字节边界开始。
3. **`int j`**: 4字节大小，按4字节边界对齐。

因此，`struct Info III` 的总大小是8字节：1字节用于 `char i`，3字节的填充，然后是4字节的 `int j`.

**对于 `struct _userinfo` (`USERINFO`)：**

- `unsigned char id` 占用 1 字节，无需额外填充。
- `unsigned char port` 占用 1 字节。直到这里，结构体已经使用了 2 字节，但是紧接着的是 `struct Info` 类型，它要求4字节对齐，因此编译器将添加2字节的填充，以确保 `struct Info III` 从下一个4字节对齐的边界地址开始。

那么 `struct _userinfo` 的总大小就是 `struct Info` 的大小 8 字节加上 `unsigned char id`, `unsigned char port`，以及在它们和 `struct Info III` 之间的 2 字节填充，加起来总共是 12 字节。



```
HAL_StatusTypeDef WriteStructToFlash(uint32_t address, UserInfo *dataStruct, uint16_t dataSize) {
    HAL_StatusTypeDef status = HAL_OK;
    // 首先，需要擦除Flash
    FLASH_EraseInitTypeDef EraseInitStruct;
    uint32_t PAGEError = 0;
    EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
    EraseInitStruct.PageAddress = address;
    EraseInitStruct.NbPages     = 1; //擦除一个Page

    // 解锁Flash
    HAL_FLASH_Unlock();

    // 擦除Flash
    status = HAL_FLASHEx_Erase(&EraseInitStruct, &PAGEError);
    if (status != HAL_OK) {
        return status;
    }

    // 写入数据
    uint32_t *dataPointer = (uint32_t*)dataStruct;
    while (dataSize > 0) {
        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, address, *(uint32_t*)(dataPointer)) == HAL_OK) {
            address += 4; // 地址增加4字节
            dataPointer++; // 数据指针增加1
            dataSize -= 4; // 剩余数据大小减少4字节
        } else {
            // 错误处理
            status = HAL_ERROR;
            break;
        }
    }
    // 锁定Flash
    HAL_FLASH_Lock();

    return status;
}



void ReadStructFromFlash(uint32_t address, UserInfo *dataStruct, uint16_t dataSize) {
    uint32_t *dataPointer = (uint32_t*)dataStruct;
    while (dataSize > 0) {
        *dataPointer = *(__IO uint32_t*)address;
        address += 4;
        dataPointer++;
        dataSize -= 4;
    }
}
```

进制打印

```
    printf( "%d\n",liu.num);
    printf( "%#x\n",liu.num); //16进制
    printf( "0x%x\n",liu.num);
    %o       8进制
```

```
//原始尺寸
uint8_t  str_OrginalSize[] = {0xa5,0x43,0x02,0x01,0x01, 0x47, 0x00,  0x0d, 0x0a};

char my_OrginalSize_key_handler(mui_t * ui)
{
   char cur_pos = ui->form_scroll_top;

	int ret;
   if( (cur_pos == 0 || cur_pos==1 || cur_pos==2 || cur_pos==3 )  && (key_mark_read >> KEY_OK_MARK_BIT) & 0x01 )
   {
		key_mark_read &= ~(0x01 << KEY_OK_MARK_BIT);//清楚ok 标志
        
        switch(cur_pos)
        {
            case 0:
                str_OrginalSize[4]=0x01;
                break;
            case 1:
                str_OrginalSize[4]=0x02;
                break;
            case 2:
                str_OrginalSize[4]=0x03;
                break;
            case 3:
                str_OrginalSize[4]=0x04;
                break;
            default:
                break;
        }
        uint16_t sum_of_buf = 0;
        //计算buf[1]到buf[4]的sum累加到buf[5]和buf[6]
        for(int i=1; i<=4 ;i++)
        {
            sum_of_buf += str_OrginalSize[i];
        }
        str_OrginalSize[5]=sum_of_buf&0xff; //取低8位
        str_OrginalSize[6]=(sum_of_buf<<8)&0xff; 
        

        str_OrginalSize[7]=0x0d;
        str_OrginalSize[8]=0x0a;


	  	if( xSemaphoreTake( uart0_mutex, ( TickType_t ) 0xffff ) == pdTRUE )
		{
			 // 发送命令
			ret = my_send_cmd_data(str_OrginalSize, 9, TRUE, 10, 0x43); //等待10*5ms
			xSemaphoreGive( uart0_mutex );
		}
		if(__OK_RET == ret)
		{
			printf("my send successful --\r\n");
			if( rx_buffer[2] == 0x00)
			{
				printf("rx_buffer[2] == 0x00? buf[2]%d --\r\n",rx_buffer[2]);
				return __OK_RET;
			}
			else
			{
				return __ERROR_RET;
			
			}
		
		}
   }
	
   return 0;
}

void __CopyFunc_OrginalSize_ui_Callback(mui_t * ui)
{

    u8g2_t *u8g2 = ui->graphics_data;
    char cur_pos = mui_GetCurrentCursorFocusPosition(ui);
    char top = ui->form_scroll_top, i;
    char *tbuf;
    tbuf = (char *)getOriginalSize_str(tbuf, top);

    u8g2_ClearBuffer(u8g2);
    u8g2_DrawUTF8(u8g2, 39, 14, "原始尺寸 ");
	
	
	
    u8g2_DrawTriangle(u8g2, 128, 24, 120, 29, 120, 19);

    u8g2_DrawXBMP(u8g2, 0, 20, 8, 9, left_jiantou_icon);

    u8g2_DrawUTF8(u8g2, 20, 30, tbuf);
    char index = ui->form_scroll_top + mui_GetCurrentCursorFocusPosition(ui);
   
    draw_sel_ui_mark_icon(ui, cfs._OriginalSizeSel-1);
	

 
//    char ret = OrginalSize_key_handler(ui);
	
	 char ret = my_OrginalSize_key_handler(ui);
	 if(ret != 0)
     {
        if(ret == __OK_RET)
        {
//			printf("come here --------\r\n");
//			if( xSemaphoreTake( u8g2_mutex, ( TickType_t ) 0xffff ) == pdTRUE )
//			{
							
				cfs._OriginalSizeSel= index+1;
                cfs._Temp_OriginalSizeSel = cfs._OriginalSizeSel;			
				SAVE_WAIT_MESSAGE(u8g2);
				vTaskDelay(100);
				//key_mark_read = 0;
				//xSemaphoreGive( u8g2_mutex );
							
//			} 
				     
		}
		else if(ret == __ERROR_RET)
		{
			vTaskDelay(150);
             Go_back_form(ui, MENUS);
		}
    }	 
	 


    
}
```

