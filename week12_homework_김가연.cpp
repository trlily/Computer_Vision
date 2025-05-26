#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

//이차원 배열 동적할당
unsigned char** imageMatrix;
//이진영상에서
unsigned char blankPixel = 255, imagePixel = 0;

typedef struct
{
	int row, col;
}pixel;


int getBlackNeighbours(int row, int col) {

	int i, j, sum = 0;

	for (i = -1; i <= 1; i++) {
		for (j = -1; j <= 1; j++) {
			if (i != 0 || j != 0)
				sum += (imageMatrix[row + i][col + j] == imagePixel);
		}
	}

	return sum;
}

int getBWTransitions(int row, int col) {
	return 	((imageMatrix[row - 1][col] == blankPixel && imageMatrix[row - 1][col + 1] == imagePixel)
		+ (imageMatrix[row - 1][col + 1] == blankPixel && imageMatrix[row][col + 1] == imagePixel)
		+ (imageMatrix[row][col + 1] == blankPixel && imageMatrix[row + 1][col + 1] == imagePixel)
		+ (imageMatrix[row + 1][col + 1] == blankPixel && imageMatrix[row + 1][col] == imagePixel)
		+ (imageMatrix[row + 1][col] == blankPixel && imageMatrix[row + 1][col - 1] == imagePixel)
		+ (imageMatrix[row + 1][col - 1] == blankPixel && imageMatrix[row][col - 1] == imagePixel)
		+ (imageMatrix[row][col - 1] == blankPixel && imageMatrix[row - 1][col - 1] == imagePixel)
		+ (imageMatrix[row - 1][col - 1] == blankPixel && imageMatrix[row - 1][col] == imagePixel));
}

//zhangSuen => thining을 할 수 있는 함수
int zhangSuenTest1(int row, int col) {
	int neighbours = getBlackNeighbours(row, col);

	return ((neighbours >= 2 && neighbours <= 6)
		&& (getBWTransitions(row, col) == 1)
		&& (imageMatrix[row - 1][col] == blankPixel || imageMatrix[row][col + 1] == blankPixel || imageMatrix[row + 1][col] == blankPixel)
		&& (imageMatrix[row][col + 1] == blankPixel || imageMatrix[row + 1][col] == blankPixel || imageMatrix[row][col - 1] == blankPixel));
}

int zhangSuenTest2(int row, int col) {
	int neighbours = getBlackNeighbours(row, col);

	return ((neighbours >= 2 && neighbours <= 6)
		&& (getBWTransitions(row, col) == 1)
		&& (imageMatrix[row - 1][col] == blankPixel || imageMatrix[row][col + 1] == blankPixel || imageMatrix[row][col - 1] == blankPixel)
		&& (imageMatrix[row - 1][col] == blankPixel || imageMatrix[row + 1][col] == blankPixel || imageMatrix[row][col + 1] == blankPixel));
}

void zhangSuen(unsigned char* image, unsigned char* output, int rows, int cols) {

	int startRow = 1, startCol = 1, endRow, endCol, i, j, count, processed;

	pixel* markers;

	imageMatrix = (unsigned char**)malloc(rows * sizeof(unsigned char*));

	for (i = 0; i < rows; i++) {
		imageMatrix[i] = (unsigned char*)malloc((cols + 1) * sizeof(unsigned char));
		for (int k = 0; k < cols; k++) imageMatrix[i][k] = image[i * cols + k];
	}

	endRow = rows - 2;
	endCol = cols - 2;
	do {
		markers = (pixel*)malloc((endRow - startRow + 1) * (endCol - startCol + 1) * sizeof(pixel));
		count = 0;

		for (i = startRow; i <= endRow; i++) {
			for (j = startCol; j <= endCol; j++) {
				if (imageMatrix[i][j] == imagePixel && zhangSuenTest1(i, j) == 1) {
					markers[count].row = i;
					markers[count].col = j;
					count++;
				}
			}
		}

		processed = (count > 0);

		for (i = 0; i < count; i++) {
			imageMatrix[markers[i].row][markers[i].col] = blankPixel;
		}

		free(markers);
		markers = (pixel*)malloc((endRow - startRow + 1) * (endCol - startCol + 1) * sizeof(pixel));
		count = 0;

		for (i = startRow; i <= endRow; i++) {
			for (j = startCol; j <= endCol; j++) {
				if (imageMatrix[i][j] == imagePixel && zhangSuenTest2(i, j) == 1) {
					markers[count].row = i;
					markers[count].col = j;
					count++;
				}
			}
		}

		if (processed == 0)
			processed = (count > 0);

		for (i = 0; i < count; i++) {
			imageMatrix[markers[i].row][markers[i].col] = blankPixel;
		}

		free(markers);
	} while (processed == 1);


	for (i = 0; i < rows; i++) {
		for (j = 0; j < cols; j++) {
			output[i * cols + j] = imageMatrix[i][j];
		}
	}
}

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

