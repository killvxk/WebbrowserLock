#pragma once

#define MAX_MD5_LEN						66
#define MAX_MD5_BUFFER_SIZE			1024


/* F, G and H are basic MD5 functions: selection, majority, parity */
#define F(x, y, z) (((x) & (y)) | ((~x) & (z)))
#define G(x, y, z) (((x) & (z)) | ((y) & (~z)))
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define I(x, y, z) ((y) ^ ((x) | (~z))) 

/* ROTATE_LEFT rotates x left n bits */
#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32-(n))))

/* FF, GG, HH, and II transformations for rounds 1, 2, 3, and 4 */
/* Rotation is separate from addition to prevent recomputation */

#define FF(a, b, c, d, x, s, ac) {(a) += F ((b), (c), (d)) + (x) + (UINT4)(ac); (a) = ROTATE_LEFT ((a), (s)); (a) += (b); }
#define GG(a, b, c, d, x, s, ac) {(a) += G ((b), (c), (d)) + (x) + (UINT4)(ac); (a) = ROTATE_LEFT ((a), (s)); (a) += (b); }
#define HH(a, b, c, d, x, s, ac) {(a) += H ((b), (c), (d)) + (x) + (UINT4)(ac); (a) = ROTATE_LEFT ((a), (s)); (a) += (b); }
#define II(a, b, c, d, x, s, ac) {(a) += I ((b), (c), (d)) + (x) + (UINT4)(ac); (a) = ROTATE_LEFT ((a), (s)); (a) += (b); }

/* typedef a 32 bit type */
typedef unsigned long int UINT4;

/* Data structure for MD5 (Message Digest) computation */
typedef struct {
  UINT4 i[2];                   /* number of _bits_ handled mod 2^64 */
  UINT4 buf[4];                                    /* scratch buffer */
  unsigned char in[64];                              /* input buffer */
  unsigned char digest[16];     /* actual digest after MD5Final call */
} MD5_CTX;


class CMD5ChecksumTool
{
public:
	CMD5ChecksumTool();
	~CMD5ChecksumTool();

public:
	void MD5Init (MD5_CTX *mdContext);
	void MD5Update (MD5_CTX *mdContext, unsigned char *inBuf, unsigned int inLen);
	void MD5Final (MD5_CTX *mdContext);
	LPCWSTR MD5_FileW (LPCWSTR path, int md5_len);
	LPCWSTR MD5_DataW (PVOID pData,DWORD dwLen,int md5_len);


	LPCSTR MD5_FileA (LPCSTR path, int md5_len = 32);
	LPCSTR MD5_DataA (PVOID pData,DWORD dwLen,int md5_len = 32);
private:
	static void Transform (UINT4 *buf, UINT4 *in);
private:
	static unsigned char PADDING[64]; 
	BYTE m_MD5[MAX_MD5_LEN];
	PUCHAR pBuffer;
};