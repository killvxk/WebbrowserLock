using System;
using System.IO;
using System.Collections.Generic;
using System.Text;
using System.Xml;

namespace VMProtect.KeyGen
{
	interface Algorithm
	{
		int Bits { get; }
		byte[] Encrypt(byte[] b);
	}

    public class Generator
    {
		// product's data
		Algorithm algo;
		byte[] pProductCode;

		// serial number's data
        String strUserName, strEMail, strHardwareID;
        DateTime? dtExpire, dtMaxBuild;
        int? nRunningTimeLimit;
        byte[] pUserData;

        public Generator(string data)
        {
			byte[] d = Convert.FromBase64String(data);
			string strXML = Encoding.UTF8.GetString(d);
			XmlDocument doc = new System.Xml.XmlDocument();
			doc.LoadXml(strXML);

			XmlNode root = doc.SelectSingleNode("/vmp-lm-product");
			if (root == null) throw new ArgumentException("Invalid data: not a product information string");

			XmlNode attrProduct = root.SelectSingleNode("@product");
			if (attrProduct == null) throw new ArgumentException("Invalid data: information about product is missed");
			pProductCode = Convert.FromBase64String(attrProduct.Value);
			if (pProductCode.Length != 8) throw new ArgumentException("Invalid data: product code has incorrect length");

			XmlNode nodeAlgo = root.SelectSingleNode("@algorithm");
			if (nodeAlgo == null) throw new ArgumentException("Invalid data: missed encryption algorithm");
			switch (nodeAlgo.Value)
			{
				case "RSA":
					algo = new AlgorithmRSA(root);
					break;
				default:
					throw new ArgumentException("Invalid data: unsupported encryption algorithm");
			}
        }

        public String UserName
        {
            get { return strUserName; }
            set 
            {
                if (value != null)
                {
                    byte[] b = Encoding.UTF8.GetBytes(value);
                    if (b.Length > 255) throw new ArgumentException(String.Format("User name is too long: {0} bytes in UTF-8, maximum is 255", b.Length));
                }
                strUserName = value; 
            }
        }

        public String EMail
        {
            get { return strEMail; }
            set
            {
                if (value != null)
                {
                    byte[] b = Encoding.UTF8.GetBytes(value);
                    if (b.Length > 255) throw new ArgumentException(String.Format("E-Mail is too long: {0} bytes in UTF-8, maximum is 255", b.Length));
                }
                strEMail = value;
            }
        }

        public DateTime? ExpirationDate
        {
            get { return dtExpire; }
            set { dtExpire = value; }
        }

        public DateTime? MaxBuildDate
        {
            get { return dtMaxBuild; }
            set { dtMaxBuild = value; }
        }

        public int? RunningTimeLimit
        {
            get { return nRunningTimeLimit; }
            set 
            { 
                if (value != null) if (value < 0 || value > 255) throw new ArgumentException("Running time limit should be from 0 to 255 minutes");
                nRunningTimeLimit = value; 
            }
        }

        public String HardwareID
        {
            get { return strHardwareID; }
            set
            { 
                if (value != null)
                {
                    byte[] b = Convert.FromBase64String(value);
                    if (b.Length == 0) throw new ArgumentException("Hardware ID has zero length, use 'null' instead");
                    if (b.Length > 32) throw new ArgumentException("Hardware ID is too long");
                    if (b.Length % 4 != 0) throw new ArgumentException("Hardware ID has invalid length");
                }
                strHardwareID = value; 
            }
        }

        public byte[] UserData
        {
            get { return pUserData; }
            set 
            {
                if (value != null)
                {
                    if (value.Length > 255) throw new ArgumentException("User data cannot exceed 255 bytes");
                    if (value.Length == 0) throw new ArgumentException("User data has zero length, use 'null' instead");
                }
                pUserData = value; 
            }
        }

        public String Generate()
        {
			byte[] bs = BuildSerialNumber();
			bs = AddPadding(bs);
			bs = algo.Encrypt(bs);
			return Convert.ToBase64String(bs, Base64FormattingOptions.InsertLineBreaks);
        }

