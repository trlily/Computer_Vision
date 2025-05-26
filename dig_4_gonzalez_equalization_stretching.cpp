#pragma warning(disable:4996) //기존 입력함수에 대해 error를 발생시키는 경우 방지
#include <stdio.h>
#include <stdlib.h> //동적 malloc 때문에
#include <Windows.h> //구조체 사용을 위함

void histogram(BYTE* Image, int* histo, int w, int h)
{
	int ImgSize = w * h;

	//히스토그램
	for (int i = 0; i < ImgSize; i++)
	{
		histo[Image[i]]++;
		//Image[i]는 픽셀의 밝기값이니까 밝기값에 해당하는 histo의 인덱스에 +1하겠다는 말
	}

	for (int i = 0; i < 256; i++)
	{
		printf("%d = %d \n", i, histo[i]);
	}

}

void plushistogram(int *histo, int *AHisto)
{
	
	AHisto[256] = { 0 };

	AHisto[0] = histo[0];

	for (int i = 1; i < 256; i++)
	{
		AHisto[i] = AHisto[i-1] + histo[i];
	}
	
}




void equalization()
{
	// h(i) = 최대 밝기값/Nt(픽셀 갯수) * 누적 히스토그램(i)
}

int main()
{

	BITMAPFILEHEADER hf;    // 14byte
	BITMAPINFOHEADER hinfo; // 40byte
	RGBQUAD hRGB[256];      // 1024byte

	FILE* fp;
	fp = fopen("lenna.bmp", "rb"); //읽을 거라서 rb
	if (fp == NULL) 
	{
		//NULL => 파일을 가르키고 있지 않다는 것
		printf("File not found! \n");
		return -1; // 비정상 종료라 -1이고 위에 main에서 반환값을 int로 바꿨기때문에 -1추가
	}

	//어떤 변수에 파일을 저장? ** 참고로 fread 함수의 첫번째 파라메터는 포인터 즉 주소로 받음
	fread(&hf, sizeof(BITMAPFILEHEADER), 1, fp);

	//이제 가르키고 있는중
	fread(&hinfo, sizeof(BITMAPINFOHEADER), 1, fp);

	//팔레트 정보 여기서 배열의 이름 자체가 주소이기 때문에 &를 붙이지 않는다
	fread(hRGB, sizeof(RGBQUAD), 256, fp);

	//정보를 다 읽어옴

	int ImgSize = hinfo.biWidth * hinfo.biHeight;
	
	//동적할당
	//malloc(ImgSize); //return type : " void " 
					// why? 시작주소를 반환하는데 어떤게 수용될지 모르니까
	//그래서 지정해주게되면
	BYTE * Image = (BYTE*)malloc(ImgSize); //byte type의 바구니에 담을 수 있는 변수
									//-> 형태를 맞추기위해 뒤에도 강제 형변환
									// 
	

	//  ==  BYTE Image [ImgSize]; 와 같은 의미이다. 다만 배열의 크기는 변수가 될 수 없어서 동적할당중

	BYTE* Output = (BYTE*)malloc(ImgSize);
	fread(Image, sizeof(BYTE), ImgSize, fp); 

	fclose(fp);

	//-------------------------------------- 입력

	int histo[256] = {0};
	//배열은 자동으로 포인터 전달이 된다
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




	//--------------------------------------- 이미지 처리

	fp = fopen("output.bmp", "wb");
	fwrite(&hf, sizeof(BYTE), sizeof(BITMAPFILEHEADER), fp);
	fwrite(&hinfo, sizeof(BYTE), sizeof(BITMAPINFOHEADER), fp);
	fwrite(hRGB, sizeof(RGBQUAD), 256, fp); //RGBQUAD = 4byte
	fwrite(Output, sizeof(BYTE), ImgSize, fp);
	fclose(fp);
	free(Image);
	free(Output); //메모리 누수(memory leak)현상 발생되기 때문에 free 시켜줘야해 계속 사용되고 있다고 인지 -> 사용 X

	//-------------------------------------- 출력

	return 0; //정상종료

}