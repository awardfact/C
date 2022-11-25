#include "dbfunction.h"


// 시작괄호 체크하는 함수 
int openCheck(char msg[], char tmp[], char tmp2[]) {

	int i = 0;
	int j = 0;
	int k = 0;
	int isOpen = 0;

	//메시지가 끝날떄까지 반복해서 괄호나오기 전까지는 tmp에 문자를 저장 괄호가 나오면 tmp2에 문자를 저장 
	while (msg[i] != '\0') {



		if (msg[i] == 40) {
			i++;
			isOpen = 1; 
			continue;
		}


		if (isOpen == 0) {
			tmp[j++] = msg[i++];

		}
		else {

			tmp2[k++] = msg[i++];

		}
	}
	//반복이 끝나면 \0을 붙여서 문자열 끝을 나타냄 
	tmp[j] = '\0';
	tmp2[k] = '\0';



}

int closeCheck(char msg[], char tmp[], char tmp2[], int *isClose, int* isComma) {

	int i = 0;
	int j = 0;
	int k = 0;
	while (msg[i] != '\0') {
		//쉼표가 나오는경우 
		if (msg[i] == 44) {
			i++;
			*isComma = 1;
			continue;
		}
		//괄호 닫는게 나오면 더이상 볼 필요가 없어사 break;
		else if (msg[i] == 41) {
			*isClose = 1;
			break;
		}

		//쉼표가 나왔으면 tmp2에 넣음 
		if (*isComma == 0) {
			tmp[j++] = msg[i++];

		}
		else {

			tmp2[k++] = msg[i++];

		}



	}


	tmp[j] = '\0';
	tmp2[k] = '\0';


}


// 서브스트링 함수 
void substring(char msg[], int start, int number) {

	char tmpMsg[100];
	int i = 0;
	int j = 0;


	//길이를 받은 경우 
	if (number != 0) {
		for (i = start; i < start + number; i++) {
			tmpMsg[j] = msg[i];
			j++;
			if (msg[i] == '\0') {
				break;
			}
		}

	}
	//길이를 받지 않은 경우에는 문자 끝까지 넣는다 
	else {
		i = start;
		do {
			tmpMsg[j++] = msg[i++];
		} while (msg[i] != '\0');
		

	}
	strcpy(msg, tmpMsg);


}

// 디렉터리 삭제하면 안에 있는 파일/폴더들까지 삭제하는 함수 
int rmdirs(char* path, int force)
{
	DIR* dir_ptr = NULL;
	struct dirent* file = NULL;
	struct stat   buf;
	char   filename[1024];
	char pathTmp[1024];


	printf(" path : %s \n", path);

	//폴더 안에 파일이 없으면 폴더 삭제 
	if ((dir_ptr = opendir(path)) == NULL) {
		return unlink(path);
	}

	//폴더 안에 파일이나 폴더가 있는경우 하나씩 읽어옴 
	while ((file = readdir(dir_ptr)) != NULL) {

		//printf("file -> %s \n ", file->d_name);
		// readdir 읽혀진 파일명 중에 현재 디렉토리를 나타네는 . 도 포함되어 있으므로 
		// 무한 반복에 빠지지 않으려면 파일명이 . 이면 skip 해야 함
		if (strcmp(file->d_name, ".") == 0 || strcmp(file->d_name, "..") == 0) {
			continue;
		}

		sprintf(filename, "%s/%s", path, file->d_name);
		//printf("%s \n", file->d_name);
		/* 파일의 속성(파일의 유형, 크기, 생성/변경 시간 등을 얻기 위하여 */
		if (lstat(filename, &buf) == -1) {
			continue;
		}

		//printf("fileName :  %s \n", filename);

		//strcpy(pathTmp, path);
		//strcat(pathTmp, "/");
		//strcat(pathTmp, filename);
		//printf("pathTmp : %s \n", pathTmp);
		if (S_ISDIR(buf.st_mode)) { // 검색된 이름의 속성이 디렉토리이면
			/* 검색된 파일이 directory이면 재귀호출로 하위 디렉토리를 다시 검색 */
			if (rmdirs(filename, force) == -1 && !force) {
				return -1;
			}
		}
		else if (S_ISREG(buf.st_mode) || S_ISLNK(buf.st_mode)) { // 일반파일 또는 symbolic link 이면
			if (unlink(filename) == -1 && !force) {
				return -1;
			}
		}
	}

	/* open된 directory 정보를 close 합니다. */
	closedir(dir_ptr);

	return rmdir(path);
}



//엔터없이 키 입력받으면 데이터 저장하는 함수 
char getch(void)
{
	int ch;
	// 터미널 io값 변수를 두개 만듬
	struct termios old;
	struct termios new;

	// 기존 터미널 io값을 가져옴 
	tcgetattr(0, &old);

	// 새로운것의 설정도 기존값을 가져와서 바꿔주는 형식 
	new = old;
	new.c_lflag &= ~(ICANON | ECHO);
	new.c_cc[VMIN] = 1;
	new.c_cc[VTIME] = 0;

	//새로 설정된 값으로 터미널 io값을 바꿔줌 
	tcsetattr(0, TCSAFLUSH, &new);
	// 문자를 받아오고나서 다시 원래껄로 바꿔줌 
	ch = getchar();
	tcsetattr(0, TCSAFLUSH, &old);

	return ch;
}


