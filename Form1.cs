using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
//添加这个类就可以使用SerialPort方法 获取可用串口
using System.IO.Ports;
using System.Net;
using System.Net.Sockets;
using System.Threading;
using System.IO;  

namespace OV7725调车上位机
{
  
    public partial class Form1 : Form
    {       
        #region  通讯协议
        /**********发送数据类型****************/
        const byte Balance_P = 0x20;
        const byte Balance_I = 0x21;
        const byte Balance_D = 0x22;
        const byte Position_P = 0x23;
        const byte Position_I = 0x24;
        const byte Position_D = 0x25;
        const byte Speed_P = 0x26;
        const byte Speed_I = 0x27;
        const byte Speed_D = 0x28;
        const byte Steer_P = 0x29;
        const byte Steer_I = 0x2A;
        const byte Steer_D = 0x2B;
        const byte Forward = 0x2C;
        const byte Backward = 0x2D;
        const byte TurnLeft = 0x2E;
        const byte TurnRight = 0x2F;
        const byte Firing = 0x30;
        const byte Misfire = 0x31;
        const byte EncoderForward = 0x32;
        const byte EncoderBackward = 0x33;
        const byte EncoderTurnLeft = 0x34;
        const byte EncoderTurnRight = 0x35;
        const byte EncoderFiring = 0x36;
        const byte EncoderMisfire = 0x37;
        const byte YAW = 0x38;
        const byte SetSpeed = 0x39;
        bool GoFlag = false;
        /**********接收数据类型****************/
        const byte Rec_CPUStatus = 0x32;
        const byte Rec_LeftRoom = 0x33;
        const byte Rec_MiddleLine = 0x34;
        const byte Rec_LSpeed = 0x35;
        const byte Rec_RSpeed = 0x36;
        const byte Rec_Pitch = 0x37;
        const byte Rec_Yaw = 0x38;
        const byte Rec_Roll = 0x39;
        const byte Rec_Number1 = 0x3A;
        const byte Rec_Number2 = 0x3B;
        const byte Rec_Number3 = 0x3C;
        const byte Rec_Number4 = 0x3D;
        const byte Rec_Number5 = 0x3E;
        const byte Rec_Number6 = 0x3F;
        const byte Rec_CCD1 = 0x40;
        const byte Rec_CCD2 = 0x41;
        const byte Rec_CMOS = 0x42;
        #endregion
       public Form1()
        {
            InitializeComponent();
            //线程设置为其他线程可以调用该线程的资源
            Control.CheckForIllegalCrossThreadCalls = false;
        }
       private NetworkStream networkStream;
       private BinaryReader streamReader;
       private BinaryWriter streamWriter;
       private Socket socketForClient;
       private TcpListener tcpListener;
       private Thread serverThread;   // 启动服务线程  
       private Thread recvThread;// 接收信息线程

