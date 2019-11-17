#include <stdio.h>
#define Nb 16
#define Nk 16
#define Nr 24
typedef unsigned char u8;
typedef unsigned int u32;

u32 ROL(u32 M, int n)
{
	u32 temp = M;
	temp >>= 32 - n;
	M <<= n;
	M |= temp;
	return M;
}

void RoundKeyGeneration128(u8 MK[], u8 RK[])
{
	u32 P[8] = { 0xc3efe9db , 0x44626b02 , 0x79e27c8a , 0x78df30ec , 0x715ea49e , 0xc785da0a , 0xe04ef22a, 0xe5c40957 };
	u32 T[4] = { 0x00000000 };
	u8 n = 0;
	for (u8 i = 0; i < 4; i++) {
		T[i] = u4byte_in(MK + i * 4);
	}
	for (u8 j = 0; j < 24; j++) {
		T[0] = ROL(T[0] + ROL(P[j % 4], j), 1);
		T[1] = ROL(T[1] + ROL(P[j % 4], j + 1), 3);
		T[2] = ROL(T[2] + ROL(P[j % 4], j + 2), 6);
		T[3] = ROL(T[3] + ROL(P[j % 4], j + 3), 11);
		for (u8 i = 0; i < 4; i++) {
			RK[n] = (T[0] >> (24 - 8 * i)) & 0xff;
			n++;
		}
		for (u8 i = 0; i < 4; i++) {
			RK[n] = (T[1] >> (24 - 8 * i)) & 0xff;
			n++;
		}
		for (u8 i = 0; i < 4; i++) {
			RK[n] = (T[2] >> (24 - 8 * i)) & 0xff;
			n++;
		}
		for (u8 i = 0; i < 4; i++) {
			RK[n] = (T[1] >> (24 - 8 * i)) & 0xff;
			n++;
		}
		for (u8 i = 0; i < 4; i++) {
			RK[n] = (T[3] >> (24 - 8 * i)) & 0xff;
			n++;
		}
		for (u8 i = 0; i < 4; i++) {
			RK[n] = (T[1] >> (24 - 8 * i)) & 0xff;
			n++;
		}
	}
}

void LEA_KeySchedule(u8 MK[], u8 RK[], u32 keysize)
{
	if (keysize == 128)RoundKeyGeneration128(MK, RK);
}


u32 u4byte_in(u8* x)
{
	return (x[0] << 24) | (x[1] << 16) | (x[2] << 8) | x[3];
}

void LEA_Round(u8 S[16], u8 K[16]) {

	// 코드 채우기
}

void LEA_ENC(u8 PT[16], u8 CT[16], u8 RK[], u32 keysize) {
	int i;
	u8 temp[16];

	temp[0] = PT[0]; temp[1] = PT[1]; temp[2] = PT[2]; temp[3] = PT[3];
	temp[4] = PT[4]; temp[5] = PT[5]; temp[6] = PT[6]; temp[7] = PT[7];
	temp[8] = PT[8]; temp[9] = PT[9]; temp[10] = PT[10]; temp[11] = PT[11];
	temp[12] = PT[12]; temp[13] = PT[13]; temp[14] = PT[14]; temp[15] = PT[15];

	for (i = 0; i <= Nr - 1; i++) {
		LEA_Round(temp, RK + 16 * (i + 1));
	}
	CT[0] = temp[0]; CT[1] = temp[1]; CT[2] = temp[2]; CT[3] = temp[3];
	CT[4] = temp[4]; CT[5] = temp[5]; CT[6] = temp[6]; CT[7] = temp[7];
	CT[8] = temp[8]; CT[9] = temp[9]; CT[10] = temp[10]; CT[11] = temp[11];
	CT[12] = temp[12]; CT[13] = temp[13]; CT[14] = temp[14]; CT[15] = temp[15];
}

void LEA_DEC_Round(u8 S[16], u8 K[16]) {
	// 코드 채우기
}

void LEA_DEC(u8 PT[16], u8 CT[16], u8 RK[], u32 keysize) {
	int i;
	u8 temp[16];
	temp[0] = CT[0]; temp[1] = CT[1]; temp[2] = CT[2]; temp[3] = CT[3];
	temp[4] = CT[4]; temp[5] = CT[5]; temp[6] = CT[6]; temp[7] = CT[7];
	temp[8] = CT[8]; temp[9] = CT[9]; temp[10] = CT[10]; temp[11] = CT[11];
	temp[12] = CT[12]; temp[13] = CT[13]; temp[14] = CT[14]; temp[15] = CT[15];

	for (i = Nr - 1; i >= 0; i--) {
		LEA_DEC_Round(temp, RK + 16 * (i + 1));
	}
	PT[0] = temp[0]; PT[1] = temp[1]; PT[2] = temp[2]; PT[3] = temp[3]; 
	PT[4] = temp[4]; PT[5] = temp[5]; PT[6] = temp[6]; PT[7] = temp[7]; 
	PT[8] = temp[8]; PT[9] = temp[9]; PT[10] = temp[10]; PT[11] = temp[11]; 
	PT[12] = temp[12]; PT[13] = temp[13]; PT[14] = temp[14]; PT[15] = temp[15];	
}

int main(int argc, char* argv[]) {
	u8 i;	
	u32 keysize = 128;
	/*
	u8 PT[Nb] = { 0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E };
	u8 CT[Nb] = { 0x00, };
	u8 MK[Nb] = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff };
	u8 RK[Nr * N] = { 0x00, };

	LEA_KeySchedule(MK, RK, keysize);
	LEA_ENC(PT, CT, RK, keysize);
	printf("LEA_ENC: ");
	for (i = 0; i < 16; i++)
		printf("%02x ",CT[i]);

	LEA_DEC(PT, CT, RK, keysize);

	/*
	enc, dec 성공 시 직접 파일 넣어서 enc, dec 할 예정
	FILE *rfp, *wfp;
	u8 r;
	u32 DataLen;
	u8 *input, *output;


	fopen_s(&rfp, argv[2], "rb");
	if (rfp == NULL)//열기 실패일 때
	{
	   perror("fopen 실패");//에러 메시지 출력
	   return 0;
	}

	fseek(rfp, 0, SEEK_END);
	DataLen = ftell(rfp);
	fseek(rfp, 0, SEEK_SET);
	fopen_s(&wfp, argv[3], "wb");
	if (wfp == NULL)//열기 실패일 때
	{
	   perror("fopen 실패");//에러 메시지 출력
	   return 0;
	}

	r = DataLen % 16;
	if (r != 0)
	   r = 16 - r;

	input = calloc(DataLen + r, sizeof(u8));
	output = calloc(DataLen + r, sizeof(u8));
	fread(input, 1, DataLen, rfp);
	memset(input + DataLen, r, r);
	if (strcmp(argv[1], "ECB") == 0)
	{
	   LEA_KeySchedule(MK, RK, 128);
	   for (i = 0; i < (DataLen + r) / 16; i++) {
		  LEA_ENC(input + i * 16, output + i * 16, RK, 128);
	   }
	   fwrite(output, 1, DataLen + r, wfp);
	}
	fclose(rfp);
    fclose(wfp);
    free(input);
    free(output);
	*/
	return 0;
}
