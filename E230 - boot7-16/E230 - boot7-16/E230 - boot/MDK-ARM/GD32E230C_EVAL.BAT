SET PATH=D:\Keil_v5\ARM\ARMCLANG\Bin;C:\Program Files (x86)\Common Files\Oracle\Java\javapath;C:\WINDOWS\system32;C:\WINDOWS;C:\WINDOWS\System32\Wbem;C:\WINDOWS\System32\WindowsPowerShell\v1.0\;C:\Program Files\IVI Foundation\VISA\Win64\Bin\;C:\Program Files (x86)\IVI Foundation\VISA\WinNT\Bin\;C:\Program Files (x86)\IVI Foundation\VISA\WinNT\Bin;C:\WINDOWS\System32\OpenSSH\;D:\Program Files\Git\cmd;C:\Program Files (x86)\Pico Technology\PicoScope6\;C:\Users\wjx\AppData\Local\Microsoft\WindowsApps;
SET ARMCC6_ASMOPT=--diag_suppress=9931
SET ARMCC6_CCOPT=--diag_suppress=9931
SET ARMCC6_LINKOPT=--diag_suppress=9931
SET CPU_TYPE=GD32E230G8
SET CPU_VENDOR=GigaDevice
SET UV2_TARGET=GD32E230C_EVAL
SET CPU_CLOCK=0x00B71B00
"D:\Keil_v5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\output\gd32e230_it.__i"
"D:\Keil_v5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\output\main.__i"
"D:\Keil_v5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\output\flash.__i"
"D:\Keil_v5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\output\usart.__i"
"D:\Keil_v5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\output\gd32e230_systick.__i"
"D:\Keil_v5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\output\dmaconfig.__i"
"D:\Keil_v5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\output\commonfunctions.__i"
"D:\Keil_v5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\output\system_gd32e230.__i"
"D:\Keil_v5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\output\gd32e230_adc.__i"
"D:\Keil_v5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\output\gd32e230_cmp.__i"
"D:\Keil_v5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\output\gd32e230_crc.__i"
"D:\Keil_v5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\output\gd32e230_dbg.__i"
"D:\Keil_v5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\output\gd32e230_dma.__i"
"D:\Keil_v5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\output\gd32e230_exti.__i"
"D:\Keil_v5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\output\gd32e230_fmc.__i"
"D:\Keil_v5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\output\gd32e230_fwdgt.__i"
"D:\Keil_v5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\output\gd32e230_gpio.__i"
"D:\Keil_v5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\output\gd32e230_i2c.__i"
"D:\Keil_v5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\output\gd32e230_misc.__i"
"D:\Keil_v5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\output\gd32e230_pmu.__i"
"D:\Keil_v5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\output\gd32e230_rcu.__i"
"D:\Keil_v5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\output\gd32e230_rtc.__i"
"D:\Keil_v5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\output\gd32e230_spi.__i"
"D:\Keil_v5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\output\gd32e230_syscfg.__i"
"D:\Keil_v5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\output\gd32e230_timer.__i"
"D:\Keil_v5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\output\gd32e230_usart.__i"
"D:\Keil_v5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\output\gd32e230_wwdgt.__i"
"D:\Keil_v5\ARM\ARMCLANG\Bin\ArmAsm" --Via ".\output\startup_gd32e230._ia"
"D:\Keil_v5\ARM\ARMCLANG\Bin\ArmLink" --Via ".\output\GD32E230C_EVAL.lnp"
"D:\Keil_v5\ARM\ARMCLANG\Bin\fromelf.exe" ".\output\GD32E230C_EVAL.axf" --i32combined --output ".\output\GD32E230C_EVAL.hex"
fromelf --bin .\output\GD32E230C_EVAL.axf --output bin/TH_boot.bin
