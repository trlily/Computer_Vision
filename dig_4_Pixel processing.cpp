#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

//����
void InverseImage(BYTE* Img, BYTE* out, int W, int H)
{
	int ImgSize = W * H;
	for (int i = 0; i < ImgSize; i++)
	{
		out[i] = 255 - Img[i];
	}
}

//��Ⱚ ����
void BrightnessAdj(BYTE* Img, BYTE* Out, int W, int H, int times)
{
	int ImgSize = W * H;
	for (int i = 0; i < ImgSize; i++)
	{
		if (Img[i] + times < 255)
		{
			Out[i] = Img[i] + times;
		}
		else if (Img[i] + times < 0)
		{
			Out[i] = 0;
		}
		else
		{
			Out[i] = 255;
		}
	}
}

//���� ����
//���������� �Ҽ����� ���� ����� �־� �Ҽ����� ����Ѵ�
void ContrastAdj(BYTE* Img, BYTE* Out, int W, int H, int times)
{
	int ImgSize = W * H;
	for (int i = 0; i < ImgSize; i++)
	{
		if (Img[i] * times > 255.0)
		{
			Out[i] = 255;
		}
		else
		{
			Out[i] = (BYTE)(Img[i] + times);
			//���⼭ BYTE�� �ϴ� ������ ���� ��� ���� �����̱� �����̴�
		}
	}
}

//������׷� ����, ��Ⱚ ���� �˾ƺ���
void ObtainHistogram(BYTE* Img, int W, int H)
{
	int ImgSize = W * H;
	int Histo[256] = { 0 };
	
	// 256���� �ε��� �ȿ� ���� ��� 0���� �ʱ�ȭ
	// ��Ⱚ�� ������׷����� ��Ÿ�� �����̶� 
	// ��Ⱚ�� 0���� 255�� x������ �ε����� �ȴ�
	// �׷� �ش��ϴ� ���(0~255)�� ���� �ȼ� ������ y�࿡ ǥ��� ��
	// for���� ����Ͽ� �ϳ��� �ȼ��� ��Ⱚ�� ���ϰ� �ش� ��Ⱚ�� ������ plus �� ����

	//double Temp[256] = { 0 };

	for (int i = 0; i < ImgSize; i++)
	{
		// ��Ⱚ�� Histo ������ �����ϱ�
		Histo[Img[i]]++;
		// ���� ���� ���� ��Ʈ ***********************
	}

	for (int i = 0; i < 256; i++)
	{
		printf(" %d  = %d \n", i, Histo[i]);
	}

}

//���� ������׷� ********************
//�Ķ��Ÿ Ȯ��
void ObtainAHistogram(int* Histo, int* AHisto)
{
	//Histo �� ������ ���� �Ǿ����ϱ�
	//Histo [i] => ��Ⱚ�� �ش��ϴ� ����
	//i => ��Ⱚ

	/*
	������ Ǯ��:

	for (int i = 0; i < 256; i++) 
	{
		for (int j = 0; j <= i; j++) 
		{
			j��ŭ ������ ��
			AHisto[i] += Histo[j];
		}
	}
	*/

	//������ 255 �� ���߸��� ����
	for (int i = 0; i <= 255; i++)
	{
		if (i == 0)
		{
			AHisto[i] = Histo[i];
		}
		
		else
		{
			AHisto[i] = AHisto[i - 1] + Histo[i];
		}
		
	}
}

//��Ʈ��Ī
void HistogramStretching(BYTE * Img, BYTE * Out, int W, int H)
{

	int ImgSize = W * H;

	int Low;
	int High;


	for (int i = 0; i <= 255; i++)
	{
		if (Img[i] != 0)
		{
			Low = i;
			break;
		}
	}

	for (int i = 255; i >= 0; i--)
	{
		if (Img[i] != 0)
		{
			High = i;
			break;
		}
	}

	for (int i = 0; i < ImgSize; i++)
	{
		//���⵵ ���� �����ϱ� BYTE ���ֱ�
		//Out[i] = ( (Img[i] - Low) / (High - Low) )* 255;
		// ��
		//������ double ������� �ֳĸ� 0~1 ���� ���̱⶧���� 0�̳� 255 ���� �ϳ��� ������ ��..
		//so double ���� �� �ʼ� *****************************
		//�߰������� ������ �κп� 255.0 �Ҽ���
		for (int i = 0; i < ImgSize; i++)
		{
			Out[i] = ((BYTE)(Img[i] - Low) / (double)(High - Low)) * 255.0;

		}
		
	}
	
}

