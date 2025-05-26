#pragma warning(disable:4996) //���� �Է��Լ��� ���� error�� �߻���Ű�� ��� ����
#include <stdio.h>
#include <stdlib.h> //���� malloc ������
#include <Windows.h> //����ü ����� ����

void histogram(BYTE* Image, int* histo, int w, int h)
{
	int ImgSize = w * h;

	//������׷�
	for (int i = 0; i < ImgSize; i++)
	{
		histo[Image[i]]++;
		//Image[i]�� �ȼ��� ��Ⱚ�̴ϱ� ��Ⱚ�� �ش��ϴ� histo�� �ε����� +1�ϰڴٴ� ��
	}

	for (int i = 0; i < 256; i++)
	{
		printf("%d = %d \n", i, histo[i]);
	}

}

int gonzalez(int *histo, int w, int h)
{

	//�ּ� �ִ� ���ϱ�
	int min;
	for (int i = 0; i < 255; i++)
	{
		if (histo[i] != 0)
		{
			printf("min = %d \n", i);
			min = i;
			break;
		}
	}

	int max;
	for (int i = 255; i > 0; i--)
	{
		if (histo[i] != 0)
		{
			printf("max = %d \n", i);
			max = i;
			break;
		}
	}

	int T; 
	T = (min + max) / 2;

	int sum1 = 0;
	int cnt1 = 0;

	int sum2 = 0;
	int cnt2 = 0;

	int m1;
	int m2;

	int new_T;
	int term;

	while (1)
	{
		sum1 = 0;
		cnt1 = 0;
		sum2 = 0;
		cnt2 = 0;

		for (int i = 0; i < T; i++)
		{
			sum1 = sum1 + (histo[i] * i); // ��Ⱚ*����
			cnt1 = cnt1 + histo[i]; //���� ����
		}
		
		m1 = sum1 / cnt1;


		
		for (int i = T; i < 256; i++)
		{
			sum2 = sum2 + (histo[i] * i); // ��Ⱚ*����
			cnt2 = cnt2 + histo[i]; //���� ����
		}
		
		m2 = sum2 / cnt2;

		//printf("%d, %d, %d, %d, %d, %d", sum1, cnt1, m1, sum2, cnt2, m2);

		
		new_T = (m1 + m2) / 2;

		if (abs(T - new_T))
		{
			T = new_T;
			break;
		}

		T = new_T;

	}
	

	return T;

}

int main()
{

	BITMAPFILEHEADER hf;    // 14byte
	BITMAPINFOHEADER hinfo; // 40byte
	RGBQUAD hRGB[256];      // 1024byte

	FILE* fp;
	fp = fopen("lenna.bmp", "rb"); //���� �Ŷ� rb
	if (fp == NULL) 
	{
		//NULL => ������ ����Ű�� ���� �ʴٴ� ��
		printf("File not found! \n");
		return -1; // ������ ����� -1�̰� ���� main���� ��ȯ���� int�� �ٲ�⶧���� -1�߰�
	}

	//� ������ ������ ����? ** ����� fread �Լ��� ù��° �Ķ���ʹ� ������ �� �ּҷ� ����
	fread(&hf, sizeof(BITMAPFILEHEADER), 1, fp);

	//���� ����Ű�� �ִ���
	fread(&hinfo, sizeof(BITMAPINFOHEADER), 1, fp);

	//�ȷ�Ʈ ���� ���⼭ �迭�� �̸� ��ü�� �ּ��̱� ������ &�� ������ �ʴ´�
	fread(hRGB, sizeof(RGBQUAD), 256, fp);

	//������ �� �о��

	int ImgSize = hinfo.biWidth * hinfo.biHeight;
	
	//�����Ҵ�
	//malloc(ImgSize); //return type : " void " 
					// why? �����ּҸ� ��ȯ�ϴµ� ��� ������� �𸣴ϱ�
	//�׷��� �������ְԵǸ�
	BYTE * Image = (BYTE*)malloc(ImgSize); //byte type�� �ٱ��Ͽ� ���� �� �ִ� ����
									//-> ���¸� ���߱����� �ڿ��� ���� ����ȯ
									// 
	

	//  ==  BYTE Image [ImgSize]; �� ���� �ǹ��̴�. �ٸ� �迭�� ũ��� ������ �� �� ��� �����Ҵ���

	BYTE* Output = (BYTE*)malloc(ImgSize);
	fread(Image, sizeof(BYTE), ImgSize, fp); 

	fclose(fp);

	//-------------------------------------- �Է�

	int histo[256] = {0};
	//�迭�� �ڵ����� ������ ������ �ȴ�
	int w = hinfo.biWidth;
	int h = hinfo.biHeight;
	histogram(Image, histo, w, h);
	int term;

	int T;
	T = gonzalez(histo, w, h);

	for (int i = 0; i < ImgSize; i++)
	{
		if (Image[i] < T)
		{
			Output[i] = 0;
		}

		else
		{
			Output[i] = 255;
		}
	}




	//--------------------------------------- �̹��� ó��

	fp = fopen("output.bmp", "wb");
	fwrite(&hf, sizeof(BYTE), sizeof(BITMAPFILEHEADER), fp);
	fwrite(&hinfo, sizeof(BYTE), sizeof(BITMAPINFOHEADER), fp);
	fwrite(hRGB, sizeof(RGBQUAD), 256, fp); //RGBQUAD = 4byte
	fwrite(Output, sizeof(BYTE), ImgSize, fp);
	fclose(fp);
	free(Image);
	free(Output); //�޸� ����(memory leak)���� �߻��Ǳ� ������ free ��������� ��� ���ǰ� �ִٰ� ���� -> ��� X

	//-------------------------------------- ���

	return 0; //��������

}