        //////////////////////////////////////////////////////////////////////////////////////////////////////
        enum eChunks : byte
        {
            VERSION = 0x01,	//	1 byte of data - version
            USER_NAME = 0x02,	//	1 + N bytes - length + N bytes of customer's name (without enging \0).
            EMAIL = 0x03,	//	1 + N bytes - length + N bytes of customer's email (without ending \0).
            HWID  = 0x04,	//	1 + N bytes - length + N bytes of hardware id (N % 4 == 0)
            EXP_DATE = 0x05,	//	4 bytes - (year << 16) + (month << 8) + (day)
            RUNNING_TIME_LIMIT = 0x06,	//	1 byte - number of minutes
            PRODUCT_CODE = 0x07,	//	8 bytes - used for decrypting some parts of exe-file
            USER_DATA = 0x08,	//	1 + N bytes - length + N bytes of user data
            MAX_BUILD = 0x09,	//	4 bytes - (year << 16) + (month << 8) + (day)

            END = 0xFF	//	4 bytes - checksum: the first four bytes of sha-1 hash from the data before that chunk
        };
  

		void StoreDate(MemoryStream s, DateTime dt)
		{
			s.WriteByte((byte)dt.Day);
			s.WriteByte((byte)dt.Month);
			s.WriteByte((byte)(dt.Year % 256));
			s.WriteByte((byte)(dt.Year/ 256));
		}

        byte [] BuildSerialNumber()
        {
			MemoryStream s = new MemoryStream();

			// version
			s.WriteByte((byte)eChunks.VERSION);
			s.WriteByte(1);

			if (strUserName != null)
			{
				s.WriteByte((byte)eChunks.USER_NAME);
				byte[] b = Encoding.UTF8.GetBytes(strUserName);
				s.WriteByte((byte)b.Length);
				s.Write(b, 0, b.Length);
			}

			if (strEMail != null)
			{
				s.WriteByte((byte)eChunks.EMAIL);
				byte[] b = Encoding.UTF8.GetBytes(strEMail);
				s.WriteByte((byte)b.Length);
				s.Write(b, 0, b.Length);
			}

			if (strHardwareID != null)
			{
				s.WriteByte((byte)eChunks.HWID);
				byte[] b = Convert.FromBase64String(strHardwareID);
				s.WriteByte((byte)b.Length);
				s.Write(b, 0, b.Length);
			}

			if (dtExpire != null)
			{
				s.WriteByte((byte)eChunks.EXP_DATE);
				StoreDate(s, dtExpire.Value);
			}

			if (nRunningTimeLimit != null)
			{
				s.WriteByte((byte)eChunks.RUNNING_TIME_LIMIT);
				s.WriteByte((byte)nRunningTimeLimit.Value);
			}

			s.WriteByte((byte)eChunks.PRODUCT_CODE);
			s.Write(pProductCode, 0, pProductCode.Length);

			if (pUserData != null)
			{
				s.WriteByte((byte)eChunks.USER_DATA);
				s.WriteByte((byte)pUserData.Length);
				s.Write(pUserData, 0, pUserData.Length);
			}

			if (dtMaxBuild != null)
			{
				s.WriteByte((byte)eChunks.MAX_BUILD);
				StoreDate(s, dtMaxBuild.Value);
			}

			// checksum
			byte[] bData = s.ToArray();
			byte[] hash = new System.Security.Cryptography.SHA1Managed().ComputeHash(bData);
			s.WriteByte((byte)eChunks.END);
			for (int i = 3; i >= 0; i--) s.WriteByte(hash[i]); // in reverse order
	
			// all done
			return s.ToArray();
        }

		byte[] AddPadding(byte[] bs)
		{
			int nMinPadding = 8 + 3;
			int nMaxPadding = nMinPadding + 16;
			int nMaxBytes = algo.Bits / 8;
			if (bs.Length + nMinPadding > nMaxBytes) throw new Exception("Serial number is too long for this algorithm");

			MemoryStream s = new MemoryStream();

			Random r = new Random();

			int nMaxPaddingAccordingToMaxBytes = nMaxBytes - bs.Length;
			if (nMaxPaddingAccordingToMaxBytes < nMaxPadding) nMaxPadding = nMaxPaddingAccordingToMaxBytes;
			int nPaddingBytes = nMinPadding;
			if (nMaxPadding > nMinPadding) nPaddingBytes += r.Next(nMaxPadding - nMinPadding);

			s.WriteByte(0);
			s.WriteByte(2);
			for (int i = 0; i < nPaddingBytes - 1; i++) s.WriteByte((byte)r.Next(1, 255));
			s.WriteByte(0);

			s.Write(bs, 0, bs.Length);

			int nRest = nMaxBytes - (int)s.Length;
			while (nRest-- > 0) s.WriteByte((byte)r.Next(255));

			return s.ToArray();
		}
    }
}
