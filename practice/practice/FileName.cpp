#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <filesystem>
#include <chrono>

// void save(const std::string& fileName);
void save(std::string_view fileName);	// 읽기 모드 경량 객체.

int main() {
	std::cout << "2025년 1학기 STL" << std::endl;

	save("FileName.cpp");
}

void save(std::string_view fileName) {
	// 1. 인자로 전달된 fileName을 읽기 모드로 연다.
	// 만일 열기에 성공, 실패했다면,,
	std::ifstream in{ fileName.data()};	// string_view는 최신 표준이라 옛날 표준인 ifstream에 맞춰서 초기화를 해야한다. RAII
	if (not in) {
		std::cout << fileName << "을 열 수 없습니다." << std::endl;
		exit(20250310);	// 숫자는 단순 시스템 전달 용도.
	}

	// 저장했다는 것을 화면에 표시하자.
	std::cout << fileName << "(" << std::filesystem::file_size(fileName) << "bytes) 저장하였습니다." << std::endl;

	// 2. 쓰기 모드로 저장할 파일을 (덧붙이기 모드로) 연다.
	std::ofstream out{ "2025_1학기_STL강의저장.txt", std::ios::app };	// { } << uniform initializer

	// 시간을 기록하자.
	using namespace std::chrono_literals;
	auto now = std::chrono::system_clock::now() + 9h;		// epoch로부터 경과된 tick 수를 얻는다.
	auto utc = std::chrono::system_clock::to_time_t(now);	// UTC로 변환한다.
	auto lt = std::localtime(&utc);							// 현지 시간으로 변환한다.
	auto old = out.imbue(std::locale("ko_KR"));				// 한국어로 출력한다.

	std::cout << "현재 시간 - " << std::put_time(lt, "%c %A") << '\n';


	out << '\n' << '\n';
	out << "----------------------------------------" << '\n';
	out << "저장한 시간: " << std::filesystem::last_write_time(fileName) + 9h << '\n';
	out << "저장한 시간: " << std::put_time(lt, "%c %A") << '\n';
	out << "----------------------------------------" << '\n';
	out << "\n";
	out.imbue(old);											// 원래대로 돌려놓는다.

	// 3. 파일을 읽어서 저장한 파일에 덧붙인다.
	/* C언어 스타일
	char c;					// 스택에 1바이트
	in >> std::noskipws;	// 공백도 저장해줘.
	while (in >> c) {		// 고급 입출력 -> istream& operator >> (istream& in, char c);
							// 글자만 읽어달라고 했기 때문에 공백을 건너 뛰고 읽음.
		out << c;			// 매번 while 조건을 체크하기 때문에 매우 비효율적.
	}*/
	/* 메모리 읽어오는 과정도 필요없음
	std::vector<char> v(std::filesystem::file_size(fileName));	// 파일 크기만큼 벡터를 할당.
	std::copy(std::istreambuf_iterator{ in }, {}, v.begin());	// 파일을 벡터에 복사. {} 은 파일의 끝을 의미함.
	std::copy(v.begin(), v.end(), std::ostreambuf_iterator{ out });	// 벡터를 파일에 복사.*/
	std::copy(std::istreambuf_iterator{ in }, {}, std::ostreambuf_iterator{ out });	// vector도 사용하지 않음.
}