int main()
{

/*�⺻ ����*/ //-----------------------------------------------------------------------------------------------------------------------------------------------------------

//��� ==> "������ ����ִ� ��, �� ���� �� ��"
	//���� ��� --> ���� ��ü ���� ��� --> �⺻���� ����� ex ������, ������ ���� ���� ��ġ, ���� (ǥ����)
	//14 bytes
	BITMAPFILEHEADER hf;

	// ���� ��� --> �̹����� ��ü���� ���� --> ���μ���, ������ ...
	//40 bytes
	BITMAPINFOHEADER hinfo;

//�ȷ�Ʈ
	//�ȷ�Ʈ ����
	//4 bytes
	RGBQUAD hRGB[256];



	//������� ��üȭ
	FILE* fp; // �ּҸ� �����ؾ��ؼ� ������ó��, fp�� ���� ��� ����Ű�� �뵵
	fp = fopen("lenna.bmp", "rb"); //open���� �о�� ���� rb
	if (fp == NULL)
	{
		//������ ����Ű�� ���� ���ϸ� --> ������ ���ٸ�
		printf("File not found! \n");
	}



/*�Է� �ޱ�*/ //-----------------------------------------------------------------------------------------------------------------------------------------------------------

	//fread ���Ͽ��� �о�ͼ� ������ �����ϱ�
	//fread( ������ �޸� �ּ�(������) , ������ ũ��, ������ ����, ������ ����Ŵ(�����) )
	fread(&hf, sizeof(BITMAPFILEHEADER), 1, fp); //������ ũ�⸸ŭ �� ���� �о��� 
	fread(&hinfo, sizeof(BITMAPINFOHEADER), 1, fp); //������ ũ�⸸ŭ �� ���� �о���
	fread(hRGB, sizeof(RGBQUAD), 256, fp); // R G B blank �̷��� 4������ �̿��� 256���� �ȷ�Ʈ ����, �迭 ũ��
	//256�� �ִ� �迭���� �� �迭�� ù��° �ּҸ� ����Ų�� => �ϳ��� �θ��� �ȷ�Ʈ ������ �ϳ��� ������ ����

	int ImgSize = hinfo.biWidth * hinfo.biHeight;

	//�����Ҵ����� ������ ���ۺκк��� ��Ī�ؼ� �־��ֱ�
	//�� �ٱ��ϰ��� ���� ������ֱ�
	//�Է� �̹����� �ٱ��Ͽ� ��� �ٴ� �ž�
	BYTE* Image = (BYTE*)malloc(ImgSize);
	//ó���� ���� ��� �̹��� �ٱ��Ͽ� �������� ���� �ž�
	BYTE* Output = (BYTE*)malloc(ImgSize);


	fread(Image, sizeof(BYTE), ImgSize, fp);
	fclose(fp);

	


/*�۾� �ϱ�*/ //-----------------------------------------------------------------------------------------------------------------------------------------------------------

	int Histo[256] = { 0 };
	//�Լ� ȣ���� �� ������ �ڷ��� ����
	//�Ʒ��� �Ұ����ϴ� �迭�� �Լ� ��ȯ���� ���� ������ �� ����.
	//Histo = ObtainHistogram(Image, hinfo.biWidth, hinfo.biHeight);
	

	ObtainHistogram(Image, hinfo.biWidth, hinfo.biHeight);
	

/*��� �ϱ�*/ //-----------------------------------------------------------------------------------------------------------------------------------------------------------

	//������ ����Ű�� �հ����� fp��� ����
	fp = fopen("output_base.bmp", "wb");
	//���� fp�� ���ο� �� ���� �غ� �Ǿ���
	//fwrite( �����͸� ������ ��, ������ ũ��, ������ ����, �����͸� ������ ��) 
	fwrite(&hf, sizeof(BYTE), sizeof(BITMAPFILEHEADER), fp);
	fwrite(&hinfo, sizeof(BYTE), sizeof(BITMAPINFOHEADER), fp);
	fwrite(hRGB, sizeof(RGBQUAD), 256, fp);
	fwrite(Output, sizeof(BYTE), ImgSize, fp);
	fclose(fp);


	//�����Ҵ� �� �� ��*��*�� free ���������
	// why? �޸� ���������� �߻��Ǳ� ������ -> ��� ����ϰ� �ִٰ� �����ؼ� ��ǻ�� ���� �ڸ�������
	free(Image);
	free(Output);

	return 0;

}