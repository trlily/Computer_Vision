#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
void InverseImage(BYTE* Img, BYTE* Out, int W, int H)
{
	int ImgSize = W * H;
	for (int i = 0; i < ImgSize; i++)
	{
		Out[i] = 255 - Img[i];
	}
}
void BrightnessAdj(BYTE* Img, BYTE* Out, int W, int H, int Val)
{
	int ImgSize = W * H;
	for (int i = 0; i < ImgSize; i++)
	{
		if (Img[i] + Val > 255)
		{
			Out[i] = 255;
		}
		else if (Img[i] + Val < 0)
		{
			Out[i] = 0;
		}
		else 	Out[i] = Img[i] + Val;
	}
}
void ContrastAdj(BYTE* Img, BYTE* Out, int W, int H, double Val)
{
	int ImgSize = W * H;
	for (int i = 0; i < ImgSize; i++)
	{
		if (Img[i] * Val > 255.0)
		{
			Out[i] = 255;
		}
		else 	Out[i] = (BYTE)(Img[i] * Val);
	}
}
void ObtainHistogram(BYTE* Img, int* Histo, int W, int H)
{
	int ImgSize = W * H;
	for (int i = 0; i < ImgSize; i++) {
		Histo[Img[i]]++;
	}
}
void ObtainAHistogram(int* Histo, int* AHisto)
{
	for (int i = 0; i < 256; i++) {
		for (int j = 0; j <= i; j++) {
			AHisto[i] += Histo[j];
		}
	}
}

void HistogramStretching(BYTE* Img, BYTE* Out, int* Histo, int W, int H)
{
	int ImgSize = W * H;
	BYTE Low, High;
	for (int i = 0; i < 256; i++) {
		if (Histo[i] != 0) {
			Low = i;
			break;
		}
	}
	for (int i = 255; i >= 0; i--) {
		if (Histo[i] != 0) {
			High = i;
			break;
		}
	}
	for (int i = 0; i < ImgSize; i++) {
		Out[i] = (BYTE)((Img[i] - Low) / (double)(High - Low) * 255.0);
	}
}

void HistogramEqualization(BYTE* Img, BYTE* Out, int* AHisto, int W, int H)
{
	int ImgSize = W * H;
	int Nt = W * H, Gmax = 255;
	double Ratio = Gmax / (double)Nt;
	BYTE NormSum[256];
	for (int i = 0; i < 256; i++) {
		NormSum[i] = (BYTE)(Ratio * AHisto[i]);
	}
	for (int i = 0; i < ImgSize; i++)
	{
		Out[i] = NormSum[Img[i]];
	}
}

void Binarization(BYTE* Img, BYTE* Out, int W, int H, BYTE Threshold)
{
	int ImgSize = W * H;
	for (int i = 0; i < ImgSize; i++) {
		if (Img[i] < Threshold) Out[i] = 0;
		else Out[i] = 255;
	}
}

