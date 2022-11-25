#include "dbfunction.h"


// ���۰�ȣ üũ�ϴ� �Լ� 
int openCheck(char msg[], char tmp[], char tmp2[]) {

	int i = 0;
	int j = 0;
	int k = 0;
	int isOpen = 0;

	//�޽����� ���������� �ݺ��ؼ� ��ȣ������ �������� tmp�� ���ڸ� ���� ��ȣ�� ������ tmp2�� ���ڸ� ���� 
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
	//�ݺ��� ������ \0�� �ٿ��� ���ڿ� ���� ��Ÿ�� 
	tmp[j] = '\0';
	tmp2[k] = '\0';



}

int closeCheck(char msg[], char tmp[], char tmp2[], int *isClose, int* isComma) {

	int i = 0;
	int j = 0;
	int k = 0;
	while (msg[i] != '\0') {
		//��ǥ�� �����°�� 
		if (msg[i] == 44) {
			i++;
			*isComma = 1;
			continue;
		}
		//��ȣ �ݴ°� ������ ���̻� �� �ʿ䰡 ����� break;
		else if (msg[i] == 41) {
			*isClose = 1;
			break;
		}

		//��ǥ�� �������� tmp2�� ���� 
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


// ���꽺Ʈ�� �Լ� 
void substring(char msg[], int start, int number) {

	char tmpMsg[100];
	int i = 0;
	int j = 0;


	//���̸� ���� ��� 
	if (number != 0) {
		for (i = start; i < start + number; i++) {
			tmpMsg[j] = msg[i];
			j++;
			if (msg[i] == '\0') {
				break;
			}
		}

	}
	//���̸� ���� ���� ��쿡�� ���� ������ �ִ´� 
	else {
		i = start;
		do {
			tmpMsg[j++] = msg[i++];
		} while (msg[i] != '\0');
		

	}
	strcpy(msg, tmpMsg);


}

// ���͸� �����ϸ� �ȿ� �ִ� ����/��������� �����ϴ� �Լ� 
int rmdirs(char* path, int force)
{
	DIR* dir_ptr = NULL;
	struct dirent* file = NULL;
	struct stat   buf;
	char   filename[1024];
	char pathTmp[1024];


	printf(" path : %s \n", path);

	//���� �ȿ� ������ ������ ���� ���� 
	if ((dir_ptr = opendir(path)) == NULL) {
		return unlink(path);
	}

	//���� �ȿ� �����̳� ������ �ִ°�� �ϳ��� �о�� 
	while ((file = readdir(dir_ptr)) != NULL) {

		//printf("file -> %s \n ", file->d_name);
		// readdir ������ ���ϸ� �߿� ���� ���丮�� ��Ÿ�״� . �� ���ԵǾ� �����Ƿ� 
		// ���� �ݺ��� ������ �������� ���ϸ��� . �̸� skip �ؾ� ��
		if (strcmp(file->d_name, ".") == 0 || strcmp(file->d_name, "..") == 0) {
			continue;
		}

		sprintf(filename, "%s/%s", path, file->d_name);
		//printf("%s \n", file->d_name);
		/* ������ �Ӽ�(������ ����, ũ��, ����/���� �ð� ���� ��� ���Ͽ� */
		if (lstat(filename, &buf) == -1) {
			continue;
		}

		//printf("fileName :  %s \n", filename);

		//strcpy(pathTmp, path);
		//strcat(pathTmp, "/");
		//strcat(pathTmp, filename);
		//printf("pathTmp : %s \n", pathTmp);
		if (S_ISDIR(buf.st_mode)) { // �˻��� �̸��� �Ӽ��� ���丮�̸�
			/* �˻��� ������ directory�̸� ���ȣ��� ���� ���丮�� �ٽ� �˻� */
			if (rmdirs(filename, force) == -1 && !force) {
				return -1;
			}
		}
		else if (S_ISREG(buf.st_mode) || S_ISLNK(buf.st_mode)) { // �Ϲ����� �Ǵ� symbolic link �̸�
			if (unlink(filename) == -1 && !force) {
				return -1;
			}
		}
	}

	/* open�� directory ������ close �մϴ�. */
	closedir(dir_ptr);

	return rmdir(path);
}



//���;��� Ű �Է¹����� ������ �����ϴ� �Լ� 
char getch(void)
{
	int ch;
	// �͹̳� io�� ������ �ΰ� ����
	struct termios old;
	struct termios new;

	// ���� �͹̳� io���� ������ 
	tcgetattr(0, &old);

	// ���ο���� ������ �������� �����ͼ� �ٲ��ִ� ���� 
	new = old;
	new.c_lflag &= ~(ICANON | ECHO);
	new.c_cc[VMIN] = 1;
	new.c_cc[VTIME] = 0;

	//���� ������ ������ �͹̳� io���� �ٲ��� 
	tcsetattr(0, TCSAFLUSH, &new);
	// ���ڸ� �޾ƿ����� �ٽ� �������� �ٲ��� 
	ch = getchar();
	tcsetattr(0, TCSAFLUSH, &old);

	return ch;
}