//int GozalezBinThresh()
//{
//
//}

void FeatureExtractThinImage(BYTE* Image, BYTE* Output, int W, int H) {
	//분기점 및 끝점 표시

	for (int i = 1; i < H - 1; i++)
	{
		for (int j = 1; j < W - 1; j++)
		{
			if (Image[i * W + j] == 0)
			{
				int count = 0;

				for (int ii = -1; ii <= 1; ii++)
				{
					for (int jj = -1; jj <= 1; jj++)
					{

						if (ii == 0 && jj == 0) continue; //주변 픽셀이 아니라 픽셀 자기 자신

						if (Image[(i + ii) * W + (j + jj)] == 0)
						{
							count++;
						}


					}
				}

				// 회색으로 둘러싸기
				if (count == 1 || count >= 3)
				{

					for (int ii = -1; ii <= 1; ii++)
					{
						for (int jj = -1; jj <= 1; jj++)
						{
							int y = i + ii, x = j + jj;
							if (y >= 0 && y < H && x >= 0 && x < W)
								Output[y * W + x] = 128; // 회색으로 표시
						}
					}
				}
				else
				{
					Output[i * W + j] = 0;
				}
			}
			else
			{
				Output[i * W + j] = 255; // 배경은 흰색
			}
		}
	}
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

	if (hInfo.biBitCount == 24)
	{
		fwrite(&hf, sizeof(BYTE), sizeof(BITMAPFILEHEADER), fp);
		fwrite(&hInfo, sizeof(BYTE), sizeof(BITMAPINFOHEADER), fp);
		//fwrite(hRGB, sizeof(RGBQUAD), 256, fp);
		fwrite(Output, sizeof(BYTE), W * H * 3, fp);
	}

	else if (hInfo.biBitCount == 8)
	{
		fwrite(&hf, sizeof(BYTE), sizeof(BITMAPFILEHEADER), fp);
		fwrite(&hInfo, sizeof(BYTE), sizeof(BITMAPINFOHEADER), fp);
		fwrite(hRGB, sizeof(RGBQUAD), 256, fp);
		fwrite(Output, sizeof(BYTE), W * H, fp);
	}

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


/*
7주차 입니다!

라벨링을 배워볼게요

라벨링이란?
영상을 이진화하고
이진화된 영상을 가지고
해당 영상의 전경에 해당되는 영역들을
덩어리진 단위로 (컨포넌트 == 묶음) 아이덴티티 부여
*/

/*
라벨링만 하는게 아니라 가장 큰 컴퍼넌트만 뽑아서 출력하는 동작이야
*/


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
// 이게 본 코드인데 여기서 push랑 pop을 사용합니다
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

	for (k = 0; k < height * width; k++)
	{
		coloring[k] = 0;
	}
	// 메모리 초기화

	for (i = 0; i < height; i++)
	{
		index = i * width; // 별 의미 없음 귀찮아서 붙인 거


		for (j = 0; j < width; j++)
		{
			// 이미 방문한 점이거나 픽셀값이 255가 아니라면 처리 안함
			if (coloring[index + j] != 0 || CutImage[index + j] != 255) continue;
			//while문 안에서 continue --> 그 다음 문장은 시행하지 않고 다음 반복(조건)으로 넘어가!

			r = i; c = j; top = 0; area = 1;
			curColor++;

			while (1)
			{
			GRASSFIRE:
				//8방향 픽셀 확인
				// 관심 픽셀만 한 번 확인해볼게!
				for (m = r - 1; m <= r + 1; m++)
				{
					index = m * width;
					for (n = c - 1; n <= c + 1; n++)
					{
						//관심 픽셀이 영상경계를 벗어나면 처리 안함
						if (m < 0 || m >= height || n < 0 || n >= width) continue;

						/*if ((int)CutImage[index + n] == 255 && coloring[index + n] == 0) {
							coloring[index + n] = curColor;
							해당 픽셀이 객체(255)이고 아직 방문 전이면

								현재 라벨 번호(curColor)로 마킹
						*/

						//아직 맵핑 안 된 거면(coloring에 해당 인덱스 값이 0으로 초기화되어있으면), 그리고 255가 지금 주변 화소인거야 주변 화소는 흰색이여야지!
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
	//curColor : 총 컨퍼런트의 갯수 -> 얼만큼의 값으로 다음 라벨링값의 밝기를 증가시킬 건가

	// 가장 면적이 넓은 영역을 찾아내기 위함 
	// 조사식을 이용하여 어떤 값을 갖는지 알 수 있어!
	for (i = 1; i <= curColor; i++)
	{
		if (BlobArea[i] >= BlobArea[Out_Area]) Out_Area = i;
		//가장 넓은 값을 찾을 것 (크기가 큰 것)
		// out_area는 기본값이 1이다 
		// 다음 인덱스 부분이 더 크다? 그럼 업데이트
		// 그래서 최종적으로는 가장 넓은 것으로 저장되어 있는 걸 저장하게 돼
		//각 컴퍼넌트별 특징 ex) 어느정도 픽셀 크기를 가지는지,얼만큼 묶음이 있는지 (컴포넌트 갯수)
	}

	// CutImage 배열 클리어~
	// 여기 cutImage가 main함수에서 output, 그러니까 입력으로 넣은 거임
	// 입력으로 넣은 파일을 출력으로도 뽑을 예정

	for (k = 0; k < width * height; k++)
	{
		CutImage[k] = 255;
	}

	// coloring에 저장된 라벨링 결과중 (Out_Area에 저장된) 영역이 가장 큰 것만 CutImage에 저장
	for (k = 0; k < width * height; k++)
	{
		//해당 값만 채우겠다 (검정색으로)
		if (coloring[k] == Out_Area) CutImage[k] = 0;  // 가장 큰 것만 저장 (= 'size filtering 한다')

		//if (BlobArea[coloring[k]] > 500) CutImage[k] = 0;  // 특정 면적이상되는 영역만 출력
		//CutImage[k] = (unsigned char)(coloring[k] * grayGap); //라벨링된 결과 그대로 출력
	}

	delete[] coloring;
	delete[] stackx;
	delete[] stacky;
}
// 라벨링 후 가장 넓은 영역에 대해서만 뽑아내는 코드 포함


void BinaryImageEdgeDetection(BYTE* Bin, BYTE* Out, int W, int H)

{

	// 경계 검출 코드
	for (int i = 0; i < W; i++)
	{
		Out[i] = Bin[i];
	}

	for (int i = 0; i < H; i++)
	{
		for (int j = 0; j < W; j++)
		{
			//경계 찾기

			//전경화소라면
			if (Bin[i * W + j] == 0)
			{
				//주변이 모두 0이면 아닌 건가? 아 맞다 하나라도 있으면 경계값이니까
				if (!(Bin[(i - 1) * W + j] == 0 && Bin[(i + 1) * W + j] == 0 && Bin[i * W + j - 1] == 0 && Bin[i * W + (j + 1)] == 0))
				{
					// not을 붙였으니까 4방향중 하나라도 전경이 아닌게 있따면!
					// ==>> 우리가 찾고 있는 거!!
					Out[i * W + j] = 255;
				}
			}
		}
	}

}

void FillColor(BYTE* Image, int X, int Y, int W, int H, BYTE R, BYTE G, BYTE B)
{
	//ex (50,40)위치를 특정 색상으로 지정해보기

	//영상에 들어간 건 BGR순서임!!
	Image[Y * W * 3 + X * 3] = B; //BLUE 성분
	Image[Y * W * 3 + X * 3 + 1] = G; //GREEN
	Image[Y * W * 3 + X * 3 + 2] = R; //RED
	//3개의 값이 같으면 gray scale 값! -> 무채색!
}

void Erosion(BYTE* Image, BYTE* Output, int W, int H)
{
	//모폴로지 연산은 구조 요소이다
	//구조 요소란? => ppt
	for (int i = 1; i < H - 1; i++)
	{
		for (int j = 1; j < W - 1; j++)
		{
			//흰색이 전경이고 블랙이 배경
			if (Image[i * W + j] == 255)
			{
				//전경화소처리 => 전경화소라면
				//주변을 확인해서(구조요소 체크) 하나라도 배경이면 배경으로 지정
				//모두가 전경화소인지 확인
				//하나라도 전경 화소가 아니면 Output의 i*W+j를 0 즉 배경으로 바꿔줘야해, 아니면 255
				if (!(Image[(i - 1) * W + j] == 255 &&
					Image[(i + 1) * W + j] == 255 &&
					Image[i * W + (j - 1)] == 255 &&
					Image[i * W + (j + 1)] == 255)
					)// 위 아래 좌 우 순서로확인

					Output[i * W + j] = 0; //

				else Output[i * W + j] = 255;
			}
			//i*W+j가 배경 화소면 신경쓸필요없이 그대로 0처리 신경쓸 필요도 없음
			else Output[i * W + j] = 0;
		}

	}
}

void Dilation(BYTE* Image, BYTE* Output, int W, int H)
{
	//모폴로지 연산은 구조 요소이다
	//구조 요소란? => ppt
	for (int i = 1; i < H - 1; i++)
	{
		for (int j = 1; j < W - 1; j++)
		{
			//흰색이 전경이고 블랙이 배경
			if (Image[i * W + j] == 0) //배경화소라면
			{
				//배경화소처리 => 배경화소라면
				//주변을 확인해서(구조요소 체크) 하나라도 전경이면 전경으로 지정
				//모두가 배경화소인지 확인
				//하나라도 전경 화소면 Output의 i*W+j를 255 즉 전경으로 바꿔줘야해, 아니면 0
				if (!(Image[(i - 1) * W + j] == 0 &&
					Image[(i + 1) * W + j] == 0 &&
					Image[i * W + (j - 1)] == 0 &&
					Image[i * W + (j + 1)] == 0)
					)// 위 아래 좌 우 순서로확인

					Output[i * W + j] = 255; //

				else Output[i * W + j] = 0;
			}
			//i*W+j가 전경 화소면 신경쓸필요없이 그대로 0처리 신경쓸 필요도 없음
			else Output[i * W + j] = 255;
		}

	}
}

int main()
{
	BITMAPFILEHEADER hf; // 14바이트
	BITMAPINFOHEADER hInfo; // 40바이트
	RGBQUAD hRGB[256]; // 1024바이트
	FILE* fp;
	fp = fopen("dilation.bmp", "rb");
	if (fp == NULL) {
		printf("File not found!\n");
		return -1;
	}

	fread(&hf, sizeof(BITMAPFILEHEADER), 1, fp);
	fread(&hInfo, sizeof(BITMAPINFOHEADER), 1, fp);
	int ImgSize = hInfo.biWidth * hInfo.biHeight;
	int H = hInfo.biHeight;
	int W = hInfo.biWidth;

	BYTE* Image;
	BYTE* Output;
	BYTE* Temp;


	if (hInfo.biBitCount == 24) //트루컬러
	{
		// 여기서는 동적 할당에 *3씩 더 해줘야겠지?
		Image = (BYTE*)malloc(ImgSize * 3);
		Temp = (BYTE*)malloc(ImgSize * 3);
		Output = (BYTE*)malloc(ImgSize * 3);
		fread(Image, sizeof(BYTE), ImgSize * 3, fp);
	}

	else  // 그레이 (인덱스)
	{
		fread(hRGB, sizeof(RGBQUAD), 256, fp);
		Image = (BYTE*)malloc(ImgSize);
		Temp = (BYTE*)malloc(ImgSize);
		Output = (BYTE*)malloc(ImgSize);
		fread(Image, sizeof(BYTE), ImgSize, fp);
	}




	fclose(fp);

	int Histo[256] = { 0 };
	int AHisto[256] = { 0 };

	/*
	for (int i = 0; i < W; i++)
	{
		FillColor(Image, i, 200, W, H, 0, 255, 255);
	}
	*/

	/*for (int i = 100; i <= 400; i++)
	{
		for (int j = 50; j <= 300; j++)
		{
			FillColor(Image, j, i, W, H, 255, 0, 255);
		}
	}*/

	////가로 띠 만들기
	//// 초기화
	//for (int i = 0; i < H; i++)
	//{
	//	for (int j = 0; j < W; j++)
	//	{
	//		Image[i * W * 3 + j * 3 + 2] = 0; //RED
	//		Image[i * W * 3 + j * 3 + 1] = 0; //GREEN
	//		Image[i * W * 3 + j * 3] = 0 ; //BLUE
	//	}
	//}
	////y기준 0~239
	//for (int i = 0; i < 240; i++)
	//{
	//	for (int j = 0; j < W; j++)
	//	{
	//		Image[i * W * 3 + j * 3 + 2] = 255; //RED
	//	}
	//}
	////y기준 120~359
	//for (int i = 0; i < 240; i++)
	//{
	//	for (int j = 0; j < W; j++)
	//	{
	//		Image[i * W * 3 + j * 3 + 1] = 255; //GREEN
	//	}
	//}
	////y기준 240~479
	//for (int i = 0; i < 240; i++)
	//{
	//	for (int j = 0; j < W; j++)
	//	{
	//		Image[i * W * 3 + j * 3 ] = 255; //BLUE
	//	}
	//}

	//
	/*ObtainHistogram(Image, Histo, hInfo.biWidth, hInfo.biHeight);
	ObtainAHistogram(Histo, AHisto);
	HistogramEqualization(Image, Output, AHisto, hInfo.biWidth, hInfo.biHeight);*/
	/*int Thres = GozalezBinThresh();
	Binarization(Image, Output, hInfo.biWidth, hInfo.biHeight, Thres);
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
	//AverageConv(Image, Output, hInfo.biWidth, hInfo.biHeight);
	//이진화
	//Binarization(Image, Temp, hInfo.biWidth, hInfo.biHeight, 100);
	//각기 다른 컴포넌트는 다른 색으로 표시하기
	//m_BlobColoring(Temp, hInfo.biHeight, hInfo.biWidth);
	//output에 결과 저장
	//BinaryImageEdgeDetection(Temp,Output,W,H);


	//for (int i = 0; i < 256; i++)
	//{
	//	hRGB[i].rgbRed =
	//		hRGB[i].rgbBlue =
	//		hRGB[i].rgbGreen =
	//		hRGB[i].rgbReserved = i;
	//}





	Dilation(Image, Output, W, H);
	Dilation(Output, Image, W, H);
	Dilation(Image, Output, W, H); //팽창은 너무 뚱뚱하게 보일 수 있다는 단점이 있지
	//Dilation(Output, Image, W, H);
	////Dilation(Image, Output, W, H);
	Erosion(Image, Output, W, H);
	Erosion(Output, Image, W, H);
	Erosion(Image, Output, W, H); //팽창은 너무 뚱뚱하게 보일 수 있다는 단점이 있지

	//zhangSuen 코드때문에 한 번 뒤집어 줘야해 전경화소랑 배경화소가 반대로 되어있음

	InverseImage(Image, Output, W, H);

	zhangSuen(Output, Image, H, W);

	//여기까지 세선화 완료


	//12주차 과제
	FeatureExtractThinImage(Image, Output, W, H);

	SaveBMPFile(hf, hInfo, hRGB, Output, hInfo.biWidth, hInfo.biHeight, "output.bmp");


	free(Image);
	free(Output);
	//free(Temp);
	return 0;

}