int gonzalez(int* histo, int w, int h)
{

	//최소 최대 구하기
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
			sum1 = sum1 + (histo[i] * i); // 밝기값*갯수
			cnt1 = cnt1 + histo[i]; //갯수 세기
		}

		m1 = sum1 / cnt1;



		for (int i = T; i < 256; i++)
		{
			sum2 = sum2 + (histo[i] * i); // 밝기값*갯수
			cnt2 = cnt2 + histo[i]; //갯수 세기
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

void AverageConv(BYTE* Img, BYTE* Out, int W, int H) // 박스평활화
{
	double Kernel[3][3] = { 0.11111, 0.11111, 0.11111,
										0.11111, 0.11111, 0.11111,
										0.11111, 0.11111, 0.11111 };
	double SumProduct = 0.0;
	for (int i = 1; i < H - 1; i++) { // Y좌표 (행)
		for (int j = 1; j < W - 1; j++) { // X좌표 (열)
			for (int m = -1; m <= 1; m++) { // Kernel 행
				for (int n = -1; n <= 1; n++) { // Kernel 열
					SumProduct += Img[(i + m) * W + (j + n)] * Kernel[m + 1][n + 1];
				}
			}
			Out[i * W + j] = (BYTE)SumProduct;
			SumProduct = 0.0;
		}
	}
}

void GaussAvrConv(BYTE* Img, BYTE* Out, int W, int H) // 가우시안평활화
{
	double Kernel[3][3] = { 0.0625, 0.125, 0.0625,
										0.125, 0.25, 0.125,
										0.0625, 0.125, 0.0625 };
	double SumProduct = 0.0;
	for (int i = 1; i < H - 1; i++) { // Y좌표 (행)
		for (int j = 1; j < W - 1; j++) { // X좌표 (열)
			for (int m = -1; m <= 1; m++) { // Kernel 행
				for (int n = -1; n <= 1; n++) { // Kernel 열
					SumProduct += Img[(i + m) * W + (j + n)] * Kernel[m + 1][n + 1];
				}
			}
			Out[i * W + j] = (BYTE)SumProduct;
			SumProduct = 0.0;
		}
	}
}

void Prewitt_X_Conv(BYTE* Img, BYTE* Out, int W, int H) // Prewitt 마스크 X
{
	double Kernel[3][3] = { -1.0, 0.0, 1.0,
										-1.0, 0.0, 1.0,
										-1.0, 0.0, 1.0 };
	double SumProduct = 0.0;
	for (int i = 1; i < H - 1; i++) { // Y좌표 (행)
		for (int j = 1; j < W - 1; j++) { // X좌표 (열)
			for (int m = -1; m <= 1; m++) { // Kernel 행
				for (int n = -1; n <= 1; n++) { // Kernel 열
					SumProduct += Img[(i + m) * W + (j + n)] * Kernel[m + 1][n + 1];
				}
			}
			// 0 ~ 765  =====> 0 ~ 255
			Out[i * W + j] = abs((long)SumProduct) / 3;
			SumProduct = 0.0;
		}
	}
}

void Prewitt_Y_Conv(BYTE* Img, BYTE* Out, int W, int H) // Prewitt 마스크 X
{
	double Kernel[3][3] = { -1.0, -1.0, -1.0,
										0.0, 0.0, 0.0,
										1.0, 1.0, 1.0 };
	double SumProduct = 0.0;
	for (int i = 1; i < H - 1; i++) { // Y좌표 (행)
		for (int j = 1; j < W - 1; j++) { // X좌표 (열)
			for (int m = -1; m <= 1; m++) { // Kernel 행
				for (int n = -1; n <= 1; n++) { // Kernel 열
					SumProduct += Img[(i + m) * W + (j + n)] * Kernel[m + 1][n + 1];
				}
			}
			// 0 ~ 765  =====> 0 ~ 255
			Out[i * W + j] = abs((long)SumProduct) / 3;
			SumProduct = 0.0;
		}
	}
}

void Sobel_X_Conv(BYTE* Img, BYTE* Out, int W, int H) // Prewitt 마스크 X
{
	double Kernel[3][3] = { -1.0, 0.0, 1.0,
										-2.0, 0.0, 2.0,
										-1.0, 0.0, 1.0 };
	double SumProduct = 0.0;
	for (int i = 1; i < H - 1; i++) { // Y좌표 (행)
		for (int j = 1; j < W - 1; j++) { // X좌표 (열)
			for (int m = -1; m <= 1; m++) { // Kernel 행
				for (int n = -1; n <= 1; n++) { // Kernel 열
					SumProduct += Img[(i + m) * W + (j + n)] * Kernel[m + 1][n + 1];
				}
			}
			// 0 ~ 1020  =====> 0 ~ 255
			Out[i * W + j] = abs((long)SumProduct) / 4;
			SumProduct = 0.0;
		}
	}
}

void Sobel_Y_Conv(BYTE* Img, BYTE* Out, int W, int H) // Prewitt 마스크 X
{
	double Kernel[3][3] = { -1.0, -2.0, -1.0,
										0.0, 0.0, 0.0,
										1.0, 2.0, 1.0 };
	double SumProduct = 0.0;
	for (int i = 1; i < H - 1; i++) { // Y좌표 (행)
		for (int j = 1; j < W - 1; j++) { // X좌표 (열)
			for (int m = -1; m <= 1; m++) { // Kernel 행
				for (int n = -1; n <= 1; n++) { // Kernel 열
					SumProduct += Img[(i + m) * W + (j + n)] * Kernel[m + 1][n + 1];
				}
			}
			// 0 ~ 765  =====> 0 ~ 255
			Out[i * W + j] = abs((long)SumProduct) / 4;
			SumProduct = 0.0;
		}
	}
}

void Laplace_Conv(BYTE* Img, BYTE* Out, int W, int H) // Prewitt 마스크 X
{
	double Kernel[3][3] = { -1.0, -1.0, -1.0,
										-1.0, 8.0, -1.0,
										-1.0, -1.0, -1.0 };
	double SumProduct = 0.0;
	for (int i = 1; i < H - 1; i++) { // Y좌표 (행)
		for (int j = 1; j < W - 1; j++) { // X좌표 (열)
			for (int m = -1; m <= 1; m++) { // Kernel 행
				for (int n = -1; n <= 1; n++) { // Kernel 열
					SumProduct += Img[(i + m) * W + (j + n)] * Kernel[m + 1][n + 1];
				}
			}
			// 0 ~ 2040  =====> 0 ~ 255
			Out[i * W + j] = abs((long)SumProduct) / 8;
			SumProduct = 0.0;
		}
	}
}

void Laplace_Conv_DC(BYTE* Img, BYTE* Out, int W, int H) // Prewitt 마스크 X
{
	double Kernel[3][3] = { -1.0, -1.0, -1.0,
										-1.0, 9.0, -1.0,
										-1.0, -1.0, -1.0 };
	double SumProduct = 0.0;
	for (int i = 1; i < H - 1; i++) { // Y좌표 (행)
		for (int j = 1; j < W - 1; j++) { // X좌표 (열)
			for (int m = -1; m <= 1; m++) { // Kernel 행
				for (int n = -1; n <= 1; n++) { // Kernel 열
					SumProduct += Img[(i + m) * W + (j + n)] * Kernel[m + 1][n + 1];
				}
			}
			//Out[i * W + j] = abs((long)SumProduct) / 8;
			if (SumProduct > 255.0) Out[i * W + j] = 255;
			else if (SumProduct < 0.0) Out[i * W + j] = 0;
			else Out[i * W + j] = (BYTE)SumProduct;
			SumProduct = 0.0;
		}
	}
}

void SaveBMPFile(BITMAPFILEHEADER hf, BITMAPINFOHEADER hInfo,
	RGBQUAD* hRGB, BYTE* Output, int W, int H, const char* FileName)
{
	FILE* fp = fopen(FileName, "wb");
	fwrite(&hf, sizeof(BYTE), sizeof(BITMAPFILEHEADER), fp);
	fwrite(&hInfo, sizeof(BYTE), sizeof(BITMAPINFOHEADER), fp);
	fwrite(hRGB, sizeof(RGBQUAD), 256, fp);
	fwrite(Output, sizeof(BYTE), W * H, fp);
	fclose(fp);
}

void swap(BYTE* a, BYTE* b)
{
	BYTE temp = *a;
	*a = *b;
	*b = temp;
}

BYTE Median(BYTE* arr, int size)
{
	// 오름차순 정렬
	const int S = size;
	for (int i = 0; i < size - 1; i++) // pivot index
	{
		for (int j = i + 1; j < size; j++) // 비교대상 index
		{
			if (arr[i] > arr[j]) 	swap(&arr[i], &arr[j]);
		}
	}
	return arr[S / 2];
}

BYTE MaxPooling(BYTE* arr, int size)
{
	// 오름차순 정렬
	const int S = size;
	for (int i = 0; i < size - 1; i++) // pivot index
	{
		for (int j = i + 1; j < size; j++) // 비교대상 index
		{
			if (arr[i] > arr[j]) 	swap(&arr[i], &arr[j]);
		}
	}
	return arr[S - 1];
}

BYTE MinPooling(BYTE* arr, int size)
{
	// 오름차순 정렬
	const int S = size;
	for (int i = 0; i < size - 1; i++) // pivot index
	{
		for (int j = i + 1; j < size; j++) // 비교대상 index
		{
			if (arr[i] > arr[j]) 	swap(&arr[i], &arr[j]);
		}
	}
	return arr[0];
}

int push(short* stackx, short* stacky, int arr_size, short vx, short vy, int* top)
{
	if (*top >= arr_size) return(-1);
	(*top)++;
	stackx[*top] = vx;
	stacky[*top] = vy;
	return(1);
}

int pop(short* stackx, short* stacky, short* vx, short* vy, int* top)
{
	if (*top == 0) return(-1);
	*vx = stackx[*top];
	*vy = stacky[*top];
	(*top)--;
	return(1);
}
// GlassFire 알고리즘을 이용한 라벨링 함수
void m_BlobColoring(BYTE* CutImage, int height, int width)
{
	int i, j, m, n, top, area, Out_Area, index, BlobArea[1000];
	long k;
	short curColor = 0, r, c;
	//	BYTE** CutImage2;
	Out_Area = 1;

	// 스택으로 사용할 메모리 할당
	short* stackx = new short[height * width];
	short* stacky = new short[height * width];
	short* coloring = new short[height * width];

	int arr_size = height * width;

	// 라벨링된 픽셀을 저장하기 위해 메모리 할당

	for (k = 0; k < height * width; k++) coloring[k] = 0;  // 메모리 초기화

	for (i = 0; i < height; i++)
	{
		index = i * width;
		for (j = 0; j < width; j++)
		{
			// 이미 방문한 점이거나 픽셀값이 255가 아니라면 처리 안함
			if (coloring[index + j] != 0 || CutImage[index + j] != 255) continue;
			r = i; c = j; top = 0; area = 1;
			curColor++;

			while (1)
			{
			GRASSFIRE:
				for (m = r - 1; m <= r + 1; m++)
				{
					index = m * width;
					for (n = c - 1; n <= c + 1; n++)
					{
						//관심 픽셀이 영상경계를 벗어나면 처리 안함
						if (m < 0 || m >= height || n < 0 || n >= width) continue;

						if ((int)CutImage[index + n] == 255 && coloring[index + n] == 0)
						{
							coloring[index + n] = curColor; // 현재 라벨로 마크
							if (push(stackx, stacky, arr_size, (short)m, (short)n, &top) == -1) continue;
							r = m; c = n; area++;
							goto GRASSFIRE;
						}
					}
				}
				if (pop(stackx, stacky, &r, &c, &top) == -1) break;
			}
			if (curColor < 1000) BlobArea[curColor] = area;
		}
	}

	float grayGap = 255.0f / (float)curColor;


	//Goal
	// 가장 면적이 넓은 영역을 찾아내기 위함 
	for (i = 1; i <= curColor; i++)
	{
		if (BlobArea[i] >= BlobArea[Out_Area]) Out_Area = i;
	}
	// CutImage 배열 클리어~
	for (k = 0; k < width * height; k++) CutImage[k] = 255;

	// coloring에 저장된 라벨링 결과중 (Out_Area에 저장된) 영역이 가장 큰 것만 CutImage에 저장
	for (k = 0; k < width * height; k++)
	{
		if (coloring[k] == Out_Area) CutImage[k] = 0;  // 가장 큰 것만 저장 (size filtering)
		//if (BlobArea[coloring[k]] > 500) CutImage[k] = 0;  // 특정 면적이상되는 영역만 출력
		//CutImage[k] = (unsigned char)(coloring[k] * grayGap); //라벨링된 결과 출력
	}

	delete[] coloring;
	delete[] stackx;
	delete[] stacky;
}
// 라벨링 후 가장 넓은 영역에 대해서만 뽑아내는 코드 포함
// 영상 경계 검출
void BinaryImgaeEdgeDetection(BYTE* Bin, BYTE* Out, int W, int H)
{
	//경계검출
	for (int i = 0; i < H; i++)
	{
		for (int j = 0; j < W; j++)
		{
			//전경화소인지 검출
			if (Bin[i * W + j] == 0)
			{
				if (!(Bin[(i - 1) * W + j] == 0 &&
					Bin[(i + 1) * W + j] == 0 &&
					Bin[i * W + (j - 1)] == 0 &&
					Bin[i * W + (j + 1)] == 0))

					Out[i * W + j] = 255;
				{

				}
			}
		}
	}
}


void DrawRectOutline(BYTE* Img, int W, int H, int LU_X, int LU_Y, int RD_X, int RD_Y)
{
	//위_가로축 그리기
	for (int i = LU_X; i < RD_X; i++)
	{
		Img[LU_Y * W + i] = 255;
	}
	//아래_가로축 그리기
	for (int i = LU_X; i < RD_X; i++)
	{
		Img[RD_Y * W + i] = 255;
	}
	//좌_세로축 그리기
	for (int i = LU_Y; i < RD_Y; i++)
	{
		Img[i * W + LU_X] = 255;
	}
	//우_세로축 그리기
	for (int i = LU_Y; i < RD_Y; i++)
	{
		Img[i * W + RD_X] = 255;
	}
}

// Img: 사각형을 그릴 이미지배열, W: 영상 가로사이즈, H: 영상 세로사이즈,

// LU_X: 사각형의 좌측상단 X좌표, LU_Y: 사각형의 좌측상단 Y좌표,

// RD_X: 사각형의 우측하단 X좌표, LU_Y: 사각형의 우측하단 Y좌표.

void DrawCrossLine(BYTE* Img, int W, int H, int Cx, int Cy)
{
	for (int i = 0; i < H; i++)
	{
		//x의 값이 동일한 것
		Img[i*W+Cx] = 255;
	}
	for (int j = 0; j < W; j++)
	{
		Img[Cy * W + j] = 255;
	} 

}


void Obtain2Dcenter(BYTE* Image, int* Cx, int* Cy, int W, int H)
{
	//저장해서 줘야하니까 주소를 보내야해

	//무게중심 구하기
	// --> 검정색 화소의 무게중심
	// --> x좌표와 y좌표를 누적해서 더한 후 검정색 화소의 갯수만큼 (픽셀만큼) 나눔

	int SumX = 0;
	int SumY = 0;

	int cnt = 0; // 몇개의 픽셀


	for (int i = 0; i < H; i++)
	{
		for (int j = 0; j < W; j++)
		{
			//동공영역이면
			if (Image[i * W + j] == 0)
			{
				//x좌표가 j (W or 열)
				SumX = SumX + j;
				SumY = SumY + i;
				cnt++;
			}
		}
	}
	//나누는 수가 0이면 에러가 발생!!!!
	//동공영역이 없는 경우 cnt가 0이므로 처리 필요
	if (cnt == 0)
	{
		cnt = 1; // 편법적으로
	}
	* Cx = SumX / cnt;
	* Cy = SumY / cnt; // 역참조 필요---------------------------------어려워 이거 :(

	//위에서 주소로 받았으니까 주소값을 바꾸는 게 아니라 주소에 있는 값을 지정해야하기때문에 *로 역참조 한 건가?
}


// Img: 가로/세로 라인을 그릴 이미지배열, W: 영상 가로사이즈, H: 영상 세로사이즈,

// Cx: 가로/세로 라인이 교차되는 지점의 X좌표

// Cy: 가로/세로 라인이 교차되는 지점의 Y좌표


void Obtain2DBoundingBox(BYTE* Image, int W, int H, int* LUX, int* LUY, int* RDX, int* RDY) 
{

	int flag = 0;

	//좌측 상단 y 구하기
	for (int i = 0; i < H; i++)
	{
		for (int j = 0; j < W; j++)
		{
			if (Image[i * W + j] == 0)
			{
				* LUY = i;
				flag = 1;
			}
		}
		if (flag == 1) break;
	}


	//좌측상단 x 구하기 -- 세로로 그어져야해
	flag = 0;
	for (int j = 0; j < W; j++)
	{
		for (int i = 0; i < H; i++)
		{
			if (Image[i * W + j] == 0)
			{
				* LUX = j;
				flag = 1;
			}
		}
		if (flag == 1) break;
	}

	//우측 하단 y 구하기
	flag = 0;
	for (int i = H - 1; i >= 0; i--)
	{
		for (int j = W - 1; j >= 0; j--)
		{
			if (Image[i * W + j] == 0)
			{
				*RDY = i;
				flag = 1;
			}
		}
		if (flag == 1) break;
	}


	//우측 하단 x 구하기
	flag = 0;
	for (int j = W - 1; j >= 0; j--)
	{
		for (int i = H - 1; i >= 0; i--)
		{
			if (Image [i * W + j] == 0)
			{
				* RDX = j;
				flag = 1;
			}
		}
		if (flag == 1) break;
	}

	printf("%d %d %d %d \n", LUX, LUY, RDX, RDY);

}



int main()
{
	BITMAPFILEHEADER hf; // 14바이트
	BITMAPINFOHEADER hInfo; // 40바이트
	RGBQUAD hRGB[256]; // 1024바이트
	FILE* fp;
	fp = fopen("pupil1.bmp", "rb");
	if (fp == NULL) {
		printf("File not found!\n");
		return -1;
	}

	fread(&hf, sizeof(BITMAPFILEHEADER), 1, fp);
	fread(&hInfo, sizeof(BITMAPINFOHEADER), 1, fp);
	fread(hRGB, sizeof(RGBQUAD), 256, fp);
	int ImgSize = hInfo.biWidth * hInfo.biHeight;
	BYTE* Image = (BYTE*)malloc(ImgSize);
	BYTE* Temp = (BYTE*)malloc(ImgSize); // 임시배열
	BYTE* Output = (BYTE*)malloc(ImgSize);
	fread(Image, sizeof(BYTE), ImgSize, fp);
	fclose(fp);


	int H = hInfo.biHeight;
	int W = hInfo.biWidth;



	int Histo[256] = { 0 };
	int AHisto[256] = { 0 };

	/*ObtainHistogram(Image, Histo, hInfo.biWidth, hInfo.biHeight);
	ObtainAHistogram(Histo, AHisto);
	HistogramEqualization(Image, Output, AHisto, hInfo.biWidth, hInfo.biHeight);*/
	/*int Thres = GozalezBinThresh();
	Binarization(Image, Output, hInfo.biWidth, hInfo.biHeight, Thres);*/

	//GaussAvrConv(Image, Output, hInfo.biWidth, hInfo.biHeight);

	/*Sobel_X_Conv(Image, Temp, hInfo.biWidth, hInfo.biHeight);
	Sobel_Y_Conv(Image, Output, hInfo.biWidth, hInfo.biHeight);
	for (int i = 0; i < ImgSize; i++) {
		if (Temp[i] > Output[i]) 	Output[i] = Temp[i];
	}
	Binarization(Output, Output, hInfo.biWidth, hInfo.biHeight, 40);*/
	/*GaussAvrConv(Image, Temp, hInfo.biWidth, hInfo.biHeight);
	Laplace_Conv_DC(Temp, Output, hInfo.biWidth, hInfo.biHeight);*/

	//HistogramStretching(Image, Output, Histo, hInfo.biWidth, hInfo.biHeight);
	//InverseImage(Image, Output, hInfo.biWidth, hInfo.biHeight);
	//BrightnessAdj(Image, Output, hInfo.biWidth, hInfo.biHeight, 70);
	//ContrastAdj(Image, Output, hInfo.biWidth, hInfo.biHeight, 0.5);

	/* Median filtering */
	//BYTE temp[9];
	//int W = hInfo.biWidth, H = hInfo.biHeight;
	//int i, j;
	//for (i = 1; i < H - 1; i++) {
	//	for (j = 1; j < W - 1; j++) {
	//		temp[0] = Image[(i - 1) * W + j-1];
	//		temp[1] = Image[(i - 1) * W + j];
	//		temp[2] = Image[(i - 1) * W + j+1];
	//		temp[3] = Image[i * W + j-1];
	//		temp[4] = Image[i * W + j];
	//		temp[5] = Image[i * W + j+1];
	//		temp[6] = Image[(i + 1) * W + j-1];
	//		temp[7] = Image[(i + 1) * W + j];
	//		temp[8] = Image[(i + 1) * W + j+1];
	//		Output[i * W + j] = Median(temp, 9);
	//		//Output[i * W + j] = MaxPooling(temp, 9);
	//		//Output[i * W + j] = MinPooling(temp, 9);
	//	}
	//}
	/* Median filtering */

	//이진화
	//Binarization(Image, Temp, hInfo.biWidth, hInfo.biHeight, 40);

	//코인과 전경과 배경이 반대라서 inverse 취해줘
	//영상 반전
	//InverseImage(Temp, Temp, W, H);

	//이제 라벨링을 할 것
	//붙어있는 것마다 덩어리진 컨퍼넌트에 대해 같은 라벨
	//하나의 픽셀이라도 떨어진 건 다른 컨퍼넌트로 지정된다.


	//몇개의 컴포넌트로 나눠질까?
	
	/*
	//초기화 (배경 깔끔 or 원본 등등)
	for (int i = 0; i < ImgSize; i++)
	{
		Output[i] = Image[i];
	}
	*/

	//BinaryImgaeEdgeDetection(Temp, Output, W, H);



	// 7주차
	// 1. 동공영역 검출
	// 2. 무게중심 구하기
	// 3. 십자가 그리기
	
	Binarization(Image, Output, W, H, 30);
	InverseImage(Output, Output, W, H);
	
	//라벨링 (여기서 라벨링시 0을 배경이라고 생각하고 해)
	m_BlobColoring(Output, H,W);

	//printf("Cx : %d, Cy : %d", Cx, Cy);

	SaveBMPFile(hf, hInfo, hRGB, Output, hInfo.biWidth, hInfo.biHeight, "before.bmp");




	int Cx, Cy;

	//이진영상 무게중심 구하기
	Obtain2Dcenter(Output, &Cx, &Cy, W, H);
	// 주소 넘기기... 주소 넘기기 너무 어렵다----------------------------------------------


	DrawCrossLine(Image, W, H, Cx, Cy);


	//바운딩박스 구하기
	int LUX, LUY, RDX, RDY;

	Obtain2DBoundingBox(Output, W, H, &LUX, &LUY, &RDX, &RDY);


	//사각형 그리는 함수
	DrawRectOutline(Image, W, H, LUX, LUY, RDX, RDY);


	



	//AverageConv(Image, Output, hInfo.biWidth, hInfo.biHeight);

	SaveBMPFile(hf, hInfo, hRGB, Image, hInfo.biWidth, hInfo.biHeight, "center_of_gravity.bmp");
	

	free(Image);
	free(Output);
	free(Temp);
	return 0;
}