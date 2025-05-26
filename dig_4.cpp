#pragma warning(disable:4996) //���� �Է��Լ��� ���� error�� �߻���Ű�� ��� ����
#include <stdio.h>
#include <stdlib.h> //���� malloc ������
#include <Windows.h> //����ü ����� ����


int ObtainHistogram(BYTE* Img, int* Histo, int W, int H, int T_old)
{
	//������׷� �Լ�

	int ImgSize = W * H; // �̹��� ������
	double Temp[256] = { 0 }; // 0���� �ʱ�ȭ
	for (int i = 0; i < ImgSize; i++) {
		Histo[Img[i]]++; // Img �ȼ��� ��Ⱚ Histo�� �����ϱ�
	}

	for (int i = 0; i < 256; i++)
	{
		{
			//printf("%d %d \n", i, Histo[i]);
		}

	}
	printf("\n\n");

	int Low = 0, High = 255;

	// Low, High ��� (������ �ʿ�)
	for (int i = 0; i < 256; i++) {
		if (Histo[i] > 0) {
			Low = i;
			break;
		}
	}
	for (int i = 255; i >= 0; i--) {
		if (Histo[i] > 0) {
			High = i;
			break;
		}
	}

	// T_old�� 0�̸� �ʱ� T �� ���� (ó�� ������ ���)
	if (T_old == 0) {
		int T_init = (Low + High) / 2;
		printf("�ʱ� T ������: %d\n", T_init);
		return T_init;
	}



	/*

	if (T_old == 0)
	{
		// 1. ���� ���� ���� ���� ���� ū ���� ��⸦ ���ϰ� 2�� �������� T�� ��´�


		//ó�� ���� �� �� for�� �ۿ��� �⺻ �����ؾ���

		for (int i = 0; i < 256; i++)
		{

			if (Histo[i] > 0)
			{
				//printf("%d %d \n", i, Histo[i]);
				Low = i;
				//printf("%d \n", Low); // ���� ���� ���
				break;
			}


		}

		for (int i = 255; i >= 0; i--)
		{

			if (Histo[i] > 0)
			{
				//printf("%d %d \n", i, Histo[i]);
				High = i;
				//printf("%d", High); //���� ū ��Ⱚ(ũ��)
				break;
			}
		}
		printf("\n");


		//printf("%d  %d \n", Low, High);
		int T = (Low + High) / 2;
		//printf("T = %d \n \n", T);
	}*/


	// 2. T���� �������� ������ �ִ� ��Ⱚ�� ���� G1, ������ �ִ� ��Ⱚ�� ���� G2�� ������

	int G1 = 0;
	int G2 = 0;
	int sum1 = 0;
	int sum2 = 0;


	for (int i = Low; i < T_old; i++)
	{
		G1 = G1 + (Histo[i] * i);

		sum1 = sum1 + Histo[i];
		//printf("G1 ��Ⱚ�� �� �� : %d    Histo[i] ��Ⱚ�� �ִ� ���� : %d   i ��Ⱚ : %d    �� �ȼ� ���� : %d \n", G1, Histo[i], i, sum1);


	}

	//printf("\n\n ----------------- \n\n");

	for (int i = T_old; i <= High; i++)
	{
		G2 = G2 + (Histo[i] * i);

		sum2 = sum2 + Histo[i];
		//printf("G2 ��Ⱚ�� �� �� : %d     Histo[i] ��Ⱚ�� �ִ� ���� : %d    i ��Ⱚ i : %d    sum2 �� �ȼ� ���� : %d\n", G2, Histo[i], i, sum2);
	}



	// 3. G1 ������ �ȼ� ��Ⱚ�� ��ü ��հ��� m1, G2 ������ �ȼ� ��Ⱚ�� ��ü ��հ��� m2�� �д�

	int m1 = 0;
	int m2 = 0;

	m1 = G1 / sum1;
	//printf("%d \n", m1);

	m2 = G2 / sum2;
	//printf("%d \n", m2);


	// 4. new T = (m1 + m2)/2

	int new_T = 0;
	new_T = (m1 + m2) / 2;

	//printf("new_T = %d \n", new_T);

	// 5. |new T - T| < ���� ���� 

	/*
	int ep = 0;
	ep = abs(new_T - T);
	*/

	printf("T = %d, new_T = %d, old_T = %d \n", new_T, T_old);

	return new_T;
}



int main()
{

	BITMAPFILEHEADER hf;    // 14byte
	BITMAPINFOHEADER hinfo; // 40byte
	RGBQUAD hRGB[256];      // 1024byte

	FILE* fp;
	fp = fopen("coin.bmp", "rb"); //���� �Ŷ� rb
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
	BYTE* Image = (BYTE*)malloc(ImgSize); //byte type�� �ٱ��Ͽ� ���� �� �ִ� ����
	//-> ���¸� ���߱����� �ڿ��� ���� ����ȯ
//  ==  BYTE Image [ImgSize]; �� ���� �ǹ��̴�. �ٸ� �迭�� ũ��� ������ �� �� ��� �����Ҵ���

	BYTE* Output = (BYTE*)malloc(ImgSize);
	fread(Image, sizeof(BYTE), ImgSize, fp);

	memcpy(Output, Image, ImgSize);  // �ܼ� ����

	fclose(fp);

	//-------------------------------------- �Է�


	int Histo[256] = { 0 };  // �� ���⿡ ����!

	int T = 0; // �ʱ� T�� �߰���

	while (1)
	{
		int new_T = ObtainHistogram(Image, Histo, hinfo.biWidth, hinfo.biHeight, T);

		if (abs(new_T - T) < 3)
		{
			printf("%d\n", new_T);
			break;
		}

		else
		{
			printf("T : %d,  new_T = %d", T, new_T);
		}

		T = new_T; // T ������ ������ ���� ���� �߻�
	}


	for (int i = 0; i < ImgSize; i++) 
	{
		if (Image[i] < T)
			Output[i] = 0;
		else
			Output[i] = 255;
	}

	//--------------------------------------- �̹��� ó��
	
	fp = fopen("GonZalez.bmp", "wb");
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




// GonZalez �˰���

// 1. ���� ���� ���� ���� ���� ū ���� ��⸦ ���ϰ� 2�� �������� T�� ��´�
// 2. T���� �������� ������ �ִ� ��Ⱚ�� ���� G1, ������ �ִ� ��Ⱚ�� ���� G2�� �Ѵ� (�� ��Ʈ�� ������)
// 3. G1 ������ �ȼ� ��Ⱚ�� ��ü ��հ��� m1, G2 ������ �ȼ� ��Ⱚ�� ��ü ��հ��� m2�� �д�
// 4. new T = (m1 + m2)/2
// 5. |new T - T| < ���� ���� 
// 6_1. ���� ���� T = new T �ϰ� ����
// 6_2. ���� ���̸� new T�� �ݿ��� ���� 2������ �ٽ� ���ư��� �ݺ�

