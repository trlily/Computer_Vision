#pragma warning(disable:4996) //기존 입력함수에 대해 error를 발생시키는 경우 방지
#include <stdio.h>
#include <stdlib.h> //동적 malloc 때문에
#include <Windows.h> //구조체 사용을 위함


int ObtainHistogram(BYTE* Img, int* Histo, int W, int H, int T_old)
{
	//히스토그램 함수

	int ImgSize = W * H; // 이미지 사이즈
	double Temp[256] = { 0 }; // 0으로 초기화
	for (int i = 0; i < ImgSize; i++) {
		Histo[Img[i]]++; // Img 픽셀별 밝기값 Histo에 누적하기
	}

	for (int i = 0; i < 256; i++)
	{
		{
			//printf("%d %d \n", i, Histo[i]);
		}

	}
	printf("\n\n");

	int Low = 0, High = 255;

	// Low, High 계산 (언제든 필요)
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

	// T_old가 0이면 초기 T 값 리턴 (처음 진입한 경우)
	if (T_old == 0) {
		int T_init = (Low + High) / 2;
		printf("초기 T 설정됨: %d\n", T_init);
		return T_init;
	}



	/*

	if (T_old == 0)
	{
		// 1. 가장 작은 값의 밝기와 가장 큰 값의 밝기를 더하고 2로 나눈값을 T로 잡는다


		//처음 선언 할 때 for문 밖에서 기본 선언해야해

		for (int i = 0; i < 256; i++)
		{

			if (Histo[i] > 0)
			{
				//printf("%d %d \n", i, Histo[i]);
				Low = i;
				//printf("%d \n", Low); // 가장 작은 밝기
				break;
			}


		}

		for (int i = 255; i >= 0; i--)
		{

			if (Histo[i] > 0)
			{
				//printf("%d %d \n", i, Histo[i]);
				High = i;
				//printf("%d", High); //가장 큰 밝기값(크기)
				break;
			}
		}
		printf("\n");


		//printf("%d  %d \n", Low, High);
		int T = (Low + High) / 2;
		//printf("T = %d \n \n", T);
	}*/


	// 2. T값을 기준으로 우측에 있는 밝기값의 합을 G1, 좌측에 있는 밝기값의 합을 G2로 나눈다

	int G1 = 0;
	int G2 = 0;
	int sum1 = 0;
	int sum2 = 0;


	for (int i = Low; i < T_old; i++)
	{
		G1 = G1 + (Histo[i] * i);

		sum1 = sum1 + Histo[i];
		//printf("G1 밝기값의 총 합 : %d    Histo[i] 밝기값에 있는 갯수 : %d   i 밝기값 : %d    총 픽셀 갯수 : %d \n", G1, Histo[i], i, sum1);


	}

	//printf("\n\n ----------------- \n\n");

	for (int i = T_old; i <= High; i++)
	{
		G2 = G2 + (Histo[i] * i);

		sum2 = sum2 + Histo[i];
		//printf("G2 밝기값의 총 합 : %d     Histo[i] 밝기값에 있는 갯수 : %d    i 밝기값 i : %d    sum2 총 픽셀 갯수 : %d\n", G2, Histo[i], i, sum2);
	}



	// 3. G1 영역의 픽셀 밝기값의 전체 평균값을 m1, G2 영역의 픽셀 밝기값의 전체 평균값을 m2로 둔다

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

	// 5. |new T - T| < 정한 오차 

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
	fp = fopen("coin.bmp", "rb"); //읽을 거라서 rb
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
	BYTE* Image = (BYTE*)malloc(ImgSize); //byte type의 바구니에 담을 수 있는 변수
	//-> 형태를 맞추기위해 뒤에도 강제 형변환
//  ==  BYTE Image [ImgSize]; 와 같은 의미이다. 다만 배열의 크기는 변수가 될 수 없어서 동적할당중

	BYTE* Output = (BYTE*)malloc(ImgSize);
	fread(Image, sizeof(BYTE), ImgSize, fp);

	memcpy(Output, Image, ImgSize);  // 단순 복사

	fclose(fp);

	//-------------------------------------- 입력


	int Histo[256] = { 0 };  // ← 여기에 선언!

	int T = 0; // 초기 T는 중간값

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

		T = new_T; // T 갱신이 없으면 무한 루프 발생
	}


	for (int i = 0; i < ImgSize; i++) 
	{
		if (Image[i] < T)
			Output[i] = 0;
		else
			Output[i] = 255;
	}

	//--------------------------------------- 이미지 처리
	
	fp = fopen("GonZalez.bmp", "wb");
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




// GonZalez 알고리즘

// 1. 가장 작은 값의 밝기와 가장 큰 값의 밝기를 더하고 2로 나눈값을 T로 잡는다
// 2. T값을 기준으로 우측에 있는 밝기값의 합을 G1, 좌측에 있는 밝기값의 합을 G2로 한다 (두 파트를 나눈다)
// 3. G1 영역의 픽셀 밝기값의 전체 평균값을 m1, G2 영역의 픽셀 밝기값의 전체 평균값을 m2로 둔다
// 4. new T = (m1 + m2)/2
// 5. |new T - T| < 정한 오차 
// 6_1. 범위 내면 T = new T 하고 끝냄
// 6_2. 범위 밖이면 new T로 반영한 다음 2번으로 다시 돌아가서 반복

