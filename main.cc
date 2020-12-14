// ������ ��������� ��� ������ �� 5.10.2020-6.10.2020
//
/*
	������:
	��������� ������ �������
		z = sinc(sqrt(x*x + y*y))
	��� 1. ��������� ������� ��� �������� ��������� �� ��������� �������
			(x, y, z) � ���������� ����� �� �������� (sx, sy)
			� ������ ���������� ������������� �� ���� ��� ����
	��� 2. ��������� ������ �������, ��������� ���� ���������� � ������� �����,
			� ������ - � ��������� �����.
	��� 2.5. ��������� ������ ��� ���, ������� ������� ��� �� ����.
		(�� ������ ����� � ��� ��� ��������� ����� ������, ��....)
	��� 3. ��� �������� ��������� ����� ������� ����������� ��������
			"���������� ���������"
				1. ������ ������ �������� �� ������� �������� � �������
				2. ���� ��������� ����� ��������� ���� ����� ���������,
					�� � �� ������
				3. ���� ��������� ����� ��������� ���� ����� ���������, ��:
					* ����� ������;
					* ��������� � ���� ������� ����������� �������� �� ������
					  ����� �����
				4. ����� ������ �������� ��������� ������� (� ����������������
					����������) �������� ��������.
		������� ����������: -30 <= x <= 30 � -30 <= y <= 30 (��������)
 */

#include <fstream>
#include <cmath>
#include <cstdint>
#include <vector>

// ��� � ��� ��������� ��������� TGA-�����
#pragma pack(push, 1)
struct TGA_Header
{
	uint8_t id_len;			// ����� ��������������
	uint8_t pal_type;		// ��� �������
	uint8_t img_type;		// ��� �����������
	uint8_t pal_desc[5];	// �������� �������
	uint16_t x_pos;			// ��������� �� ��� X
	uint16_t y_pos;			// ��������� �� ��� Y
	uint16_t width;			// ������
	uint16_t height;		// ������
	uint8_t depth;			// ������� �����
	uint8_t img_desc;		// ��������� �����������
};
#pragma pack(pop)

constexpr uint16_t IMG_WIDTH = 1280;
constexpr uint16_t IMG_HEIGHT = 720;
constexpr uint32_t COL_BACKGROUND = 0xff003f3f;
constexpr uint32_t COL_FOREGROUND = 0xffcfcfcf;

// ��� ����� �� ������������ ���� ���������

double sinc(double x)
{
	if (x == 0.0) return 1.0;
	return sin(x) / x;
}

double my_evil_function(double x, double y)
{
	return sinc(hypot(x, y));
}


int main()
{
	// �������� ����������� ����������� � ���� ������� TGA
	TGA_Header hdr {};
	hdr.width = IMG_WIDTH;
	hdr.height = IMG_HEIGHT;
	hdr.depth = 32;
	hdr.img_type = 2;
	hdr.img_desc = 0x28;
	// ��������� ������ ������� � ��������� ������
	std::vector<uint32_t> picture(IMG_WIDTH * IMG_HEIGHT);
	for (auto && p : picture) p = COL_BACKGROUND;
	// ����� ��� �� ������������ ��� ���.
	static constexpr auto Pi = acos (-1.);
	double shag1=0.3, shag2=0.001;
	double  R=30.0;
	double ugol=Pi/6.0;
	int massiv[IMG_WIDTH];
	for (int i=0; i<IMG_WIDTH; i++) massiv[i]=IMG_HEIGHT;
	int x1, y1;
	double z;
		for (double x=R; x>-R; x-=shag1) {
			for (double y=R; y>-R; y-=shag2) {
				z=double(my_evil_function(x, y));
				x1=int(IMG_WIDTH/2+20.0*cos(ugol)*(y-x));
				y1=int(IMG_HEIGHT/2+20.0*sin(ugol)*(x+y)-200.0*z);
				if(0<x1 && x1<IMG_WIDTH && y1*IMG_WIDTH+x1<=int(IMG_WIDTH * IMG_HEIGHT) && massiv[x1]>y1){
					massiv[x1] = y1;
					if(0<y1 && y1<=IMG_HEIGHT )
						picture[y1*IMG_WIDTH+x1]=COL_FOREGROUND;
				}
			}
		}
		for (int i=0; i<IMG_WIDTH; i++) massiv[i]=IMG_HEIGHT;
		for (double y=R; y>-R; y-=shag1) {
			for (double x=R; x>-R; x-=shag2) {
				z=double(my_evil_function(x, y));
				x1=int(IMG_WIDTH/2+20.0*cos(ugol)*(y-x));
				y1=int(IMG_HEIGHT/2+20.0*sin(ugol)*(x+y)-200.0*z);
				if(0<x1 && x1<IMG_WIDTH && y1*IMG_WIDTH+x1<=int(IMG_WIDTH * IMG_HEIGHT) && massiv[x1]>y1){
					massiv[x1] = y1;
					if(0<y1 && y1<=IMG_HEIGHT )
						picture[y1*IMG_WIDTH+x1]=COL_FOREGROUND;
				}
			}
		}


	// ������ ����� + ��������� ����� ����
	std::ofstream tga_file { "output.tga", std::ios::binary };
	// ���������� ��������� � ������ ��������
	tga_file.write(reinterpret_cast<char*>(&hdr), sizeof(hdr));
	tga_file.write(reinterpret_cast<char*>(&picture[0]),
			picture.size() * 4.0);
	// ��������� ����
	tga_file.close();

	return 0;
}
