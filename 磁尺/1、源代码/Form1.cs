using System;
using System.Collections.Generic;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Management;
using System.Text.RegularExpressions;
using System.Threading;
using MagneticRuler.Properties;
using System.Runtime.InteropServices;

namespace MagneticRuler
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
            Control.CheckForIllegalCrossThreadCalls = false;                    // 充许子线程访问控件 但安全性会降低
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            MyPara.Title = "Magnetic Ruler";

            this.Text = MyPara.Title + DateTime.Now.ToString("  HH:mm:ss");

            MyPara.Modbus.ReadData.SenorID = 1;
            MyPara.Modbus.ReadData.IsIDTrue = false;

            MyPara.Modbus.Send.SensorID = 1;
            MyPara.Modbus.Send.Data = new uint[20];
            MyPara.Modbus.Rece.Data = new uint[20];

            MyPara.Modbus.Send.SendSema = new Semaphore(0, 1);                  // 发送信号量
            MyPara.Modbus.Send.ReceSema = new Semaphore(0, 1);                  // 接收完成信号量
            Thread thread = new Thread(new ThreadStart(SendReceMethod));        // 创建接收发线程       
            thread.IsBackground = true;                                         // 后台线程 不设置关闭程序后线程依然会运行
            thread.Start();                                                     // 启动线程

            MyPara.RunState.Model = 0;                                          // 运行模式初始值
            MyPara.RunState.IsSerialUseMask = 0x40000000;                       // 正在读取数据掩码定义为第30位(0：读写空闲状态 1：当前正在读写数据)
            MyPara.RunState.ReadWriteMast = 0x80000000;                         // 读写掩码主义为第31位(0：读模式1：写模式)

            // 按F1显示CHM帮助文档
            MyPara.HelpPath = System.IO.Directory.GetParent(System.Environment.CurrentDirectory).FullName + @"\Help\磁尺软件操作手册.pdf";
            helpProvider1.HelpNamespace = MyPara.HelpPath;
            helpProvider1.SetShowHelp(this, true);

            MyPara.Serial.IsOpen = false;                                       // 串口默认为关闭状态
            MyPara.Serial.IsDataReceived = false;                               // 串口是否接收到数据
            MyPara.Serial.IsRefresh = false;
            RefreshSerial();                                                    // 刷新串口

            RecoverySetting();                                                  // 恢复历史设置

            if (WriteBaud.SelectedIndex == -1)
                WriteBaud.Text = "9600";
            if (WriteParity.SelectedIndex == -1)
                WriteParity.Text = "NONE";
            if (WriteCircularCount.SelectedIndex == -1)
                WriteCircularCount.Text = "One";
            if (WriteDirection.SelectedIndex == -1)
                WriteDirection.Text = "Positive";

            MyPara.Modbus.Send.DataID = (int)WriteSensorID.Value;
        }

        public Para MyPara = new Para();                                        // 定义参数结构体

        public struct Para                                                      // 结构体定义
        {
            public string Title;                                                // 标题栏名称
            public runstate RunState;                                           // 运行状态
            public string HelpPath;                                             // 帮助文档路径
            public modbus Modbus;                                               // 代码参数结构体
            public serial Serial;                                               // 串口参数结构体
            public int SerialMode;                                              // 串口通信模式 0：RTU模式 1：ASCII模式
            public bool SerialMatchFlag;                                        // 串口匹配标志
            public bool SerialMatchSuccessful;                                  // 串口匹配成功标志
            public int SenorAddr;                                               // 设备地址
            public Writepara WritePara;                                         // 写参数数据

            public struct runstate
            {
                public uint Model;                                              // 运行模式
                public uint IsSerialUseMask;                                    // 是否正在读写数据掩码   第30位    0：空闲状态       1：正在读写
                public uint ReadWriteMast;                                      // 读写掩码               第31位    0：读模式         1：写模式
            }

            public struct Writepara 
            {
                public int SensorID;
                public int Baud;
                public int Parity;
                public int Mode;
                public int Resolution;
                public int ZeroMode;
                public int Direction;
                public int Reset;
            }

            public struct serial
            {
                public bool IsOpen;
                public bool IsRefresh;
                public bool IsDataReceived;
                public string[] Items;
                public int Baud;
                public int DataBits;
                public string Parity;
                public string Stop;
            }

            public struct modbus
            {
                public data Send;                                            // 传感器发送参数
                public data Rece;                                            // 传感器接收参数
                public readdata ReadData;                                       // 双通道读数据参数

                public struct readdata
                {
                    public bool IsIDTrue;                                       // ID是否正确
                    public bool FirstTime;
                    public bool IsCycleArrive;
                    public bool ReadFinsh;
                    public uint OverTimeCount;                                  // 连续超时次数
                    public uint TimeOut;
                    public uint SenorID;
                    public byte[] Cmd;
                }

                public struct data
                {
                    public Semaphore SendSema;                                  // 发送信号量
                    public Semaphore ReceSema;                                  // 接收完成信号量
                    public bool Flag;                                           // 读写成功标志
                    public int ReadIndex;                                       // 读参数索引
                    public string ReadResults;                                  // 读结果
                    public WriteCmd WriteCmd;
                    public uint SensorID;                                       // 传感器ID
                    public uint FunCode;                                        // 功能码
                    public uint StartAddr;                                      // 起始地址
                    public uint RegNum;                                         // 寄存器数量
                    public uint ByteNum;                                        // 字节数
                    public uint[] Data;                                         // 数据
                    public int DataID;
                    public int DataBaud;
                    public int DataParity;
                    public int DataMode;
                    public int DataDirection;
                    public int DataCircularCount;
                    public uint ErrorNum;                                       // 错误编号

                    public byte[] Cmd;                                          // 命令数组
                    public uint CmdLen;                                         // 命令数组长度

                    public uint CorrectRecLen;                                  // 正确回复长度
                }

                public enum WriteCmd
                {
                    WriteID = 0,
                    WriteBaud,
                    WriteParity,
                    WriteCircularCount,
                    WriteDirection,
                }
            }
        }

        [DllImport("user32", EntryPoint = "HideCaret")]
        private static extern bool HideCaret(IntPtr hWnd);

        public void RecoverySetting()                                           // 恢复历史设置
        {
            try
            {
                SensorID.Value = Settings.Default.SensorID;

                if (SerialCom.Items.Contains(Settings.Default.SerialCom))
                    SerialCom.Text = Settings.Default.SerialCom;

                Baud.Text = Settings.Default.Baud;
                Parity.Text = Settings.Default.Parity;

                WriteSensorID.Value = Settings.Default.WriteID;
                WriteBaud.Text = Settings.Default.WriteBaud;
                WriteParity.Text = Settings.Default.WriteParity;
                WriteCircularCount.Text = Settings.Default.WriteCircularCount;
                WriteDirection.Text = Settings.Default.WriteDirection;
            }
            catch { }
        }
       
        public class Modbus
        {
            public static uint GetLRC(byte[] Data, uint DataLen)
            {
                uint LRC = 0;

                for (int i = 0; i < DataLen; i++)
                    LRC += Data[i];

                LRC = (256 - (LRC % 256)) % 256;
                return LRC;
            }

            public static uint GetCRC(byte[] Data, uint DataLen)
            {
                uint CRCLo = 0xFF;                                              // 高CRC字节初始化
                uint CRCHi = 0xFF;                                              // 低CRC字节初始化 
                uint uIndex = 0;                                                // CRC循环中的索引 

                for (uint i = 0; i < DataLen; i++)                              // 传输消息缓冲区
                {
                    uIndex = CRCLo ^ Data[i];                                   // 计算CRC 
                    CRCLo = CRCHi ^ auchCRCLo[uIndex];
                    CRCHi = auchCRCHi[uIndex];
                }

                return (CRCHi << 8) | CRCLo;
            }

            public static byte[] Rtu2Ascii(byte[] RtuData, uint DataLen)
            {
                return Rtu2AsciiAuto(RtuData, DataLen - 2);
            }

            public static byte[] Ascii2Rtu(byte[] AsciiData, uint AsciiLen)
            {
                uint RtuLen = (AsciiLen - 1) / 2;
                byte[] RtuData = new byte[RtuLen];

                for (int i = 1; i < AsciiLen - 4; i++)
                {
                    if (AsciiData[i] >= '0' && AsciiData[i] <= '9')
                        AsciiData[i] = (byte)(AsciiData[i] - '0');
                    else
                    {
                        if (AsciiData[i] >= 'A' && AsciiData[i] <= 'F')
                            AsciiData[i] = (byte)(AsciiData[i] - 'A' + 10);
                    }
                }

                for (int i = 0; i < RtuLen - 2; i++)
                {
                    RtuData[i] = (byte)((AsciiData[(i << 1) + 1] << 4) + AsciiData[(i << 1) + 2]);
                }

                uint CRC = GetCRC(RtuData, RtuLen - 2);
                RtuData[RtuLen - 2] = (byte)CRC;
                RtuData[RtuLen - 1] = (byte)(CRC >> 8);

                return RtuData;
            }

            public static byte[] Rtu2AsciiAuto(byte[] RtuData, uint DataLen)
            {
                byte[] AsciiData = new byte[2 * DataLen + 5];

                AsciiData[0] = 0x3A;

                for (int i = 0; i < DataLen; i++)
                {
                    AsciiData[(i << 1) + 1] = (byte)(RtuData[i] >> 4);
                    AsciiData[(i << 1) + 2] = (byte)(RtuData[i] & 0x0F);
                }
                uint LRC = GetLRC(RtuData, DataLen);
                AsciiData[2 * DataLen + 1] = (byte)(LRC >> 4);
                AsciiData[2 * DataLen + 2] = (byte)(LRC & 0x0F);

                for (int i = 1; i <= 2 * DataLen + 2; i++)
                {
                    if (AsciiData[i] <= 9)
                        AsciiData[i] = (byte)(AsciiData[i] + 48);
                    else
                        AsciiData[i] = (byte)(AsciiData[i] - 10 + 65);
                }

                AsciiData[2 * DataLen + 3] = 0x0D;                              // ASCII模式结束符
                AsciiData[2 * DataLen + 4] = 0x0A;

                return AsciiData;
            }

            public static byte[] Ascii2RtuData(byte[] AsciiData, uint AsciiLen) // 除帧头帧尾以外的RTU数据
            {
                uint RtuLen = (AsciiLen - 3) / 2;
                byte[] RtuData = new byte[RtuLen];

                for (int i = 1; i < AsciiLen - 2; i++)
                {
                    if (AsciiData[i] >= '0' && AsciiData[i] <= '9')
                        AsciiData[i] = (byte)(AsciiData[i] - '0');
                    else
                    {
                        if (AsciiData[i] >= 'A' && AsciiData[i] <= 'F')
                            AsciiData[i] = (byte)(AsciiData[i] - 'A' + 10);
                    }
                }

                for (int i = 0; i < RtuLen; i++)
                {
                    RtuData[i] = (byte)((AsciiData[(i << 1) + 1] << 4) + AsciiData[(i << 1) + 2]);
                }

                return RtuData;
            }

            static byte[] auchCRCLo = { 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
                                        0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
                                        0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
                                        0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
                                        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
                                        0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
                                        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
                                        0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
                                        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
                                        0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
                                        0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
                                        0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
                                        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
                                        0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
                                        0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
                                        0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
                                        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
                                        0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
                                        0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
                                        0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
                                        0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
                                        0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
                                        0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
                                        0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
                                        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
                                        0x80, 0x41, 0x00, 0xC1, 0x81, 0x40 };

            /* CRC高位字节值表*/
            static byte[] auchCRCHi = { 0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06,
                                        0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD,
                                        0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
                                        0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A,
                                        0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4,
                                        0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
                                        0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3,
                                        0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4,
                                        0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
                                        0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29,
                                        0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED,
                                        0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
                                        0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60,
                                        0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67,
                                        0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
                                        0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68,
                                        0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E,
                                        0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
                                        0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71,
                                        0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92,
                                        0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
                                        0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B,
                                        0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B,
                                        0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
                                        0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42,
                                        0x43, 0x83, 0x41, 0x81, 0x80, 0x40 };
        }

        private void ReadData_Click(object sender, EventArgs e)
        {
            if (btn_SerialOpen.Text == "打开" || btn_SerialOpen.Text == "Open")
                SerialOpen();

            if (!MyPara.Serial.IsOpen)
            {
                if (Language.Text == "EN")
                    MessageBox.Show("串口打开失败", "提示");
                else
                    MessageBox.Show("Serial port opening failed!", "Tip");

                return;
            }

            if (this.ReadData.Text == "开始" || this.ReadData.Text == "Start")
            {
                if (MyPara.Modbus.ReadData.SenorID == 0)
                {
                    ReadData.BackColor = Color.Red;
                    if (Language.Text == "EN")
                        MessageBox.Show("传感器地址不能为0", "提示");
                    else
                        MessageBox.Show("Sensor Address can not be 0", "Tip");
                }
                else
                {
                    GetReadDataCmd();

                    ReadData.Text = Language.Text == "EN" ? "停止" : "Stop";
                    ReadData.BackColor = Color.FromArgb(71, 191, 43);
                    MyPara.Modbus.ReadData.FirstTime = true;
                    timer_ReadData.Interval = 10;                               // 立即执行读操作
                    timer_ReadData.Enabled = true;
                }
            }
            else
                CloseReadData();
        }

        public void GetReadDataCmd()
        {
            MyPara.Modbus.ReadData.Cmd = new byte[8];

            MyPara.Modbus.ReadData.Cmd[0] = (byte)SensorID.Value;
            MyPara.Modbus.ReadData.Cmd[1] = 0x04;
            MyPara.Modbus.ReadData.Cmd[2] = 0x00;
            MyPara.Modbus.ReadData.Cmd[3] = 0x00;
            MyPara.Modbus.ReadData.Cmd[4] = 0x00;
            MyPara.Modbus.ReadData.Cmd[5] = 0x06;
            uint CRC = Modbus.GetCRC(MyPara.Modbus.ReadData.Cmd, 6);
            MyPara.Modbus.ReadData.Cmd[6] = (byte)CRC;
            MyPara.Modbus.ReadData.Cmd[7] = (byte)(CRC >> 8);

            MyPara.Modbus.ReadData.OverTimeCount = 0;
        }

        public void CloseReadData()
        {
            ReadData.Text = Language.Text == "EN" ? "开始" : "Start";
            ReadData.BackColor = Color.White;
            timer_ReadData.Enabled = false;
            MyPara.Modbus.ReadData.IsCycleArrive = false;
        }

        private void timer1_Time_Tick(object sender, EventArgs e)
        {
            this.Text = MyPara.Title + DateTime.Now.ToString("  HH:mm:ss");
        }

        private void btn_SerialOpen_Click(object sender, EventArgs e)
        {
            string Open = Language.Text == "CN" ? "Open" : "打开";

            if (btn_SerialOpen.Text == Open)
                SerialOpen();
            else
                SerialClose();
        }

        protected override void WndProc(ref Message m)                          // 可函数不可删除 用作串口插入或拔出响应中断
        {
            if (m.Msg == 0x0219)                                                // usb串口
            {
                if (m.WParam.ToInt32() == 0x8000 ||                             // 串口接入
                    m.WParam.ToInt32() == 0x8004)                               // 串口拔出
                {
                    timer_RefreshSerialDelay.Enabled = true;                    // 串口接入或拔出后定时器延时10ms再执行刷新操作 软件延时有产生问题
                }
            }
            base.WndProc(ref m);
        }

        public void RefreshSerial()
        {
            string[] SerialNames;
            SerialNames = MulGetHardwareInfo(HardwareEnum.Win32_PnPEntity, "Name"); // 调用方式通过WMI获取COM端口

            MyPara.Serial.IsRefresh = true;                                         // 刷新开始 标记

            if (SerialNames.Length == 0)
            {
                SerialClose();

                SerialCom.Items.Clear();
                SerialCom.Items.Add("Default(COM1)");
                SerialCom.SelectedIndex = 0;
                MyPara.Serial.IsRefresh = false;                                    // 刷新结束 标记

                return;
            }

            string[] NewSerialNames = new string[SerialNames.Length];               // 截取前面6个字符信息 + "..." + 端口号
            for (int i = 0; i < SerialNames.Length; i++)
            {
                if (SerialNames[i].Length >= 12 && (SerialNames[i].Substring(0, 8).Contains("(") == false))
                    NewSerialNames[i] = SerialNames[i].Substring(0, 8) + ".(" + Regex.Replace(SerialNames[i], @"(.*\()(.*)(\).*)", "$2") + ")";
                else
                    NewSerialNames[i] = SerialNames[i];
            }

            string OldIItemStr = SerialCom.Text;
            int OldItemsCount = SerialCom.Items.Count;
            int Count = SerialNames.Length > OldItemsCount ? SerialNames.Length : OldItemsCount;

            for (int i = 0; i < Count; i++)
            {
                if (i < OldItemsCount)
                {
                    if (i < SerialNames.Length)
                        SerialCom.Items[i] = NewSerialNames[i];
                    else
                        SerialCom.Items.RemoveAt(SerialCom.Items.Count - 1);
                }
                else
                    SerialCom.Items.Add(NewSerialNames[i]);
            }
            MyPara.Serial.IsRefresh = false;                                    // 刷新结束 标记

            if (Array.IndexOf(NewSerialNames, OldIItemStr) >= 0)
                SerialCom.Text = OldIItemStr;
            else
            {
                SerialClose();
                SerialCom.Text = SerialCom.Items[0].ToString();
            }
        }

        public enum HardwareEnum
        {
            Win32_SerialPort = 10,                                              // 串口
            Win32_PnPEntity = 49,                                               // 所有
        }

        public static string[] MulGetHardwareInfo(HardwareEnum hardType, string propKey)
        {
            List<string> strs = new List<string>();
            try
            {
                using (ManagementObjectSearcher searcher = new ManagementObjectSearcher("select * from " + hardType))
                {
                    var hardInfos = searcher.Get(); foreach (var hardInfo in hardInfos)
                    {
                        string pattern = @"COM\d{1,3}";
                        Regex rx = new Regex(pattern);

                        if (hardInfo.Properties[propKey].Value != null && rx.IsMatch(hardInfo.Properties[propKey].Value.ToString()))
                        {
                            strs.Add(hardInfo.Properties[propKey].Value.ToString());
                        }
                    }
                    searcher.Dispose();
                }
                return strs.ToArray();
            }
            catch { return null; }
            finally { strs = null; }
        }

        private void SerialID_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (MyPara.Serial.IsRefresh)
            {
                serialPort1.PortName = Regex.Replace(SerialCom.Text, @"(.*\()(.*)(\).*)", "$2");    // 提取文本中小括号内的内容
                return;
            }  
            else
            {
                if (SerialCom.Items.Count == 0)
                {
                    if (serialPort1.IsOpen)
                        SerialClose();

                    serialPort1.PortName = "COM1";
                    SerialCom.Text = "";
                    return;
                }

                if (Regex.Replace(SerialCom.Text, @"(.*\()(.*)(\).*)", "$2") != serialPort1.PortName)
                {
                    if (serialPort1.IsOpen)
                    {
                        SerialClose();
                        serialPort1.PortName = Regex.Replace(SerialCom.Text, @"(.*\()(.*)(\).*)", "$2");    // 提取文本中小括号内的内容
                        SerialOpen();
                    }
                    else
                        serialPort1.PortName = Regex.Replace(SerialCom.Text, @"(.*\()(.*)(\).*)", "$2");
                }
            }
        }

        public void SerialOpen()
        {
            try
            {
                if (Baud.SelectedIndex == -1)
                {
                    Baud.Text = "9600";

                    MyPara.Serial.Baud = Convert.ToInt32(Baud.Text);
                    serialPort1.BaudRate = MyPara.Serial.Baud;
                }
                serialPort1.Open();
                if (serialPort1.IsOpen)
                {
                    MyPara.Serial.IsOpen = true;
                    btn_SerialOpen.BackColor = Color.FromArgb(71, 191, 43);
                    btn_SerialOpen.Text = Language.Text == "CN" ? "Colse" : "关闭";
                }
                else
                {
                    MyPara.Serial.IsOpen = false;
                    btn_SerialOpen.BackColor = Color.Red;
                }
            }
            catch (Exception)
            {
                MyPara.Serial.IsOpen = false;
                btn_SerialOpen.BackColor = Color.Red;
            }
        }

        public void SerialClose()
        {
            if (serialPort1.IsOpen)
                serialPort1.Close();

            btn_SerialOpen.Text = Language.Text == "CN" ? "Open" : "打开";
            MyPara.Serial.IsOpen = false;

            if (btn_SerialOpen.BackColor != Color.White)
                btn_SerialOpen.BackColor = Color.White;
        }

        private void Baud_SelectedIndexChanged(object sender, EventArgs e)
        {
            int baud = Convert.ToInt32(Baud.Text);
            MyPara.Serial.Baud = baud;
            serialPort1.BaudRate = baud;
            MyPara.Modbus.ReadData.TimeOut = (uint)(11000000 / baud + 1);
        }

        private void Parity_SelectedIndexChanged(object sender, EventArgs e)
        {
            switch (Parity.Text)
            {
                case "NONE":
                    serialPort1.Parity = System.IO.Ports.Parity.None;
                    break;
                case "ODD":
                    serialPort1.Parity = System.IO.Ports.Parity.Odd;
                    break;
                case "EVEN":
                    serialPort1.Parity = System.IO.Ports.Parity.Even;
                    break;
                default:
                    serialPort1.Parity = System.IO.Ports.Parity.None;
                    break;
            }
            MyPara.Serial.Parity = Parity.Text;
        }

        private void serialPort1_DataReceived(object sender, System.IO.Ports.SerialDataReceivedEventArgs e)
        {
            if (MyPara.SerialMatchFlag)
            {
                try
                {
                    if (MyPara.SerialMode == 0)                                     // RTU模式
                    {
                        if (serialPort1.BytesToRead == 7)
                        {
                            byte[] data = new byte[7];
                            MyPara.SerialMatchSuccessful = true;
                            serialPort1.Read(data, 0 ,7);
                            MyPara.SenorAddr = 256 * data[3] + data[4];
                        }
                    }
                    else
                    {
                        if (serialPort1.BytesToRead == 15)
                        {
                            byte[] data = new byte[15];
                            MyPara.SerialMatchSuccessful = true;
                            serialPort1.Read(data, 0, 15);
                            byte[] RtuData = Modbus.Ascii2RtuData(data, 15);
                            MyPara.SenorAddr = 256 * RtuData[3] + RtuData[4];
                        }
                    }
                }
                catch { }

                return;
            }

            MyPara.Serial.IsDataReceived = true;
        }

        private void button_Help_Click(object sender, EventArgs e)
        {
            // 点击按钮，显示帮助文档
            if (File.Exists(MyPara.HelpPath) == false)              // 文件不存在就提示
                MessageBox.Show("帮助文档丢失!!!", "提示");
            else
                System.Diagnostics.Process.Start(MyPara.HelpPath);  // 文件存在就打开 
        }

        private void timer_RefreshSerialDelay_Tick(object sender, EventArgs e)
        {
            timer_RefreshSerialDelay.Enabled = false;
            RefreshSerial();
        }

        private void timer_ReadData_Tick(object sender, EventArgs e)
        {
            if (MyPara.Modbus.ReadData.FirstTime)
            {
                MyPara.Modbus.ReadData.FirstTime = false;
                timer_ReadData.Interval = 500;
                MyPara.Modbus.ReadData.IsCycleArrive = true;
                MyPara.Modbus.ReadData.ReadFinsh = false;
                CreatReadDataThread();
            }
            else
            {
                MyPara.Modbus.ReadData.IsCycleArrive = true;
                if (MyPara.Modbus.ReadData.ReadFinsh)
                {
                    MyPara.Modbus.ReadData.ReadFinsh = false;
                    CreatReadDataThread();
                }
            }
        }

        public void CreatReadDataThread()
        {
            Thread thread = new Thread(new ThreadStart(ReadDataThreadMethod));  // 创建线程       
            thread.IsBackground = true;                                         // 后台线程 不设置关闭程序后线程依然会运行
            thread.Start();                                                     // 启动线程
        }

        public void ReadDataThreadMethod()
        {
            while (MyPara.Modbus.ReadData.IsCycleArrive)
            {
                MyPara.Modbus.ReadData.IsCycleArrive = false;

                if (IsSerialCanUse() == false)
                {
                    CloseReadData();
                    return;
                }

                MyPara.RunState.Model = MyPara.RunState.Model | MyPara.RunState.IsSerialUseMask;    // 标记Serial在使用中

                SerialSendData(MyPara.Modbus.ReadData.Cmd, (uint)MyPara.Modbus.ReadData.Cmd.Length);

                MyPara.RunState.Model = MyPara.RunState.Model & (~MyPara.RunState.IsSerialUseMask); // 标记Serial使用结束

                Thread.Sleep(10);                                                                   // 此处延时防止周期超时时发送数据依然能正常发送 延时要大于5ms

                if (MyPara.Modbus.ReadData.IsCycleArrive == false)
                    MyPara.Modbus.ReadData.ReadFinsh = true;
            }
        }

        public bool IsSerialCanUse()
        {
            if (MyPara.Serial.IsOpen == false)
                return false;

            int Count = 0;
            while ((MyPara.RunState.Model & MyPara.RunState.IsSerialUseMask) == MyPara.RunState.IsSerialUseMask)
            {
                Thread.Sleep(5);
                if (++Count == 100)                                             // 在连续0.5内若串口一直被占用则停止读数
                {
                    this.Invoke((MethodInvoker)delegate
                    {
                        MessageBox.Show("串口被占用,数据访问超时", "提示");
                    });
                    return false;
                }
            }

            return true;
        }

        public void SerialSendData(byte[] SendData, uint DataLen)
        {
            int TimeOut = (int)(100 + MyPara.Modbus.ReadData.TimeOut * (DataLen + 8) / 1000);   // 计算响应超时时间(ms) MyPara.Modbus.ReadData.OverTimeCount * 16 / 1000为数据传输时间 100为下位机响应时间
            int TimeOutCount = (TimeOut + 1) / 2;

            MyPara.Modbus.ReadData.OverTimeCount = 0;                           // 超时次数清零
            while (true)
            {
                MyPara.Serial.IsDataReceived = false;

                if (serialPort1.IsOpen == false)
                {
                    MyPara.RunState.Model = MyPara.RunState.Model & (~MyPara.RunState.IsSerialUseMask); // 标记Serial使用结束
                    CloseReadData();
                    return;
                }

                if (serialPort1.BytesToRead > 0)                                // 发送指令前检测接收缓存中是否有数据 有则清除
                    serialPort1.ReadExisting();

                serialPort1.Write(SendData, 0, (int)DataLen);                   // 串口发送数据
                MyPara.Modbus.ReadData.OverTimeCount++;

                int i;
                for (i = 0; i < TimeOutCount; i++)
                {
                    if (MyPara.Serial.IsDataReceived == false)                  // 等待接收第一个字节数据
                        Thread.Sleep(2);
                    else
                        break;
                }

                if (i != TimeOutCount)
                    break;
                else
                {
                    MyPara.RunState.Model = MyPara.RunState.Model & (~MyPara.RunState.IsSerialUseMask); // 标记Serial使用结束
                    Channal1Pos.Text = "----";
                    Channal2Pos.Text = "----";
                    return;
                }
            }

            int RecLen = serialPort1.BytesToRead;
            int Response = 17;

            if (RecLen != Response)
            {
                for (int i = 0; i < 10; i++)
                {
                    if (serialPort1.IsOpen == false)
                    {
                        MyPara.RunState.Model = MyPara.RunState.Model & (~MyPara.RunState.IsSerialUseMask); // 标记Serial使用结束
                        CloseReadData();
                        return;
                    }

                    int NewRecLen = serialPort1.BytesToRead;
                    if (NewRecLen != Response)
                    {
                        if (NewRecLen != RecLen)
                        {
                            i = -1;
                            RecLen = NewRecLen;
                        }
                        Thread.Sleep(2);
                    }
                    else
                        break;
                }
            }

            if (serialPort1.IsOpen == false)
            {
                MyPara.RunState.Model = MyPara.RunState.Model & (~MyPara.RunState.IsSerialUseMask); // 标记Serial使用结束
                CloseReadData();
                return;
            }

            byte[] RecData = new byte[serialPort1.BytesToRead];
            serialPort1.Read(RecData, 0, RecData.Length);
            RecLen = RecData.Length;


            if ((RecData[0] != MyPara.Modbus.ReadData.SenorID) ||               // 地址错误
                (RecData[1] != 0x04) ||                                         // 功能码错误
                (RecData[2] != 12) ||                                           // 字节数错误
                (RecLen != Response))                                           // 数据长度错误
            {
                Channal1Pos.Text = "----";
                Channal2Pos.Text = "----";
            }
            else
            {
                if (Modbus.GetCRC(RecData, (uint)RecData.Length) != 0)          // CRC校验错误
                {
                    Channal1Pos.Text = "----";
                    Channal2Pos.Text = "----";
                }
                else
                {
                    double CH1Data = 0;
                    double CH2Data = 0;

                    CH1Data = 256 * RecData[3] + RecData[4] + (RecData[5] + RecData[6]) / 65536.0;
                    CH2Data = 256 * RecData[11] + RecData[12] + (RecData[13] + RecData[14]) / 65536.0;

                    Channal1Pos.Text = CH1Data.ToString("#0.000");
                    Channal2Pos.Text = CH2Data.ToString("#0.000");
                }
            }
        }

        private void Baud_TextChanged(object sender, EventArgs e)
        {
            if (Baud.Text.Length > 0)
            {
                try
                {
                    MyPara.Serial.Baud = Convert.ToInt32(Baud.Text);
                    serialPort1.BaudRate = MyPara.Serial.Baud;
                }
                catch { }
            }
        }

        private void Form1_FormClosed(object sender, FormClosedEventArgs e)
        {
            SaveSettings();                                         // 保存配置
        }

        public void SaveSettings()
        {
            try
            {
                Settings.Default.SensorID = (int)SensorID.Value;

                Settings.Default.SerialCom = SerialCom.Text;
                Settings.Default.Baud = Baud.Text;
                Settings.Default.Parity = Parity.Text;

                Settings.Default.WriteID = (int)WriteSensorID.Value;
                Settings.Default.WriteBaud = WriteBaud.Text;
                Settings.Default.WriteParity = WriteParity.Text;
                Settings.Default.WriteCircularCount = WriteCircularCount.Text;
                Settings.Default.WriteDirection = WriteDirection.Text;
            }
            catch { }

            Settings.Default.Save();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            AutoMatchStart();

            byte[] RtuCommand  = new byte[] { 0x00, 0x03, 0x00, 0x30, 0x00, 0x01, 0x85, 0xD4 };
            byte[] AsciiCommand = new byte[] { 0x3A, 0x30, 0x30, 0x30, 0x33, 0x30, 0x30, 0x33, 0x30, 0x30, 0x30,
                                              0x30, 0x31, 0x43, 0x43, 0x0D, 0x0A };

            if (!serialPort1.IsOpen)
                SerialOpen();

            if (!serialPort1.IsOpen)
            {
                if (Language.Text == "EN")
                    MessageBox.Show("串口打开失败，匹配结束！");
                else
                    MessageBox.Show("Serial port opening failed, end of match!");

                AutoMatchEnd();
                return;
            }

            MyPara.SerialMatchFlag = true;                                      // 串口匹配标志
            MyPara.SerialMatchSuccessful = false;                               // 串口匹配成功标志

            for (int SerialIndex = 0; SerialIndex < SerialCom.Items.Count; SerialIndex++)
            {
                SerialCom.SelectedIndex = SerialIndex;
                for (int BaudIndex = 6; BaudIndex >= 0; BaudIndex--)
                {
                    Baud.SelectedIndex = BaudIndex;

                    int OverTime;                                               // 超时时间
                    if (MyPara.Serial.Baud > 9600)
                        OverTime = 150;
                    else
                        OverTime = 250;

                    for (int ParityIndex = 0; ParityIndex < 3; ParityIndex++)
                    {
                        Parity.SelectedIndex = ParityIndex;

                        serialPort1.ReceivedBytesThreshold = 7;

                        if (!serialPort1.IsOpen)
                        {
                            if (Language.Text == "EN")
                                MessageBox.Show("串口异常，匹配结束！");
                            else
                                MessageBox.Show("Serial port exception, end of match!");

                            AutoMatchEnd();
                            return;
                        }
                        if (serialPort1.BytesToRead > 0)            // 发送指令前检测接收缓存中是否有数据 有则清除
                            serialPort1.ReadExisting();

                        MyPara.SerialMode = 0;
                        serialPort1.Write(RtuCommand, 0, 8);        // 发送RTU广播读地址指令

                        DateTime StartTime1 = DateTime.Now;
                        while (true)
                        {
                            DelayMs(5);

                            if (MyPara.SerialMatchSuccessful)                                            
                                break;

                            if (StartTime1.AddMilliseconds(OverTime) < DateTime.Now)
                                break;
                        }

                        if (MyPara.SerialMatchSuccessful)
                            break;
                    }
                    if (MyPara.SerialMatchSuccessful)
                        break;
                }
                if (MyPara.SerialMatchSuccessful)
                    break;
            }

            serialPort1.ReceivedBytesThreshold = 5;

            if (MyPara.SerialMatchSuccessful)
            {
                SensorID.Value = MyPara.SenorAddr;

                if (Language.Text == "EN")
                    MessageBox.Show("通信匹配成功！通信参数如下：" +
                                                            "\r\n串口号：" + SerialCom.Text +
                                                            "\r\n设备ID：" + MyPara.SenorAddr.ToString() +
                                                            "\r\n波特率：" + Baud.Text +
                                                            "\r\n校验位：" + Parity.Text);
                else
                    MessageBox.Show("Match successful! Parameters are as follows:" +
                                                            "\rCOM：\t" + SerialCom.Text +
                                                            "\r\nAddress：\t" + MyPara.SenorAddr.ToString() +
                                                            "\r\nBaud：\t" + Baud.Text +
                                                            "\r\nParity：\t" + Parity.Text);

            }
            else
            {
                SerialClose();
                
                if (Language.Text == "EN")
                    MessageBox.Show("通信匹配失败！");
                else
                    MessageBox.Show("Communication match failed!");
            }  

            MyPara.SerialMatchFlag = false;                                     // 串口匹配标志
            AutoMatchEnd();
        }

        public void AutoMatchStart()
        {
            if (ReadData.Text == "停止" || ReadData.Text == "Stop")
                CloseReadData();

            gb_SerialSet.Enabled = false;
        }

        public void AutoMatchEnd()
        {
            gb_SerialSet.Enabled = true;
        }

        public static void DelayMs(int Ms)
        {
            DateTime EndTime = DateTime.Now.AddMilliseconds(Ms);

            while (DateTime.Now < EndTime)
                Application.DoEvents();
        }

        private void SensorID_ValueChanged(object sender, EventArgs e)
        {
            MyPara.Modbus.ReadData.SenorID = (uint)SensorID.Value;
            MyPara.Modbus.Send.SensorID = (uint)SensorID.Value;

            GetReadDataCmd();
        }

        public void GetCmd()
        {
            MyPara.Modbus.Send.Cmd = new byte[30];
            MyPara.Modbus.Send.CmdLen = 0;

            MyPara.Modbus.Send.Cmd[0] = (byte)MyPara.Modbus.Send.SensorID;      // 设备地址

            switch(MyPara.Modbus.Send.WriteCmd)
            {
                case Para.modbus.WriteCmd.WriteID:
                    WriteF10();
                    break;
                case Para.modbus.WriteCmd.WriteBaud:
                    WriteF10();
                    break;
                case Para.modbus.WriteCmd.WriteParity:
                    WriteF10();
                    break;
                case Para.modbus.WriteCmd.WriteDirection:
                    WriteF10();
                    break;
                case Para.modbus.WriteCmd.WriteCircularCount:
                    WriteF27();
                    break;
            }

            uint CRC = Modbus.GetCRC(MyPara.Modbus.Send.Cmd, MyPara.Modbus.Send.CmdLen);
            MyPara.Modbus.Send.Cmd[MyPara.Modbus.Send.CmdLen++] = (byte)CRC;
            MyPara.Modbus.Send.Cmd[MyPara.Modbus.Send.CmdLen++] = (byte)(CRC / 256);
        }

        public void WriteF10()
        {
            MyPara.Modbus.Send.Cmd[1] = 0x10;                                                       // 功能码
            MyPara.Modbus.Send.CorrectRecLen = 8;                                                   // 正确回复长度

            switch (MyPara.Modbus.Send.WriteCmd)
            {
                case Para.modbus.WriteCmd.WriteID:
                    MyPara.Modbus.Send.StartAddr = 0x30;
                    MyPara.Modbus.Send.Cmd[7] = (byte)(MyPara.Modbus.Send.DataID / 256);
                    MyPara.Modbus.Send.Cmd[8] = (byte)MyPara.Modbus.Send.DataID;
                    break;
                case Para.modbus.WriteCmd.WriteBaud:
                    MyPara.Modbus.Send.Cmd[7] = (byte)(MyPara.Modbus.Send.DataBaud / 256);
                    MyPara.Modbus.Send.Cmd[8] = (byte)MyPara.Modbus.Send.DataBaud;
                    MyPara.Modbus.Send.StartAddr = 0x31;
                    break;
                case Para.modbus.WriteCmd.WriteParity:
                    MyPara.Modbus.Send.Cmd[7] = (byte)(MyPara.Modbus.Send.DataParity / 256);
                    MyPara.Modbus.Send.Cmd[8] = (byte)MyPara.Modbus.Send.DataParity;
                    MyPara.Modbus.Send.StartAddr = 0x32;
                    break;
                case Para.modbus.WriteCmd.WriteDirection:
                    MyPara.Modbus.Send.Cmd[7] = (byte)(MyPara.Modbus.Send.DataDirection / 256);
                    MyPara.Modbus.Send.Cmd[8] = (byte)MyPara.Modbus.Send.DataDirection;
                    MyPara.Modbus.Send.StartAddr = 0x3E;
                    break;
                default:
                    break;
            }

            MyPara.Modbus.Send.Cmd[2] = (byte)(MyPara.Modbus.Send.StartAddr / 256);                 // 写寄存器起始地址
            MyPara.Modbus.Send.Cmd[3] = (byte)(MyPara.Modbus.Send.StartAddr % 256);

            MyPara.Modbus.Send.Cmd[4] = 0x00;                                   // 写寄存器数量
            MyPara.Modbus.Send.Cmd[5] = 0x01;
            MyPara.Modbus.Send.Cmd[6] = 0x02;                                   // 写入字节数
            MyPara.Modbus.Send.CmdLen = 9;                                      // 命令长度 不包含CRC
        }

        public void WriteF27()
        {
            MyPara.Modbus.Send.Cmd[1] = 0x27;                                   // 功能码
            MyPara.Modbus.Send.CorrectRecLen = 8;                               // 正确回复长度

            MyPara.Modbus.Send.Cmd[2] = 0x00;                                   // 写寄存器起始地址
            MyPara.Modbus.Send.Cmd[3] = 0xB2;
            MyPara.Modbus.Send.Cmd[4] = 0x00;                                   // 写寄存器数量
            MyPara.Modbus.Send.Cmd[5] = 0x02;
            MyPara.Modbus.Send.Cmd[6] = 0x04;                                   // 写入字节数
            MyPara.Modbus.Send.Cmd[7] = 0x00;
            MyPara.Modbus.Send.Cmd[8] = 0x00;
            MyPara.Modbus.Send.Cmd[9] = (byte)(MyPara.Modbus.Send.DataCircularCount / 256);
            MyPara.Modbus.Send.Cmd[10] = (byte)MyPara.Modbus.Send.DataCircularCount;

            
            MyPara.Modbus.Send.CmdLen = 11;                                     // 命令长度 不包含CRC
        }

        private void bt_WriteSensorID_Click(object sender, EventArgs e)
        {
            WriteCmd(Para.modbus.WriteCmd.WriteID);
        }

        private void bt_WriteBaud_Click(object sender, EventArgs e)
        {
            WriteCmd(Para.modbus.WriteCmd.WriteBaud);
        }

        private void bt_WriteParity_Click(object sender, EventArgs e)
        {
            WriteCmd(Para.modbus.WriteCmd.WriteParity);
        }

        private void bt_WriteCircularCount_Click(object sender, EventArgs e)
        {
            WriteCmd(Para.modbus.WriteCmd.WriteCircularCount);
        }

        private void bt_WriteDirection_Click_1(object sender, EventArgs e)
        {
            WriteCmd(Para.modbus.WriteCmd.WriteDirection);
        }

        public void WriteCmd(Para.modbus.WriteCmd WriteCmd)
        {
            if (btn_SerialOpen.Text == "打开" || btn_SerialOpen.Text == "Open")
                SerialOpen();

            MyPara.Modbus.Send.WriteCmd = WriteCmd;
            GetCmd();

            try
            {
                MyPara.Modbus.Send.SendSema.Release();
            }
            catch { }

            MyPara.Modbus.Send.ReceSema.WaitOne();

            Color color = new Color();
            if (MyPara.Modbus.Send.Flag)
            {
                color = Color.FromArgb(71, 191, 43);

                switch (MyPara.Modbus.Send.WriteCmd)
                {
                    case Para.modbus.WriteCmd.WriteID:
                        SensorID.Value = WriteSensorID.Value;
                        break;
                    case Para.modbus.WriteCmd.WriteBaud:
                        Baud.Text = WriteBaud.Text;
                        break;
                    case Para.modbus.WriteCmd.WriteParity:
                        Parity.Text = WriteParity.Text;
                        break;
                }
            }
            else
                color = Color.Red;

            switch (MyPara.Modbus.Send.WriteCmd)
            {
                case Para.modbus.WriteCmd.WriteID:
                    bt_WriteSensorID.BackColor = color;
                    DelayMs(200);
                    bt_WriteSensorID.BackColor = Color.White;
                    break;
                case Para.modbus.WriteCmd.WriteBaud:
                    bt_WriteBaud.BackColor = color;
                    DelayMs(200);
                    bt_WriteBaud.BackColor = Color.White;
                    break;
                case Para.modbus.WriteCmd.WriteParity:
                    bt_WriteParity.BackColor = color;
                    DelayMs(200);
                    bt_WriteParity.BackColor = Color.White;
                    break;
                case Para.modbus.WriteCmd.WriteDirection:
                    bt_WriteDirection.BackColor = color;
                    DelayMs(200);
                    bt_WriteDirection.BackColor = Color.White;
                    break;
                case Para.modbus.WriteCmd.WriteCircularCount:
                    bt_WriteCircularCount.BackColor = color;
                    DelayMs(200);
                    bt_WriteCircularCount.BackColor = Color.White;
                    break;
                default:
                    break;
            }
        }

        public void SendReceMethod()
        {
            while (true)
            {
                MyPara.Modbus.Send.SendSema.WaitOne();

                MyPara.Modbus.Send.Flag = false;

                if (MyPara.Modbus.Send.CmdLen == 0)
                    continue;

                bool SerialState = IsSerialCanUse();

                if (SerialState == false)
                    continue;

                MyPara.RunState.Model |= MyPara.RunState.IsSerialUseMask;                           // 标记串口在使用中

                SendReceData(MyPara.Modbus.Send.Cmd, MyPara.Modbus.Send.CmdLen);                    // 发送

                try
                {
                    MyPara.Modbus.Send.ReceSema.Release();
                }
                catch { }
            }
        }

        public void SendReceData(byte[] SendData, uint DataLen)
        {
            int TimeOut = (int)(100 + MyPara.Modbus.ReadData.TimeOut * (DataLen + 8) / 1000);       // 计算响应超时时间(ms) MyPara.Modbus.ReadData.TimeOut * (DataLen + 8) / 1000为数据传输时间 100为下位机响应时间
            int TimeOutCount = (TimeOut + 1) / 2;

            if (serialPort1.IsOpen == false)
            {
                MyPara.RunState.Model = MyPara.RunState.Model & (~MyPara.RunState.IsSerialUseMask); // 标记Serial使用结束
                return;
            }
            if (serialPort1.BytesToRead > 0)                                    // 发送指令前检测接收缓存中是否有数据 有则清除
                serialPort1.ReadExisting();

            MyPara.Serial.IsDataReceived = false;
            serialPort1.Write(SendData, 0, (int)DataLen);                       // 串口发送数据

            for (int i = 0; i < TimeOutCount; i++)
            {
                if (MyPara.Serial.IsDataReceived == false)                      // 等待接收第一个字节数据
                    Thread.Sleep(2);
                else
                    break;
            }

            if (serialPort1.IsOpen == false)
            {
                MyPara.RunState.Model = MyPara.RunState.Model & (~MyPara.RunState.IsSerialUseMask);         // 标记Serial使用结束
                return;
            }

            int RecLen = serialPort1.BytesToRead;
            if (RecLen != MyPara.Modbus.Send.CorrectRecLen)
            {
                for (int i = 0; i < 10; i++)
                {
                    if (serialPort1.IsOpen == false)
                    {
                        MyPara.RunState.Model = MyPara.RunState.Model & (~MyPara.RunState.IsSerialUseMask); // 标记Serial使用结束
                        return;
                    }
                    int NewRecLen = serialPort1.BytesToRead;
                    if (NewRecLen != MyPara.Modbus.Send.CorrectRecLen)
                    {
                        if (NewRecLen != RecLen)
                        {
                            i = -1;
                            RecLen = NewRecLen;
                        }
                        Thread.Sleep(2);
                    }
                    else
                        break;
                }
            }

            if (MyPara.Serial.IsDataReceived == false)
            {
                MyPara.RunState.Model &= (~MyPara.RunState.IsSerialUseMask);    // 标记串口使用结束
                return;
            }

            if (serialPort1.IsOpen == false)
            {
                MyPara.RunState.Model = MyPara.RunState.Model & (~MyPara.RunState.IsSerialUseMask); // 标记Serial使用结束
                return;
            }

            byte[] RecData = new byte[serialPort1.BytesToRead];

            if (RecData.Length == MyPara.Modbus.Send.CorrectRecLen)
                MyPara.Modbus.Send.Flag = true;                                 // 标志指令读写成功

            serialPort1.Read(RecData, 0, RecData.Length);
            MyPara.Modbus.Rece.CmdLen = (uint)RecData.Length;

            MyPara.RunState.Model &= (~MyPara.RunState.IsSerialUseMask);        // 标记串口使用结束

            MyPara.Modbus.Rece.Cmd = RecData;
            MyPara.Modbus.Rece.CmdLen = (uint)RecData.Length;

            MyPara.Modbus.Rece.FunCode = MyPara.Modbus.Rece.Cmd[1];
            MyPara.Modbus.Rece.ByteNum = MyPara.Modbus.Rece.Cmd[2];

            if (MyPara.Modbus.Rece.FunCode >= 0x80)
            {
                MyPara.Modbus.Rece.FunCode = MyPara.Modbus.Rece.Cmd[1];
                MyPara.Modbus.Rece.ErrorNum = MyPara.Modbus.Rece.Cmd[2];
            }

            if (MyPara.Modbus.Rece.FunCode == 0x03 || MyPara.Modbus.Rece.FunCode == 0x26)
            {
                for (int i = 0; i < MyPara.Modbus.Rece.ByteNum / 2; i++)
                    MyPara.Modbus.Rece.Data[i] = (uint)((MyPara.Modbus.Rece.Cmd[3 + 2 * i] << 8) + MyPara.Modbus.Rece.Cmd[4 + 2 * i]);
            }
        }

        private void WriteSensorID_ValueChanged(object sender, EventArgs e)
        {
            MyPara.Modbus.Send.DataID = (int)WriteSensorID.Value;
        }

        private void WriteBaud_SelectedIndexChanged(object sender, EventArgs e)
        {
            MyPara.Modbus.Send.DataBaud = WriteBaud.SelectedIndex + 1;
        }

        private void WriteParity_SelectedIndexChanged(object sender, EventArgs e)
        {
            switch(WriteParity.Text)
            {
                case "ODD":
                    MyPara.Modbus.Send.DataParity = 1;
                    break;
                case "EVEN":
                    MyPara.Modbus.Send.DataParity = 2;
                    break;
                case "NONE":
                    MyPara.Modbus.Send.DataParity = 3;
                    break;
                default:
                    MyPara.Modbus.Send.DataParity = 0;
                    break;
            }
        }

        private void WriteCircularCount_SelectedIndexChanged(object sender, EventArgs e)
        {
            MyPara.Modbus.Send.DataCircularCount = WriteCircularCount.SelectedIndex + 1;
        }

        private void WriteDirection_SelectedIndexChanged(object sender, EventArgs e)
        {
            MyPara.Modbus.Send.DataDirection = WriteDirection.SelectedIndex;
        }

        private void textBox1_MouseDown(object sender, MouseEventArgs e)
        {
            HideCaret(((TextBox)Channal2Pos).Handle);                              // 示数显示隐藏光标 就不会闪烁造成视觉效果不好
        }

        private void textBox2_MouseDown(object sender, MouseEventArgs e)
        {
            HideCaret(((TextBox)Channal1Pos).Handle);                              // 示数显示隐藏光标 就不会闪烁造成视觉效果不好
        }

        private void button1_Click_1(object sender, EventArgs e)
        {
            if (Language.Text == "CN")
                CN_Settings();
            else
                EN_Settings();
        }

        public void CN_Settings()
        {
            Language.Text = "EN";
            MyPara.Title = "磁尺";
            this.Text = MyPara.Title + DateTime.Now.ToString("  HH:mm:ss");

            gb_SerialSet.Text = "通信设置";
            Serial_Address.Text = "设备ID";
            Serial_Com.Text = "端口号";
            Serial_Baud.Text = "波特率";
            Serial_Parity.Text = "校验位";
            AutoMatch.Text = "一键匹配";
            btn_SerialOpen.Text = btn_SerialOpen.Text == "Open" ? "打开" : "关闭";

            gb_ReadData.Text = "读数据";
            ReadData.Text = ReadData.Text == "Start" ? "开始" : "停止";

            gb_ParametersModify.Text = "参数设置";
            Write_SensorID.Text = "设备ID";
            Write_Baud.Text = "波特率";
            Write_Parity.Text = "校验位";
            Write_MagnetNum.Text = "磁环数";
            Write_Diretion.Text = "正反向";

            bt_WriteSensorID.Text = "写入";
            bt_WriteBaud.Text = "写入";
            bt_WriteParity.Text = "写入";
            bt_WriteCircularCount.Text = "写入";
            bt_WriteDirection.Text = "写入";
        }

        public void EN_Settings()
        {
            Language.Text = "CN";
            MyPara.Title = "Magnetic Ruler";
            this.Text = MyPara.Title + DateTime.Now.ToString("  HH:mm:ss");

            gb_SerialSet.Text = "Communication Settings";
            Serial_Address.Text = "Address";
            Serial_Com.Text = " COM";
            Serial_Baud.Text = " Baud";
            Serial_Parity.Text = "Parity";
            AutoMatch.Text = "Match";
            btn_SerialOpen.Text = btn_SerialOpen.Text == "打开" ? "Open" : "Colse";

            gb_ReadData.Text = "Read Data";
            ReadData.Text = ReadData.Text == "开始" ? "Start" : "Stop";

            gb_ParametersModify.Text = "Parameters Settings";
            Write_SensorID.Text = "Address";
            Write_Baud.Text = "Baud";
            Write_Parity.Text = "Parity";
            Write_MagnetNum.Text = "MagnetNum";
            Write_Diretion.Text = "Direction";

            bt_WriteSensorID.Text = "Modify";
            bt_WriteBaud.Text = "Modify";
            bt_WriteParity.Text = "Modify";
            bt_WriteCircularCount.Text = "Modify";
            bt_WriteDirection.Text = "Modify";
        }
    }
}

