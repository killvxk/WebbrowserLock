using System;
using System.IO;
using System.Collections.Generic;
using System.Text;
using System.Xml;

namespace VMProtect.KeyGen
{
    public class AlgorithmRSA : Algorithm
	{
		int nBits;
		BigInteger pExponent, pModulus;
		public AlgorithmRSA(XmlNode node)
		{
			try
			{
				nBits = int.Parse(node.Attributes["bits"].Value);
				pExponent = new BigInteger(Convert.FromBase64String(node.Attributes["exp"].Value));
				pModulus = new BigInteger(Convert.FromBase64String(node.Attributes["mod"].Value));
			}
			catch (Exception ex)
			{
				throw new ArgumentException("Missed data for RSA algorithm", ex);
			}
		}

		public int Bits { get { return nBits; } }

		public byte[] Encrypt(byte[] data)
		{
			BigInteger src = new BigInteger(data);
			BigInteger res = src.modPow(pExponent, pModulus);
			return res.getBytes();
		}
	}
}