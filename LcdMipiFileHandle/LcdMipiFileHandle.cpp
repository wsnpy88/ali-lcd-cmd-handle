// LcdMipiFileHandle.cpp : �������̨Ӧ�ó������ڵ㡣
//
//TODO:��Ҫ�Ľ��ĵط�
//1.ֻ�ܴ���H:\\ILI9881_CPT5.5_MIPI.txt��һ���ļ�
//2.ֻ�����һ���ļ���
//3.�����LCD�����ļ���ʽ��Ҫ����Ԫ����ͨ��
//4.�̶ȵļܹ���Ҫ�Ż��¡���̹淶��
//5.���������һЩbug��Ҫfix
//6.����ͨ��Git���й���

#include "stdafx.h"
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

enum MyEnum
{
	INNUMBER = 0,
	INCHARACTER
};

static char shift_buf[1000][200];
static int shift_n = -1;
static int shift_count = 0;

int _tmain(int argc, _TCHAR* argv[])
{
	int n = 0;
	char a[1000][500];
	int i = 0, j = 0;
	int state = INCHARACTER;

	char *filename = "H:\\ILI9881_CPT5.5_MIPI.txt"; //�ļ���
	char commandbuf[1000][200];
	FILE *fp, *fp1;

	if ((fp = fopen(filename, "r")) == NULL) //�ж��ļ��Ƿ���ڼ��ɶ�
	{
		printf("error!");
		return -1;
	}

	while (!feof(fp))
	{

		fgets(commandbuf[i], 1024, fp);  //��ȡһ��
		i++;
	}

	fclose(fp);                     //�ر��ļ�

	int row = i;
	for (n = 0; n < 1000; n++)
	{
		i = 0; j = 0; state = INCHARACTER;

		//��ȡ�Ĵ�������
		while (commandbuf[n][i] != '\0')
		{
			if (state == INNUMBER && commandbuf[n][i] != ')' && commandbuf[n][i] != 'x' && commandbuf[n][i] != 'X' && commandbuf[n][i] != ' ')
			{
				a[n][j] = commandbuf[n][i];
				++j;
			}
			else if (commandbuf[n][i] == '0' && state == INCHARACTER)
			{
				if (commandbuf[n][i + 1] == 'x' || commandbuf[n][i + 1] == 'X')
				{
					state = INNUMBER;
				}
			}
			else if ((commandbuf[n][i] == ' ' || commandbuf[n][i] == ')') && state == INNUMBER)
			{
				state = INCHARACTER;
			}

			++i;
		}

		a[n][j] = '\0';

		//����ָ�����ַ���
		if (strstr(commandbuf[n], "LCD_ILI9881_CMD"))
		{
			shift_n++;
			shift_count = 0;

			for (i = 0; i < 8; i++)
			{
				shift_buf[shift_n][i] = '0';
			}

			//strcpy(shift_buf[shift_n], a[n]);
			for (i = 0; i < 2; i++)
			{
				shift_buf[shift_n][shift_count++] = a[n][i];
			}

		}
		else if (strstr(commandbuf[n], "LCD_ILI9881_INDEX"))
		{
			for (i = 0; i < 2; i++)
			{
				shift_buf[shift_n][shift_count++] = a[n][i];
			}

		}
	}

	fp1 = fopen("H:\\ILI9881_CPT5.5_MIPI_shift.txt", "w");
	//TODO:�ж�fp1�Ƿ�Ϊ��

	for (i = 0; i < shift_n; i++)
	{
		fprintf(fp1, "cmd%03d = <0x39>, <1>, <0>, <2>, <0x%s>;\n", i, shift_buf[i]);
	}
	fclose(fp1);

	printf("LCD mipi init code convert successed!\nSave final file in %s\n", filename);


	return 0;
}





