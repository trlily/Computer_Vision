#pragma warning(disable:4996) //���� �Է��Լ��� ���� error�� �߻���Ű�� ��� ����
#include <stdio.h>
#include <stdlib.h> //���� malloc ������
#include <Windows.h> //����ü ����� ����

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
	//  ==  BYTE Image [ImgSize]; �� ���� �ǹ��̴�. �ٸ� �迭�� ũ��� ������ �� �� ��� �����Ҵ���

	BYTE* Output = (BYTE*)malloc(ImgSize);
	fread(Image, sizeof(BYTE), ImgSize, fp); 

	fclose(fp);

	//-------------------------------------- �Է�





	//output1
	for (int i = 0; i < ImgSize; i++)
	{
		Output[i] = Image[i];
	}

	fp = fopen("output1.bmp", "wb");
	fwrite(&hf, sizeof(BYTE), sizeof(BITMAPFILEHEADER), fp);
	fwrite(&hinfo, sizeof(BYTE), sizeof(BITMAPINFOHEADER), fp);
	fwrite(hRGB, sizeof(RGBQUAD), 256, fp); //RGBQUAD = 4byte
	fwrite(Output, sizeof(BYTE), ImgSize, fp);
	fclose(fp);





	//output2
	for (int i = 0; i < ImgSize; i++)
	{

		if (Image[i] + 50 > 255)
		{
			Output[i] = 255;
		}

		else if (Image[i] + 50 < 0)
		{
			Output[i] = 0;
		}

		else
		{
			Output[i] = Image[i] + 50;
		}

		//�̹��� ����
		//����ϰ�; ���� �ø��� �����÷ο츦 �����ؾ��Ѵ�
	}

	fp = fopen("output2.bmp", "wb");
	fwrite(&hf, sizeof(BYTE), sizeof(BITMAPFILEHEADER), fp);
	fwrite(&hinfo, sizeof(BYTE), sizeof(BITMAPINFOHEADER), fp);
	fwrite(hRGB, sizeof(RGBQUAD), 256, fp); //RGBQUAD = 4byte
	fwrite(Output, sizeof(BYTE), ImgSize, fp);
	fclose(fp);







	//output3
	for (int i = 0; i < ImgSize; i++)
	{
		Output[i] = 255 - Image[i];
	}

	//--------------------------------------- �̹��� ó��

	fp = fopen("output3.bmp", "wb");
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