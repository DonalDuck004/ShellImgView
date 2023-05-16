#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <format>

void GetWindowSize(int& rows, int& columns);
void GetSpaceCounts(int argc, char**& argv, int& space_c);
void OpenImage(char* path, cv::Mat& image);
void Print(cv::Mat& image, int scale, int space_c);
int main(int argc, char* argv[]);

#ifdef _WIN32
#include <windows.h>

void GetWindowSize(int& rows, int& columns) {
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
}

#else

#include <unistd.h>
#include <sys/ioctl.h>

void GetWindowSize(int& rows, int& columns) {
	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

	rows = w.ws_row;
	columns = w.ws_col;
}

#endif

void GetSpaceCounts(int argc, char** &argv, int& space_c) {
	if (argc > 2) {
		try {
			space_c = std::stoi(argv[2]);
		}
		catch (std::exception) {
		}

		if (space_c < 1) {
			std::cerr << "Invalid space_c given" << std::endl;

			exit(1);
		}
	}
	else
		space_c = 2;
}

void OpenImage(char* path, cv::Mat& image) {
	image = cv::imread(path);
	if (image.data == NULL)
	{
		std::cerr << "CV failed image opening" << std::endl;

		exit(1);
	}
}

void Print(cv::Mat& image, int scale, int space_c) {
	
	cv::Mat rect;
	cv::Scalar bgr;

	for (int r = 0; r < image.rows; r += scale)
	{
		for (int c = 0; c < image.cols; c += scale)
		{
			rect = image(cv::Range(r, std::min<int>(r + scale, image.rows)), cv::Range(c, std::min<int>(c + scale, image.cols)));
			bgr = cv::mean(rect);
			std::cout << std::format("\x1b[48;2;{};{};{}m{}", floor(bgr[2]), floor(bgr[1]), floor(bgr[0]), std::string(space_c, ' '));
		}

		std::cout << "\033[0m\n";
	}
}

int main(int argc, char* argv[])
{
	if (argc == 1) {
		std::cerr << "No path given" << std::endl;

		return 1;
	}
	int terminal_rows, terminal_columns, scale, space_c;
	cv::Mat image;

	GetSpaceCounts(argc, argv, space_c);
	OpenImage(argv[1], image);
	GetWindowSize(terminal_rows, terminal_columns);

	scale = std::max<int>(image.cols / terminal_columns, image.rows / terminal_rows);
	Print(image, scale, space_c);

	return 0;
}
