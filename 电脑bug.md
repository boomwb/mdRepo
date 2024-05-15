坑爹bug(ctrl+ 空格 输入法的问题)

![image-20240515135957531](https://raw.githubusercontent.com/boomwb/mdRepo/main/img202405151359565.png)

```
Windows Registry Editor Version 5.00

[HKEY_CURRENT_USER\Control Panel\Input Method\Hot Keys\00000010]
"Key Modifiers"=hex:00,c0,00,00
"Virtual Key"=hex:ff,00,00,00

[HKEY_CURRENT_USER\Control Panel\Input Method\Hot Keys\00000070]
"Key Modifiers"=hex:00,c0,00,00
"Virtual Key"=hex:ff,00,00,00

[HKEY_USERS\.DEFAULT\Control Panel\Input Method\Hot Keys\00000010]
"Key Modifiers"=hex:02,c0,00,00
"Target IME"=hex:00,00,00,00
"Virtual Key"=hex:ff,00,00,00

[HKEY_USERS\.DEFAULT\Control Panel\Input Method\Hot Keys\00000070]
"Key Modifiers"=hex:02,c0,00,00
"Target IME"=hex:00,00,00,00
"Virtual Key"=hex:ff,00,00,00
```













