﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.Net;
using System.Net.Sockets;
using System.IO;
using System.Threading;

namespace UpdaterServer
{
	class Program
	{
		#region 全部字段

		// 端口号:1981 用于ServiceAreaClient向ServiceAreaServer(中继服务端)发送采集数据report
		// 端口号:1982 用于UpdaterServer向UpdaterClient传送更新文件
		// 端口号:1983 用于ServiceAreaClient监听接收消息
		static int _portTransData = 1982;

		public static int PortTransData
		{
			get { return _portTransData; }
			set { _portTransData = value; }
		}

		static int _portListener = 1983;

		public static int PortListener
		{
			get { return Program._portListener; }
			set { Program._portListener = value; }
		}

		// 文件传送时的包的大小(10KB)
		static int _packetSize = 10 * 1024;

		public static int PacketSize
		{
			get { return Program._packetSize; }
			set { Program._packetSize = value; }
		}

		static string _updateFileName = @"\Send\dummy.exe";

		public static string UpdateFileName
		{
			get { return Program._updateFileName; }
			set { Program._updateFileName = value; }
		}

		#endregion

		static void Main(string[] args)
		{
			while (true)
			{
				Console.WriteLine("0: Exit");
				Console.WriteLine("1: Say Hello");
				Console.WriteLine("2: Update EXE");
                Console.WriteLine("3: Update DLL");
				Console.WriteLine("4: Update Setting");
				Console.WriteLine("5: Update Config");

				string cmdStr = Console.ReadLine();
				if (cmdStr.Equals("0"))
				{
					break;
				}
				else if (cmdStr.Equals("1"))
				{
					SayHello();
					Console.WriteLine("Say Hello结束. 按任意键继续...");
					Console.ReadKey();
				}
				else if (cmdStr.Equals("2"))
				{
					UpdateEXE();
					Console.WriteLine("Update EXE结束. 按任意键继续...");
					Console.ReadKey();
				}
                else if (cmdStr.Equals("3"))
                {
                    UpdateDLL();
                    Console.WriteLine("Update DLL结束. 按任意键继续...");
                    Console.ReadKey();
                }
				else if (cmdStr.Equals("4"))
				{
					UpdateSetting();
					Console.WriteLine("Update Setting结束, 按任意键继续...");
					Console.ReadKey();
				}
				else if (cmdStr.Equals("5"))
				{
                    UpdateConfig();
					Console.WriteLine("Update Config结束, 按任意键继续...");
					Console.ReadKey();
				}
				else
				{
					Console.WriteLine("无效的命令输入!. 按任意键继续...");
					Console.ReadKey();
				}
			}
		}

		#region 内部方法

		static void SayHello()
		{
			// ① 获得输入的对端IP地址
			IPAddress clientIpAddr = GetTargetIpAddr();

			IPEndPoint ipep = new IPEndPoint(clientIpAddr, PortListener);
			Socket cSocket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
			string cmdStr = "Hello There!";
			try
			{
				cSocket.Connect(ipep);
				Console.WriteLine("Connect成功!");
				Thread.Sleep(1000);

				// 向客户端发送更新指示
				string sndStr = cmdStr;
				byte[] sndBytes = Encoding.ASCII.GetBytes(sndStr);
				cSocket.Send(sndBytes);
				Console.WriteLine("\"" + cmdStr + "\" 送信成功!");
				while (true)
				{
					byte[] recvBytes = new byte[1024];
					int bytes = cSocket.Receive(recvBytes, recvBytes.Length, 0);
					if (0 == bytes)
					{
						break;
					}
					string recvStr = Encoding.ASCII.GetString(recvBytes, 0, bytes);
					Console.WriteLine(clientIpAddr.ToString() + " : {0}", recvStr);
					if (recvStr.ToLower().Equals("form close"))
					{
						break;
					}
				}
				cSocket.Close();
			}
			catch (Exception ex)
			{
				Console.WriteLine(ex.ToString());
			}
			finally
			{
				if (cSocket.Connected)
				{
					cSocket.Close();
				}
			}
		}

		static void UpdateEXE()
		{
			UpdateFile("Update EXE");
		}

        static void UpdateDLL()
        {
            UpdateFile("Update DLL");
        }

		static void UpdateSetting()
		{
			UpdateFile("Update Setting");
		}

		static void UpdateConfig()
		{
			UpdateFile("Update Config");
		}

		static void UpdateFile(String cmdStr)
		{
			// ① 获得输入的对端IP地址
			IPAddress clientIpAddr = GetTargetIpAddr();

			// ② 确定更新文件所在的路径和文件名
            UpdateFileName = GetUpdateFileFullName();

			Console.WriteLine("更新开始: " + clientIpAddr.ToString());
			Thread.Sleep(1000);
			// 首先以客户端的身份连接ServiceAreaClient的消息监听线程, 并向其发送更新指示
			IPEndPoint ipep = new IPEndPoint(clientIpAddr, PortListener);
			Socket cSocket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            Socket sSocket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);

