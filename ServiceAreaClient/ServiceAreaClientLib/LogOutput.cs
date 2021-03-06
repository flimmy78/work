﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.IO;

namespace ServiceAreaClientLib
{
	public class LogOutput
	{
		static string _logName = "log.txt";

		public static string LogName
		{
			get { return _logName; }
			set { _logName = value; }
		}

		static bool _enabled = false;

		public static bool Enabled
		{
			get { return LogOutput._enabled; }
			set { LogOutput._enabled = value; }
		}


		public static void LogAppend(string logText)
		{
			if (false == Enabled)
			{
				return;
			}
			string dateTimeStr = DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss");
			StreamWriter sw = new StreamWriter(LogName, true);
			sw.WriteLine(dateTimeStr + ":\t" + logText);
			sw.Close();
		}
	}
}
