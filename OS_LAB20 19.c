

//=======     19          ==================================
/*
19. Шаблоны имен файлов
Программа, которая приглашает пользователя ввести шаблон имени файла,
аналогичный тому, который используется в shell.
Синтаксис шаблона таков:

	 *   соответствует последовательности любых символов кроме /,
		 имеющей любую длину; возможно - пустой последовательности.
	?    соответствует любому одному символу.
	/    не может встречаться.
	любой другой символ соответствует самому себе.

   Символы * и ? в шаблоне могут встречаться в любом количестве и в любом порядке.

 Затем программа должна найти и распечатать имена всех файлов в текущем каталоге,
 соответствующих шаблону. Если таких файлов нет, программа должна распечатать сам шаблон.

 Совет: используйте readdir, чтобы считать все имена файлов в текущем каталоге,
		и выберите из них соответствующие шаблону.
*/


#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <limits.h>


#define SIZE 256
#define FIASCO -1
#define SUCCESS 0
#define BAD_SYMBOL 1


int checkPattern(char* pattern) {
	int patternLength = strlen(pattern) - 1;
	pattern[patternLength] = 0;
	for (int i = 0; patternLength > i; i++) {
		if (pattern[i] == '/') {
			printf("Symbol / is not supported pattern\n");
			return BAD_SYMBOL;
		}
	}

}


//struct dirent {
	//	ino_t d_ino; / *inode number * /
	//		off_t d_off; / *offset to the next dirent* /
	//		беззнаковый короткий d_reclen; / *length of this record * /
	//		беззнаковый char d_type;       / *type of file; not supported;
	//		                                   by all file system types* /
	//		char d_name[256];              / *filename with '/0' * /
	//};

int printFileNames(char* pattern) {
	struct dirent* entry;
	
	DIR* dir;
	int i, j;
	int length, count = 0;
	int	patternLength = strlen(pattern) - 1;
	int flag = 0;
	// открыть текущий директорий
	dir = opendir(".");  //возвращает указатель на directory stream шаблона
	//При достижении конца списка файлов в директории или возникновении ошибки возвращает NULL
	//: устанавливает ERRNO: EACCES,ENOMEM, ENFILE, ENOENT etc
	if (!dir) {
		perror("opendir");
		return FIASCO;
	}
	//поиск файлов ссоответс-х шаблону
//Функция readdir () возвращает указатель на структуру dirent
	while (NULL != (entry = readdir(dir))) {
		length = strlen(entry->d_name);
		j = 0;
		flag = 0;  //считали весь шаблон
		for (i = 0; (length > i) && (patternLength > j); i++) {
			if (('?' != pattern[j]) && ('*' != pattern[j])) {
				if (pattern[j] != entry->d_name[i]) break;
				j++;
			}
			else if ('?' == pattern[j]) {
				j++;
			}
			else { //*
				while (patternLength > j) {
					j++;
					if ('*' != pattern[j]) break;
				}//на * шаблон закончился
				if (patternLength == j) {
					flag = 1;
					break;
				}//если после * есть ?
				if (pattern[j] == '?') {
					j++;
					continue;
				}
				while (length > i) {
					if (pattern[j] == entry->d_name[i]) {
						break;
					}
					i++;
				}
				j++;
			}
		}
		if (length == i) {
			while (patternLength > j) {
				if ('*' != pattern[j])
					break;
				j++;
			}
			if (patternLength == j) flag = 1;
		}
		if ((1 == flag) || (i == length && j == patternLength)) {
			printf("%s\n", entry->d_name);
			count++;
		}
	}
	if (!count) {
		printf("Pattern was = %s\n", pattern);
	}
	closedir(dir);
	return SUCCESS;
}

int main(int argc, char** argv) {
	
	char pattern[SIZE]; //шаблон
	// Считать шаблон
	printf("Enter pattern: ");
	fgets(pattern, SIZE, stdin);
	if (checkPattern(pattern) == BAD_SYMBOL)
		return FIASCO;
	
	if (printFileNames(pattern) == FIASCO) return FIASCO;
	
	
	return SUCCESS;
}