        private void Form1_Load(object sender, EventArgs e)
        {
            radioButton1.Checked = true;
            textBox2.Text = "8080";
            textBox1.Text = "192.168.191.1";
            button2.Enabled = false;
            button12.Enabled = false;
            #region  波特率初始化
            string[] BaudRate = { "9600", "115200", "315200" };
            comboBox2.DropDownStyle = ComboBoxStyle.DropDownList;
            for (int i = 0; i < BaudRate.Length; i++)
            {
                comboBox2.Items.Add(BaudRate[i]);
            }
            comboBox2.Text = "115200";
            serialPort1.BaudRate = 115200;
            #endregion
            #region  串口初始化
            //让下拉框只能选择已有选项，不能输入
            comboBox1.DropDownStyle = ComboBoxStyle.DropDownList;
            //自动搜索并且添加可用串口
            string[] PortName = SerialPort.GetPortNames();//读取
            for (int i = 0; i < PortName.Length; i++)//添加
            {
                comboBox1.Items.Add(PortName[i]);
            }

            try
            {
                comboBox1.SelectedIndex = 0;
            }
            catch
            {
                MessageBox.Show("没有可用端口");
            }
            //串口接受消息响应设置
            serialPort1.DataReceived += serialPort1_DataReceived;
            #endregion
        }
        void serialPort1_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            byte ReceiveData;
            //读取数据
            ReceiveData = (byte)serialPort1.ReadByte();
            RecvHandle(ReceiveData);
        }
        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            // 在未停止 服务 前，不允许关闭窗体  
            if (button1.Enabled == false)
            {
                MessageBox.Show("请先停止服务!", "Server提示");
                e.Cancel = true;
            }
        }
        

        private void Listen()
        {
            try
            {
                Int32 port = Int32.Parse(textBox2.Text);
                IPAddress ipAddress = IPAddress.Parse(textBox1.Text);
                tcpListener = new TcpListener(ipAddress, port);
                // 开始侦听  
                tcpListener.Start();
                listBox1.Items.Add(DateTime.Now.ToString()+"  服务已经启动！");
                button1.Enabled = false;
                button2.Enabled = true;

                // 返回可以用以处理连接的Socket实例  
                socketForClient = tcpListener.AcceptSocket();
                // 如果返回值是"true"，则产生的套节字已经接受来自远方的连接请求  
                if (socketForClient.Connected)
                {
                    button12.Enabled = true;
                    listBox1.Items.Add(DateTime.Now.ToString()+"  客户端成功连接上服务器!");
                    // 创建networkStream对象通过网络套节字来接受和发送数据  
                    networkStream = new NetworkStream(socketForClient);
                    // 从当前数据流中读取一行字符，返回值是字符串  
                    //streamReader = new StreamReader(networkStream, System.Text.Encoding.UTF8);
                    //streamReader = new StreamReader(networkStream, System.Text.Encoding.UTF8);
                    streamReader = new BinaryReader(networkStream);
                    streamWriter = new BinaryWriter(networkStream);
                    recvThread = new Thread(new ThreadStart(RecvData));
                    recvThread.Start();
                }
            }
            catch (Exception exc)
            {
                MessageBox.Show(exc.Message, "Server提示");
            }

        }

        private void RecvHandle(byte ReceiveData)
        {
            #region  缓存区数据处理
            float usart_temp = 0;
            if ((global.USART_RX_STA & 0x8000) == 0) //接收未完成
            {
                if ((global.USART_RX_STA & 0x4000) != 0) //接收到了0x0d
                {
                    if (ReceiveData != 0x0a)
                    {
                        global.USART_RX_STA = 0; //接收错误,重新开始
                    }
                    else
                    {
                        global.USART_RX_STA |= 0x8000;	//接收完成了
                    }
                }
                else //还没收到0X0D
                {
                    if (ReceiveData == 0x0d)
                    {
                        global.USART_RX_STA |= 0x4000;
                    }
                    else
                    {
                        global.USART_RX_BUF[global.USART_RX_STA & 0X3FFF] = (byte)ReceiveData;
                        global.USART_RX_STA++;
                        if (global.USART_RX_STA > (global.USART_REC_LEN - 1))
                        {
                            global.USART_RX_STA = 0; //接收数据错误,重新开始接收
                        }
                    }
                }
            }
            #endregion
            #region  接收数据显示
            if ((global.USART_RX_STA & 0x8000) != 0)
            {
                int len;
                len = global.USART_RX_STA & 0x3fff; //得到此次接收到的数据长度
                //检验第一位  确保数据正确
                if (global.USART_RX_BUF[0] == 0xff)
                {
                    if (len == 5)
                    {
                        usart_temp = (float)(global.USART_RX_BUF[3]) + ((float)global.USART_RX_BUF[4]) / 100;
                        if (global.USART_RX_BUF[2] == 0x01)
                        {
                            usart_temp = -usart_temp;
                        }
                    }
                    else if (len == 3)
                    {
                        usart_temp = (float)global.USART_RX_BUF[2];
                    }
                    switch (global.USART_RX_BUF[1])
                    {
                        case Rec_CPUStatus:
                            label17.Text = usart_temp.ToString() + "%";
                            break;
                        case Rec_LeftRoom:
                            label18.Text = usart_temp.ToString() + "K";
                            break;
                        case Rec_MiddleLine:
                            label25.Text = usart_temp.ToString();
                            break;
                        case Rec_LSpeed:
                            label26.Text = usart_temp.ToString();
                            break;
                        case Rec_RSpeed:
                            label27.Text = usart_temp.ToString();
                            break;
                        case Rec_Pitch:
                            label28.Text = usart_temp.ToString();
                            break;
                        case Rec_Yaw:
                            label29.Text = usart_temp.ToString();
                            break;
                        case Rec_Roll:
                            label30.Text = usart_temp.ToString();
                            break;
                        case Rec_Number1:
                            label37.Text = usart_temp.ToString();
                            break;
                        case Rec_Number2:
                            label38.Text = usart_temp.ToString();
                            break;
                        case Rec_Number3:
                            label39.Text = usart_temp.ToString();
                            break;
                        case Rec_Number4:
                            label40.Text = usart_temp.ToString();
                            break;
                        case Rec_Number5:
                            label41.Text = usart_temp.ToString();
                            break;
                        case Rec_Number6:
                            label42.Text = usart_temp.ToString();
                            break;
                    }
                }
                //CCD
                else if (global.USART_RX_BUF[0] == 0x0f)
                {
                    switch (global.USART_RX_BUF[1])
                    {
                        case Rec_CCD1:
                            global.CCD1 = global.USART_RX_BUF;
                            break;
                        case Rec_CCD2:
                            global.CCD2 = global.USART_RX_BUF;
                            break;
                    }
                }

            #endregion
                global.USART_RX_STA = 0;
            }
        }
        // 接收服务器发来的信息  
        private void RecvData()
        {

            byte msg=0;
            try
            {
                msg = streamReader.ReadByte();
            }
            catch
            {
                MessageBox.Show("读不到字节");
            }
            // 如果不是接收到客户端发来的退出信息，则不断接收信息  
            while (true)
            {
                RecvHandle(msg);
                //listBox1.Items.Add(DateTime.Now.ToString() + "  收到信息:  " + msg);
                try
                {
                    msg = streamReader.ReadByte();
                }
                catch
                {
                    //MessageBox.Show("读不到字节");
                }
            }
            //// 如果接收到客户的退出信息，则退出服务  
            //button1.Enabled = true;
            //button2.Enabled = false;
            //button12.Enabled = false;
            //listBox1.Items.Add("客户已经关闭");
            //listBox1.Items.Add("服务器关闭");
            //// 关闭资源  
            //ReleaseResource();
        }

        #region 开启服务线程
        private void button1_Click(object sender, EventArgs e)
        {
            serverThread = new Thread(new ThreadStart(Listen));
            serverThread.Start(); 
        }

        #endregion
        private void button2_Click(object sender, EventArgs e)
        {
            try
            {
                if (button3.Enabled == true)
                {
                    // 停止服务后，允许 启动服务，但是不允许发送消息  
                    button1.Enabled = true;
                    button2.Enabled = false;
                    button12.Enabled = false;
                    string exitMsg = "exit";  // 要退出时，发送 exit 信息给客户端  
                    streamWriter.Write(exitMsg+"\r\n");
                    // 刷新当前数据流中的数据  
                    streamWriter.Flush();
                    listBox1.Items.Add(DateTime.Now.ToString()+"  服务器关闭");

                    // 释放资源  
                    recvThread.Abort();
                    ReleaseResource();
                }
                else
                {
                    // 停止服务后，允许 启动服务，但是不允许发送消息  
                    button1.Enabled = true;
                    button2.Enabled = false;
                    button12.Enabled = false;
                    listBox1.Items.Add(DateTime.Now.ToString()+"  服务器关闭");

                    // 关闭TCP连接，关闭线程  
                    tcpListener.Stop();
                    serverThread.Abort();
                }
            }
            catch (Exception exc)
            {
                MessageBox.Show(exc.Message, "Server提示");
            }  
        }
        // 释放资源  
        private void ReleaseResource()
        {
            networkStream.Close();
            streamReader.Close();
            streamWriter.Close();
            socketForClient.Shutdown(SocketShutdown.Both);
            socketForClient.Close();
            tcpListener.Stop();
        }

        #region  发送协议
        private void SendData(float data,byte type)
        {
            //发送数据
            byte[] Send_Data = new byte[7] { 0xff, type, 0x00,0x00, 0x00, 0x0d, 0x0a };
            if (data < 0)
            {
                Send_Data[2] = 0x01;
                data = -data;
            }
            //temp_1整数部分  temp_2两位小数
            byte temp_1,temp_2;
            temp_1 = (byte)data;
            Send_Data[3] = temp_1;
            temp_2 = (byte)(100*(data-(float)temp_1));
            Send_Data[4] = temp_2;

            string[] str_temp = new string[7];
            for (int i = 0; i < 7;i++ )
            {
                str_temp[i]=Convert.ToString(Send_Data[i], 16);
            }
            string str = str_temp[0] + " " + str_temp[1] + " " + str_temp[2] + " " + str_temp[3] + " " + str_temp[4] + " " + str_temp[5] + " " + str_temp[6];
            listBox1.Items.Add(DateTime.Now.ToString() + "  发送信息: " + str); 
            try
            {
                if (radioButton1.Checked)
            {
                streamWriter.Write(Send_Data);
                // 刷新当前数据流中的数据  
                streamWriter.Flush();
            }
                else
                {
                    //第零位开始发  发7个
                    serialPort1.Write(Send_Data, 0, 7);
                }

                

            }
            catch
            {
                MessageBox.Show("发送数据失败");
            }
        }
        private void SendData(byte type)
        {
            //发送数据
            byte[] Send_Data = new byte[4] { 0xff, type, 0x0d, 0x0a };

            string[] str_temp = new string[4];
            for (int i = 0; i < 4; i++)
            {
                str_temp[i] = Convert.ToString(Send_Data[i], 16);
            }
            string str = str_temp[0] + " " + str_temp[1] + " " + str_temp[2] + " " + str_temp[3] ;
            listBox1.Items.Add(DateTime.Now.ToString() + "  发送信息: " + str);
            try
            {
                if (radioButton1.Checked)
                {
                    streamWriter.Write(Send_Data);
                    // 刷新当前数据流中的数据  
                    streamWriter.Flush();
                }
                else
                {
                    serialPort1.Write(Send_Data, 0, 4);
                }
            }
            catch
            {
                MessageBox.Show("发送数据失败");
            }
        }

        private void SendData()
        {
            if (textBox3.Text != "")
            {
                listBox1.Items.Add(DateTime.Now.ToString()+"  发送信息: " + textBox3.Text);
                if (radioButton1.Checked)
                {
                    // 往当前的数据流中写入一行字符串  
                    streamWriter.Write(textBox3.Text + "\r\n");
                    // 刷新当前数据流中的数据  
                    streamWriter.Flush();
                }
                else
                {
                    serialPort1.Write(textBox3.Text);
                }
                textBox3.Text = "";  // 清空  
            }
            else
            {
                MessageBox.Show("发送信息不能为空", "提示");
            }
        }

        #endregion
        #region  速度闭环PID发送
        private void button3_Click(object sender, EventArgs e)
        {
            float f1=-1,f2=-1,f3=-1;
            try 
            {
                f1 = float.Parse(numericUpDown1.Text);
                f2 = float.Parse(numericUpDown2.Text);
                f3 = float.Parse(numericUpDown3.Text);
            }
            catch
            {
                MessageBox.Show("请输入0~255区间二位小数");
            }

            try
            {
                    SendData(f1, Speed_P);
                    SendData(f2, Speed_I);
                    SendData(f3, Speed_D);
            }
            catch
            {
                MessageBox.Show("发送数据失败");
            }
        }
        #endregion
        #region  位置开环PID发送
        private void button4_Click(object sender, EventArgs e)
        {
            float f1 = -1, f2 = -1, f3 = -1;
            try
            {
                f1 = float.Parse(numericUpDown4.Text);
                f2 = float.Parse(numericUpDown5.Text);
                f3 = float.Parse(numericUpDown6.Text);
            }
            catch
            {
                MessageBox.Show("请输入0~255区间二位小数");
            }

            try
            {
                    SendData(f1, Position_P);
                    SendData(f2, Position_I);
                    SendData(f3, Position_D);
            }
            catch
            {
                MessageBox.Show("发送数据失败");
            }
        }
        #endregion
        #region  舵机PID发送
        private void button5_Click(object sender, EventArgs e)
        {
            float f1 = -1, f2 = -1, f3 = -1;
            try
            {
                f1 = float.Parse(numericUpDown7.Text);
                f2 = float.Parse(numericUpDown8.Text);
                f3 = float.Parse(numericUpDown9.Text);
            }
            catch
            {
                MessageBox.Show("请输入0~255区间二位小数");
            }

            try
            {
                    SendData(f1, Steer_P);
                    SendData(f2, Steer_I);
                    SendData(f3, Steer_D);
            }
            catch
            {
                MessageBox.Show("发送数据失败");
            }
        }
        #endregion
        #region 平衡PID发送
        private void button6_Click(object sender, EventArgs e)
        {
            float f1 = -1, f2 = -1, f3 = -1;
            try
            {
                f1 = float.Parse(numericUpDown10.Text);
                f2 = float.Parse(numericUpDown11.Text);
                f3 = float.Parse(numericUpDown12.Text);
            }
            catch
            {
                MessageBox.Show("请输入0~255区间二位小数");
            }

            try
            {
                    SendData(f1, Balance_P);
                    SendData(f2, Balance_I);
                    SendData(f3, Balance_D);
            }
            catch
            {
                MessageBox.Show("发送数据失败");
            }
        }
        #endregion
        #region 前后左右开车停止信号发送
        private void button7_Click(object sender, EventArgs e)
        {
            SendData(Forward);
        }

        private void button11_Click(object sender, EventArgs e)
        {
            SendData(Backward);
        }

        private void button8_Click(object sender, EventArgs e)
        {
            SendData(TurnLeft);
        }

        private void button10_Click(object sender, EventArgs e)
        {
            SendData(TurnRight);
        }

        private void button9_Click(object sender, EventArgs e)
        {
            if(GoFlag==false)
            {
                SendData(Firing);
                GoFlag = true;
                button9.Text = "停车";
            }
            else
            {
                SendData(Misfire);
                GoFlag = false;
                button9.Text = "发车";
            }
        }
        #endregion  
        #region  单个PID数据发送
        private void numericUpDown1_ValueChanged(object sender, EventArgs e)
        {
            float f1 = -1;
            try
            {
                f1 = float.Parse(numericUpDown1.Text);
            }
            catch
            {
                MessageBox.Show("请输入0~255区间二位小数");
            }

            try
            {
                SendData(f1, Speed_P);
            }
            catch
            {
                MessageBox.Show("发送数据失败");
            }
        }

        private void numericUpDown2_ValueChanged(object sender, EventArgs e)
        {
            float  f2 = -1;
            try
            {
                f2 = float.Parse(numericUpDown2.Text);
            }
            catch
            {
                MessageBox.Show("请输入0~255区间二位小数");
            }

            try
            {
                SendData(f2, Speed_I);
            }
            catch
            {
                MessageBox.Show("发送数据失败");
            }
        }

        private void numericUpDown3_ValueChanged(object sender, EventArgs e)
        {
            float  f3 = -1;
            try
            {
                f3 = float.Parse(numericUpDown3.Text);
            }
            catch
            {
                MessageBox.Show("请输入0~255区间二位小数");
            }

            try
            {
                SendData(f3, Speed_D);
            }
            catch
            {
                MessageBox.Show("发送数据失败");
            }
        }

        private void numericUpDown4_ValueChanged(object sender, EventArgs e)
        {
            float f1 = -1;
            try
            {
                f1 = float.Parse(numericUpDown4.Text);
            }
            catch
            {
                MessageBox.Show("请输入0~255区间二位小数");
            }

            try
            {
                SendData(f1, Position_P);
            }
            catch
            {
                MessageBox.Show("发送数据失败");
            }
        }

        private void numericUpDown5_ValueChanged(object sender, EventArgs e)
        {
            float  f2 = -1;
            try
            {
                f2 = float.Parse(numericUpDown5.Text);
            }
            catch
            {
                MessageBox.Show("请输入0~255区间二位小数");
            }

            try
            {
                SendData(f2, Position_I);
            }
            catch
            {
                MessageBox.Show("发送数据失败");
            }
        }

        private void numericUpDown6_ValueChanged(object sender, EventArgs e)
        {
            float  f3 = -1;
            try
            {
                f3 = float.Parse(numericUpDown6.Text);
            }
            catch
            {
                MessageBox.Show("请输入0~255区间二位小数");
            }

            try
            {
                SendData(f3, Position_D);
            }
            catch
            {
                MessageBox.Show("发送数据失败");
            }
        }

        private void numericUpDown7_ValueChanged(object sender, EventArgs e)
        {
            float f1 = -1;
            try
            {
                f1 = float.Parse(numericUpDown7.Text);
            }
            catch
            {
                MessageBox.Show("请输入0~255区间二位小数");
            }

            try
            {
                SendData(f1, Steer_P);
            }
            catch
            {
                MessageBox.Show("发送数据失败");
            }
        }

        private void numericUpDown8_ValueChanged(object sender, EventArgs e)
        {
            float f2 = -1;
            try
            {
                f2 = float.Parse(numericUpDown8.Text);
            }
            catch
            {
                MessageBox.Show("请输入0~255区间二位小数");
            }

            try
            {
                SendData(f2, Steer_I);
            }
            catch
            {
                MessageBox.Show("发送数据失败");
            }
        }

        private void numericUpDown9_ValueChanged(object sender, EventArgs e)
        {
            float  f3 = -1;
            try
            {
                f3 = float.Parse(numericUpDown9.Text);
            }
            catch
            {
                MessageBox.Show("请输入0~255区间二位小数");
            }

            try
            {
                SendData(f3, Steer_D);
            }
            catch
            {
                MessageBox.Show("发送数据失败");
            }
        }

        private void numericUpDown10_ValueChanged(object sender, EventArgs e)
        {
            float f1 = -1;
            try
            {
                f1 = float.Parse(numericUpDown10.Text);
            }
            catch
            {
                MessageBox.Show("请输入0~255区间二位小数");
            }

            try
            {
                SendData(f1, Balance_P);
            }
            catch
            {
                MessageBox.Show("发送数据失败");
            }
        }

        private void numericUpDown11_ValueChanged(object sender, EventArgs e)
        {
            float  f2 = -1;
            try
            {
                f2 = float.Parse(numericUpDown11.Text);
            }
            catch
            {
                MessageBox.Show("请输入0~255区间二位小数");
            }

            try
            {
                SendData(f2, Balance_I);
            }
            catch
            {
                MessageBox.Show("发送数据失败");
            }
        }

        private void numericUpDown12_ValueChanged(object sender, EventArgs e)
        {
            float f3 = -1;
            try
            {
                f3 = float.Parse(numericUpDown12.Text);
            }
            catch
            {
                MessageBox.Show("请输入0~255区间二位小数");
            }

            try
            {
                SendData(f3, Balance_D);
            }
            catch
            {
                MessageBox.Show("发送数据失败");
            }
        }
        #endregion

        private void timer1_Tick(object sender, EventArgs e)
        {
            #region  CCD图像绘制
            Graphics Draw_CCD1 =pictureBox1.CreateGraphics();
            Graphics Draw_CCD2 = pictureBox2.CreateGraphics();
            for (int i = 3; i < 130; i++)
            {
                Draw_CCD1.DrawLine(new Pen(Color.Red), 2 * (i - 1), 140 - (global.CCD1[i - 1]) / 2, 2 * i, 140 - (global.CCD1[i]) / 2);
            }
            for (int i = 3; i < 130; i++)
            {
                Draw_CCD2.DrawLine(new Pen(Color.Red), 2 * (i - 1), 140 - (global.CCD2[i - 1]) / 2, 2 * i, 140 - (global.CCD2[i]) / 2);
            }
            #endregion

        }

        private void button12_Click(object sender, EventArgs e)
        {
            Thread sendThread = new Thread(new ThreadStart(SendData));
            sendThread.Start(); 
        }

        private void button13_Click(object sender, EventArgs e)
        {
            listBox1.Items.Clear();
        }

        private void button15_Click(object sender, EventArgs e)
        {
            //打开串口
            if (serialPort1.IsOpen == false)
            {
                try
                {
                    serialPort1.Open();
                    button15.Text = "关闭";
                }
                catch
                {
                    MessageBox.Show("打开失败");
                }
            }
            //关闭串口
            else
            {
                try
                {
                    serialPort1.Close();
                    button15.Text = "打开";
                }
                catch
                {
                    MessageBox.Show("关闭失败");
                }
            }
        }

        private void button14_Click(object sender, EventArgs e)
        {
            //清空之前的选项
            comboBox1.Items.Clear();

            //自动搜索并且添加可用串口
            string[] PortName = SerialPort.GetPortNames();//读取
            for (int i = 0; i < PortName.Length; i++)//添加
            {
                comboBox1.Items.Add(PortName[i]);
            }

            try
            {
                comboBox1.SelectedIndex = 0;
            }
            catch
            {
                MessageBox.Show("没有可用端口");
            }
        }

        private void radioButton1_CheckedChanged(object sender, EventArgs e)
        {
            if(radioButton1.Checked)
            {
                try
                {
                    serialPort1.Close();
                    button1.Text = "打开";
                }
                catch
                {
                    MessageBox.Show("串口已关闭");
                }
                button15.Enabled = false;
                button1.Enabled = true;
            }
            if (radioButton2.Checked)
            {
                button2_Click(null, null);
                button1.Enabled = false;
                button15.Enabled = true;
            }
        }

        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            //设置端口名称
            serialPort1.PortName = comboBox1.Text;
        }

        private void comboBox2_SelectedIndexChanged(object sender, EventArgs e)
        {
            //设置波特率
            serialPort1.BaudRate = int.Parse(comboBox2.Text);
        }

        private void button16_Click(object sender, EventArgs e)
        {
            MessageBox.Show("王容是傻逼");
        }

        private void button19_Click(object sender, EventArgs e)
        {
            if (GoFlag == false)
            {
                SendData(EncoderFiring);
                GoFlag = true;
                button19.Text = "停车";
            }
            else
            {
                SendData(EncoderMisfire);
                GoFlag = false;
                button19.Text = "发车";
            }
        }

        private void button17_Click(object sender, EventArgs e)
        {
            SendData(EncoderForward);
        }

        private void button21_Click(object sender, EventArgs e)
        {
            SendData(EncoderBackward);
        }

        private void button18_Click(object sender, EventArgs e)
        {
            SendData(EncoderTurnLeft);
        }

        private void button20_Click(object sender, EventArgs e)
        {
            SendData(EncoderTurnRight);
        }

        private void numericUpDown13_ValueChanged(object sender, EventArgs e)
        {
            float f1 = -1;
            try
            {
                f1 = float.Parse(numericUpDown13.Text);
            }
            catch
            {
                MessageBox.Show("请输入-180~180区间二位小数");
            }

            try
            {
                SendData(f1, YAW);
            }
            catch
            {
                MessageBox.Show("发送数据失败");
            }
        }

        private void button22_Click(object sender, EventArgs e)
        {
            float f1 = -1,f2= -1;
            try
            {
                f1 = float.Parse(numericUpDown13.Text);
                f2 = float.Parse(numericUpDown15.Text);
            }
            catch
            {
                MessageBox.Show("请输入-180~180区间二位小数");
            }

            try
            {
                SendData(f1, YAW);
                SendData(f2, SetSpeed);
            }
            catch
            {
                MessageBox.Show("发送数据失败");
            }
        }

        private void numericUpDown14_ValueChanged(object sender, EventArgs e)
        {
            float f1 = 0;
            try
            {
                f1 = float.Parse(numericUpDown14.Text);
            }
            catch
            {
                MessageBox.Show("请输入二位小数");
            }

            try
            {
                SendData(f1, SetSpeed);
            }
            catch
            {
                MessageBox.Show("发送数据失败");
            }
        }

        private void numericUpDown15_ValueChanged(object sender, EventArgs e)
        {
            float f1 = 0;
            try
            {
                f1 = float.Parse(numericUpDown15.Text);
            }
            catch
            {
                MessageBox.Show("请输入二位小数");
            }

            try
            {
                SendData(f1, SetSpeed);
            }
            catch
            {
                MessageBox.Show("发送数据失败");
            }
        }

        private void button23_Click(object sender, EventArgs e)
        {
            SendData(Misfire);
        }


    }
    #region 全局变量
    public class global
    {
        /**********接收数据缓存****************/
        public static int USART_REC_LEN = 200;
        public static byte[] USART_RX_BUF = new byte[USART_REC_LEN];//接收缓冲,最大200个字节.
        //接收状态
        //bit15，	接收完成标志
        //bit14，	接收到0x0d
        //bit13~0，	接收到的有效字节数目
        public static int USART_RX_STA = 0;         		//接收状态标记
        public static byte[] CCD1 = new byte[130];//CCD1数组.
        public static byte[] CCD2 = new byte[130];//CCD2数组.
        public static byte[] CMOS = new byte[76810];//CMOS数组.
    }  
    #endregion
}
