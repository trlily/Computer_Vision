#pragma warning(disable:4996) //기존 입력함수에 대해 error를 발생시키는 경우 방지
#include <stdio.h>
#include <stdlib.h> //동적 malloc 때문에
#include <Windows.h> //구조체 사용을 위함

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
	//  ==  BYTE Image [ImgSize]; 와 같은 의미이다. 다만 배열의 크기는 변수가 될 수 없어서 동적할당중

	BYTE* Output = (BYTE*)malloc(ImgSize);
	fread(Image, sizeof(BYTE), ImgSize, fp); 

	fclose(fp);

	//-------------------------------------- 입력





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

		//이미지 반전
		//밝게하고싶어서 값을 올릴땐 오버플로우를 주의해야한다
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

	//--------------------------------------- 이미지 처리

	fp = fopen("output3.bmp", "wb");
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