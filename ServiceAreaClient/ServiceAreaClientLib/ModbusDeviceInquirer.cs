﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.Threading;
using MySql.Data.MySqlClient;

namespace ServiceAreaClientLib
{
    public class ModbusDeviceInquirer
    {
        List<ModbusDeviceInfo> _eMeterList;

		// 循环查询周期(单位为分钟)
		int _cyclePeriod = 10;

		public int CyclePeriod
		{
			get { return _cyclePeriod; }
			set { _cyclePeriod = value; }
		}

        internal List<ModbusDeviceInfo> EMeterList
        {
            get { return _eMeterList; }
            set { _eMeterList = value; }
        }

        public ModbusDeviceInquirer(List<ModbusDeviceInfo> meterInfoList)
        {
            EMeterList = meterInfoList;
        }

		/// <summary>
		/// 查询开始
		/// </summary>
		public void StartInquiry()
		{
			// 启动timer
			System.Timers.Timer timer = new System.Timers.Timer(CyclePeriod * 60 * 1000);
			timer.AutoReset = false;
			timer.Elapsed += new System.Timers.ElapsedEventHandler(TimerElapsed);
		}

		void TimerElapsed(object sender, System.Timers.ElapsedEventArgs e)
		{
			DoInquiry();
		}

        /// <summary>
        /// 电表查询执行
        /// </summary>
        public void DoInquiry()
        {
            if (null != _eMeterList)
            {
                // 对列表中的各个电表, 逐一进行查询
                for (int i = 0; i < _eMeterList.Count; i++)
                {
                    Thread inquiryThread = new Thread(delegate() { InquiryTask(_eMeterList[i]); });
                    inquiryThread.Start();
                }
            }
        }

		/// <summary>
		/// 单个电表查询线程的执行过程
		/// </summary>
		/// <param name="meterInfo"></param>
        void InquiryTask(ModbusDeviceInfo meterInfo)
        {
            TcpSocketCommunicator inquirer = new TcpSocketCommunicator();

            try
            {
                // 与设备模块进行连接(Connect)
				// 设定Receive的接收超时时间为3000毫秒
                inquirer.Connect(meterInfo.HostName, meterInfo.PortNum, 3000);

                // 向设备模块发送查询指令(Modbus协议)
                //  第一个字节是通信地址(设备号)
                //  第二个字节是功能码0x03(读数据)
                //  后面依次是读的起始地址0x0000和读长度0x004C
                //  最后两个字节是CRC16校验码
                byte[] tmpBytes = { (byte)meterInfo.DeviceNum, 0x03, 0x00, 0x00, 0x00, 0x4C };

				// 计算CRC校验码
                UInt16 crc16 = CRC16(tmpBytes, 6);
                byte crcLowByte = (byte)(crc16 & 0x00FF);
                byte crcHighByte = (byte)((crc16 & 0xFF00) >> 8);

                byte[] sendBytes = { (byte)meterInfo.DeviceNum, 0x03, 0x00, 0x00, 0x00, 0x4C, crcLowByte, crcHighByte };

				// 向设备模块发送Modbus读数查询指令
                inquirer.Send(sendBytes);

                // 接收设备模块返回的读数查询结果
                InquiryResult ir = inquirer.Receive();

                // 上报给服务器
                Report2Server(ir);

				// 保存到本地

                inquirer.Close();
            }
            catch (Exception ex)
            {
                System.Diagnostics.Trace.WriteLine(ex.ToString());
            }
        }

        void Report2Server(InquiryResult inquiryResult)
        {
            DBConnectMySQL mysql_object = new DBConnectMySQL();
            string reportStr = GetReportString(inquiryResult);
			string dbTableName = "electric_meter";
			string insertStr = "INSERT INTO " + dbTableName + " VALUES(null" + reportStr + ")";
            mysql_object.ExecuteMySqlCommand(insertStr);
        }

        public static string GetReportString(InquiryResult inquiryResult)
        {
			string reportStr = "";
			ElectricMeterDataSetting dataSets = new ElectricMeterDataSetting();
			foreach (var data in dataSets.dataArray)
			{
				if (data.Offset < inquiryResult.RcvLen - 1)
				{
					string dataStr = "";
					int idx = data.Offset;
					for (int i = 0; i < data.Length; i++)
					{
						dataStr += string.Format("{0:X}", inquiryResult.RcvBytes[idx]).PadLeft(2, '0');
						idx++;
					}
					ulong val;
					if (ulong.TryParse(dataStr, out val))
					{
						data.Value = val;
						reportStr += "," + val.ToString();
					}
				}
			}
			return reportStr;
        }

        public static UInt16 CRC16(Byte[] dat, int count)
        {
            uint crc = 0xFFFF;
            int i, j;
            for ( j=0; j<count; j++)
            {
                crc = crc ^ dat[j];
                for ( i=0; i<8; i++)
                {
                    if ((crc & 0x0001) > 0)
                    {
                        crc = crc >> 1;
                        crc = crc ^ 0xa001;
                    }
                    else
                    {
                        crc = crc >> 1;
                    }
                }
            }
            return (UInt16)(crc);
        }

    }
}