			try
			{
				cSocket.Connect(ipep);
				Console.WriteLine("Connect成功!");
				Thread.Sleep(1000);

				// 向客户端发送更新指示
				string sndStr = cmdStr;
				byte[] sndBytes = Encoding.ASCII.GetBytes(sndStr);
				cSocket.Send(sndBytes);
				Console.WriteLine("\"" + cmdStr + "\" 送信成功!");
				while (true)
				{
					byte[] recvBytes = new byte[1024];
					int bytes = cSocket.Receive(recvBytes, recvBytes.Length, 0);
					if (0 == bytes)
					{
						break;
					}
					string recvStr = Encoding.ASCII.GetString(recvBytes, 0, bytes);
					Console.WriteLine(clientIpAddr.ToString() + " : {0}", recvStr);
                    if (recvStr.ToLower().Equals("form close"))
                    {
                        break;
                    }
				}
				cSocket.Close();

				// 接着再以服务器的身份等待来自客户端更新程序(UpdaterClient)的更新就绪应答
				ipep = new IPEndPoint(IPAddress.Any, PortTransData);
				sSocket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
				sSocket.Bind(ipep);												// 绑定
				sSocket.Listen(10);												// 监听

				Console.WriteLine(@"等待对端UpdaterClient的连接...");

                while (true)
                {
                    cSocket = sSocket.Accept();							        // 当有可用的客户端连接尝试时执行，并返回一个新的socket,用于与客户端之间的通信
                    IPEndPoint clientip = (IPEndPoint)cSocket.RemoteEndPoint;
					Console.WriteLine(@"与对端UpdaterClient建立连接: " + clientip.Address + @" 端口号: " + clientip.Port);

                    while (true)
                    {
                        byte[] recvBytes = new byte[1024];
                        int bytes = cSocket.Receive(recvBytes, recvBytes.Length, 0);	// 从客户端接受消息
						if (0 == bytes)
						{
							break;
						}
                        string recvStr = Encoding.ASCII.GetString(recvBytes, 0, bytes);
                        Console.WriteLine(clientIpAddr.ToString() + " : {0}", recvStr);	// 把客户端传来的信息显示出来

                        if (recvStr.Equals("Update Start Ready"))
                        {
							// 更新文件传输(发送)
							FileDataSend(UpdateFileName, cSocket);
                            Console.WriteLine("---> " + clientip.Address + " 文件发送完成!");
                            cSocket.Close();
                            break;
                        }
                        else if (recvStr.Equals("UpdaterClient.exe Close"))
                        {
                            cSocket.Close();
                            return;
                        }
                        else
                        {
                        }
                        
                    }
                }
			}
			catch (Exception ex)
			{
				Console.WriteLine(ex.ToString());
			}
			finally
			{
                if (cSocket.Connected)
                {
                    cSocket.Close();
                }
                if (sSocket.Connected)
                {
                    sSocket.Close();
                }
            }
		}

		static string GetModulePath()
		{
			string retStr = System.Diagnostics.Process.GetCurrentProcess().MainModule.FileName;
			int idx = -1;
			if (-1 != (idx = retStr.LastIndexOf(@"\")))
			{
				retStr = retStr.Remove(idx);
			}
			return retStr;
		}

		static int SendVarData(Socket s, byte[] data) // return integer indicate how many data sent.
		{
			int total = 0;
			int size = data.Length;
			int dataleft = size;
			int sent;
			byte[] datasize = new byte[4];
			datasize = BitConverter.GetBytes(size);
			sent = s.Send(datasize);						// send the size of data array.

			while (total < size)
			{
				sent = s.Send(data, total, dataleft, SocketFlags.None);
				total += sent;
				dataleft -= sent;
			}

			return total;  
		}

		static void FileDataSend(string full_name, Socket sClient)
		{
			FileInfo sfi = new FileInfo(full_name);
			if (!sfi.Exists)
			{
                Console.WriteLine("Error! " + sfi.FullName + " 不存在!");
				return;
			}
			FileStream sfs = sfi.OpenRead();

			int sndPacket = (int)(sfs.Length / PacketSize);
			int lastPacket = (int)(sfs.Length - sndPacket * PacketSize);

			byte[] data = new byte[PacketSize];
			for (int i = 0; i < sndPacket; i++)
			{
				sfs.Read(data, 0, data.Length);
				SendVarData(sClient, data);
			}

			if (lastPacket != 0)
			{
				data = new byte[lastPacket];
				sfs.Read(data, 0, data.Length);
				SendVarData(sClient, data);
			}

			sfs.Close();
		}

		/// <summary>
		/// 取得更新对端的IP地址输入
		/// </summary>
		/// <returns></returns>
		static IPAddress GetTargetIpAddr()
		{
			string clientIpStr = "";
			IPAddress clientIpAddr;
			while (true)
			{
				Console.WriteLine("请输入更新对端的IP地址:");
				clientIpStr = Console.ReadLine();
				if (!IPAddress.TryParse(clientIpStr, out clientIpAddr))
				{
					Console.WriteLine("输入的IP地址不正确");
				}
				else
				{
					break;
				}
			}
			return clientIpAddr;
		}

		static string GetUpdateFileFullName()
		{
			string fullName = "";
			while (true)
			{
				Console.WriteLine("请输入更新文件的完整路径和文件名:");
				fullName = Console.ReadLine();
				if (File.Exists(fullName))
				{
					break;
				}
				else
				{
					Console.WriteLine("指定的路径名或者文件名不正确!");
					Thread.Sleep(2000);
				}
			}
			return fullName;
		}

		#endregion
	}
}
