SET PATH=D:\Keil5\ARM\ARMCLANG\Bin;C:\Program Files (x86)\Common Files\Oracle\Java\javapath;C:\Windows\system32;C:\Windows;C:\Windows\System32\Wbem;C:\Windows\System32\WindowsPowerShell\v1.0\;C:\Windows\System32\OpenSSH\;C:\Program Files (x86)\NVIDIA Corporation\PhysX\Common;C:\Program Files\NVIDIA Corporation\NVIDIA NvDLISR;C:\WINDOWS\system32;C:\WINDOWS;C:\WINDOWS\System32\Wbem;C:\WINDOWS\System32\WindowsPowerShell\v1.0\;C:\WINDOWS\System32\OpenSSH\;D:\Program Files\Git\cmd;D:\Program Files\doxygen\bin;D:\Program Files (x86)\Graphviz2.38\bin;C:\Program Files\IVI Foundation\VISA\Win64\Bin\;C:\Program Files (x86)\IVI Foundation\VISA\WinNT\Bin;C:\Program Files (x86)\STMicroelectronics\STM32 ST-LINK Utility\ST-LINK Utility;C:\Users\Soway\AppData\Local\Programs\Python\Python38\Scripts\;C:\Users\Soway\AppData\Local\Programs\Python\Python38\;C:\Users\Soway\AppData\Local\Microsoft\WindowsApps;;D:\VS\Microsoft VS Code\bin;D:\Program Files\python_pycharm\PyCharm Community Edition 2020.1.2\bin;
SET CPU_TYPE=GD32E230G8
SET CPU_VENDOR=GigaDevice
SET UV2_TARGET=GD32E230C_EVAL
SET CPU_CLOCK=0x00B71B00
"D:\Keil5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\output\gd32e230_it.__i"
"D:\Keil5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\output\main.__i"
"D:\Keil5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\output\systick.__i"
"D:\Keil5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\output\common.__i"
"D:\Keil5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\output\modbus_asc.__i"
"D:\Keil5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\output\modbus_ascii.__i"
"D:\Keil5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\output\para.__i"
"D:\Keil5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\output\flash.__i"
"D:\Keil5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\output\timer.__i"
"D:\Keil5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\output\system_gd32e230.__i"
"D:\Keil5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\output\gd32e230_adc.__i"
"D:\Keil5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\output\gd32e230_cmp.__i"
"D:\Keil5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\output\gd32e230_crc.__i"
"D:\Keil5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\output\gd32e230_dbg.__i"
"D:\Keil5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\output\gd32e230_dma.__i"
"D:\Keil5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\output\gd32e230_exti.__i"
"D:\Keil5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\output\gd32e230_fmc.__i"
"D:\Keil5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\output\gd32e230_fwdgt.__i"
"D:\Keil5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\output\gd32e230_gpio.__i"
"D:\Keil5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\output\gd32e230_i2c.__i"
"D:\Keil5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\output\gd32e230_misc.__i"
"D:\Keil5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\output\gd32e230_pmu.__i"
"D:\Keil5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\output\gd32e230_rcu.__i"
"D:\Keil5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\output\gd32e230_rtc.__i"
"D:\Keil5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\output\gd32e230_spi.__i"
"D:\Keil5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\output\gd32e230_syscfg.__i"
"D:\Keil5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\output\gd32e230_timer.__i"
"D:\Keil5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\output\gd32e230_usart.__i"
"D:\Keil5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\output\gd32e230_wwdgt.__i"
"D:\Keil5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\output\gd32e230c_eval.__i"
"D:\Keil5\ARM\ARMCLANG\Bin\ArmAsm" --Via ".\output\startup_gd32e230._ia"
"D:\Keil5\ARM\ARMCLANG\Bin\ArmLink" --Via ".\output\GD32E230C_EVAL.lnp"
fromelf --bin .\output\GD32E230C_EVAL.axf --output bin/GD32_boot.bin
