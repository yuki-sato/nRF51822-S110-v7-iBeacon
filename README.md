nRF51822-S110-v7-iBeacon
========================

SimpleWay to generate iBeacon with nRF51822 using S110 v7  

after S110 version7, sdk are distributed over CMSIS.  
This is very simple way to generate iBeacon advertisement.  
Nothing do others( like turning on leds and uarts).  

###This is how i create Keil project.  
  

  
1) Create newn project using "RF51822xxAA"  
2) add S110 with CMSIS (include related others)  


Project Configration  
3) Xtal set to 16Mhz 
4) ROM 0x16000 - 0x29000  RAM 0x20002000　- 0x6000  
5) UseMicroLibにチェック  

Project Configration C/C++
6) Define NRF51 BLE_STACK_SUPPORT_REQD S110 SOFTDEVICE_PRESENT  
7) Add --c99 to MiscControls  

Project Configration ASM  
8) Same define on define  

Project Configration Linker  
9) Check Use memory layout from target dialog  
10) Debugger set to JLINK. 

Debugger Setting  
11) Port JTAG to SW  
12) FlashDownload RAM 0x20000000 - 0x2000
13) Check ResetAndRun

Finished!!  
Now You can Flash.( Do not forget flash also S110 )

##(japanese)  
ターゲットを nRF51822xxAAでプロジェクト作成  
CMSISにS110を追加。関係するファイルも追加  

プロジェクト設定にて  
Xtalを16  
ROMを0x16000 - 0x29000  
RAMを0x20002000　- 0x6000  
UseMicroLibにチェック  

C/C++
Defineに  
NRF51 BLE_STACK_SUPPORT_REQD S110 SOFTDEVICE_PRESENT  
MiscControls に  
--c99  
ASMにも同じdefine  

Linker  
Use memory layout from target dialog  

DebuggerにJLINKを指定。  
SettingでPortをJTAGからSWにする  
FlashDownloadのRAMは  
0x20000000 - 0x2000  
ResetAndRunにチェック入れると便利  

以上！
