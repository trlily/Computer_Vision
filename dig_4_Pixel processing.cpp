#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

//반전
void InverseImage(BYTE* Img, BYTE* out, int W, int H)
{
	int ImgSize = W * H;
	for (int i = 0; i < ImgSize; i++)
	{
		out[i] = 255 - Img[i];
	}
}

//밝기값 조정
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

//대조 조정
//곱셈에서는 소수점이 나올 우려가 있어 소수까지 고려한다
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
			//여기서 BYTE를 하는 이유는 실제 밝기 값은 정수이기 때문이다
		}
	}
}

//히스토그램 구성, 밝기값 분포 알아보기
void ObtainHistogram(BYTE* Img, int W, int H)
{
	int ImgSize = W * H;
	int Histo[256] = { 0 };
	
	// 256개의 인덱스 안에 값들 모두 0으로 초기화
	// 밝기값을 히스토그램으로 나타낼 예정이라 
	// 밝기값인 0부터 255가 x축으로 인덱스가 된다
	// 그럼 해당하는 밝기(0~255)를 가진 픽셀 갯수가 y축에 표기될 것
	// for문을 사용하여 하나씩 픽셀의 밝기값을 구하고 해당 밝기값에 갯수를 plus 할 예정

	//double Temp[256] = { 0 };

	for (int i = 0; i < ImgSize; i++)
	{
		// 밝기값인 Histo 모음에 누적하기
		Histo[Img[i]]++;
		// 전혀 예상 못한 파트 ***********************
	}

	for (int i = 0; i < 256; i++)
	{
		printf(" %d  = %d \n", i, Histo[i]);
	}

}

//누적 히스토그램 ********************
//파라메타 확인
void ObtainAHistogram(int* Histo, int* AHisto)
{
	//Histo 는 위에서 정의 되었으니까
	//Histo [i] => 밝기값에 해당하는 갯수
	//i => 밝기값

	/*
	교수님 풀이:

	for (int i = 0; i < 256; i++) 
	{
		for (int j = 0; j <= i; j++) 
		{
			j만큼 누적된 값
			AHisto[i] += Histo[j];
		}
	}
	*/

	//마지막 255 안 빠뜨리게 조심
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

//스트레칭
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
		//여기도 곱셈 들어갔으니까 BYTE 해주기
		//Out[i] = ( (Img[i] - Low) / (High - Low) )* 255;
		// ↓
		//무조건 double 써줘야해 왜냐면 0~1 사이 값이기때문에 0이나 255 둘중 하나가 나오게 돼..
		//so double 적는 거 필수 *****************************
		//추가적으로 마지막 부분에 255.0 소수저
		for (int i = 0; i < ImgSize; i++)
		{
			Out[i] = ((BYTE)(Img[i] - Low) / (double)(High - Low)) * 255.0;

		}
		
	}
	
}

int main()
{

/*기본 세팅*/ //-----------------------------------------------------------------------------------------------------------------------------------------------------------

//헤더 ==> "정보가 들어있는 것, 즉 설명서 한 장"
	//파일 헤더 --> 파일 전체 정보 요약 --> 기본적인 내용들 ex 사이즈, 데이터 실제 시작 위치, 압축 (표지판)
	//14 bytes
	BITMAPFILEHEADER hf;

	// 영상 헤더 --> 이미지의 구체적인 설명 --> 가로세로, 압축방식 ...
	//40 bytes
	BITMAPINFOHEADER hinfo;

//팔레트
	//팔레트 정보
	//4 bytes
	RGBQUAD hRGB[256];



	//영상헤더 구체화
	FILE* fp; // 주소를 추적해야해서 포인터처리, fp는 무언갈 잠깐 가리키는 용도
	fp = fopen("lenna.bmp", "rb"); //open으로 읽어올 때는 rb
	if (fp == NULL)
	{
		//파일을 가르키고 있지 못하면 --> 파일이 없다면
		printf("File not found! \n");
	}



/*입력 받기*/ //-----------------------------------------------------------------------------------------------------------------------------------------------------------

	//fread 파일에서 읽어와서 변수에 저장하기
	//fread( 저장할 메모리 주소(목적지) , 데이터 크기, 데이터 갯수, 파일을 가리킴(출발지) )
	fread(&hf, sizeof(BITMAPFILEHEADER), 1, fp); //데이터 크기만큼 한 번만 읽어줘 
	fread(&hinfo, sizeof(BITMAPINFOHEADER), 1, fp); //데이터 크기만큼 한 번만 읽어줘
	fread(hRGB, sizeof(RGBQUAD), 256, fp); // R G B blank 이렇게 4가지를 이용한 256가지 팔레트 제조, 배열 크기
	//256개 있는 배열에서 그 배열의 첫번째 주소를 가리킨다 => 하나만 부르면 팔레트 색깔중 하나만 나오는 거임

	int ImgSize = hinfo.biWidth * hinfo.biHeight;

	//동적할당으로 변수를 시작부분부터 매칭해서 넣어주기
	//빈 바구니같은 변수 만들어주기
	//입력 이미지를 바구니에 담고 다닐 거야
	BYTE* Image = (BYTE*)malloc(ImgSize);
	//처리된 값을 출력 이미지 바구니에 차곡차곡 담을 거야
	BYTE* Output = (BYTE*)malloc(ImgSize);


	fread(Image, sizeof(BYTE), ImgSize, fp);
	fclose(fp);

	


/*작업 하기*/ //-----------------------------------------------------------------------------------------------------------------------------------------------------------

	int Histo[256] = { 0 };
	//함수 호출할 땐 오히려 자료형 생략
	//아래는 불가능하다 배열에 함수 반환값을 직접 대입할 수 없다.
	//Histo = ObtainHistogram(Image, hinfo.biWidth, hinfo.biHeight);
	

	ObtainHistogram(Image, hinfo.biWidth, hinfo.biHeight);
	

/*출력 하기*/ //-----------------------------------------------------------------------------------------------------------------------------------------------------------

	//파일을 가리키는 손가락이 fp라고 생각
	fp = fopen("output_base.bmp", "wb");
	//이제 fp는 새로운 걸 받을 준비가 되었어
	//fwrite( 데이터를 가져올 곳, 데이터 크기, 데이터 갯수, 데이터를 저장할 곳) 
	fwrite(&hf, sizeof(BYTE), sizeof(BITMAPFILEHEADER), fp);
	fwrite(&hinfo, sizeof(BYTE), sizeof(BITMAPINFOHEADER), fp);
	fwrite(hRGB, sizeof(RGBQUAD), 256, fp);
	fwrite(Output, sizeof(BYTE), ImgSize, fp);
	fclose(fp);


	//동적할당 된 건 무*조*건 free 시켜줘야해
	// why? 메모리 누수현상이 발생되기 때문에 -> 계속 사용하고 있다고 생각해서 컴퓨터 뇌에 자리차지함
	free(Image);
	free(Output);

	return 0;

}