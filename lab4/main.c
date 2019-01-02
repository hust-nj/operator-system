#include<unistd.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<stdio.h>
#include<dirent.h>
#include<stdlib.h>
#include<string.h>
#include<pwd.h>
#include<grp.h>
#include<time.h>


void printmod(int mode)
{
	char modebuf[11];
	strcpy(modebuf, "----------");

	//file type
	if(S_ISDIR(mode))
		modebuf[0] = 'd';
	if(S_ISCHR(mode))
		modebuf[0] = 'c';
	if(S_ISBLK(mode))
		modebuf[0] = 'b';
	if(S_ISLNK(mode))
		modebuf[0] = 'l';

	//file permission bits
	if(mode & S_IRUSR)
		modebuf[1] = 'r';
	if(mode & S_IWUSR)
		modebuf[2] = 'w';
	if(mode & S_IXUSR)
		modebuf[3] = 'x';

	if(mode & S_IRGRP)
		modebuf[4] = 'r';
	if(mode & S_IWGRP)
		modebuf[5] = 'w';
	if(mode & S_IXGRP)
		modebuf[6] = 'x';

	if(mode & S_IROTH)
		modebuf[7] = 'r';
	if(mode & S_IWOTH)
		modebuf[8] = 'w';
	if(mode & S_IXOTH)
		modebuf[9] = 'x';

	printf("%s", modebuf);
}

void printuser(int uid)
{
	struct passwd *pwd;
	if((pwd = getpwuid(uid)) == NULL)
	{
		fprintf(stderr, "Can't print the uid %d\n", uid);
		return;
	}
	printf(" %s", pwd->pw_name);
}

void printgroup(int gid)
{
	struct group *grp;
	if((grp = getgrgid(gid)) == NULL)
	{
		fprintf(stderr, "Can't print the gid %d\n", gid);
		return;
	}
	printf(" %s", grp->gr_name);
}

void printtime(time_t t)
{
	char strtime[26];
	strcpy(strtime, ctime(&t));
	strtime[24] = '\0';//去掉换行
	printf(" %s", strtime);
}

void printinfo(const char *name, const struct stat *statbuf)
{
	printf("%s ", name);//打印文件名
	printmod(statbuf->st_mode);//打印模式
	printf(" %lu", statbuf->st_nlink);//打印硬链接数
	printuser(statbuf->st_uid);//打印用户名
	printgroup(statbuf->st_gid);//打印群组名
	printf(" %ld", statbuf->st_blksize);
	// printf(" %s", ctime(&statbuf->st_ctime));//打印时间(文件最后修改属性的时间)
	printtime(statbuf->st_ctime);//不换行打印
	// printf(" %s\n", name);//打印文件名
	puts("");//换行
}

void printdir(char *dir, int depth)
{
	DIR *dp;
	struct dirent *entry;
	struct stat statbuf;
	if((dp = opendir(dir)) == NULL)
	{
		fprintf(stderr, "can't open dir %s\n", dir);
		return;
	}

	if(lstat(dir, &statbuf) == -1)
	{
		fprintf(stderr, "lstat error on %s\n", dir);
		return;
	}
	printf("%*s", depth, "");
	printinfo(dir, &statbuf);//打印目录信息


	if(chdir(dir) == -1)
	{
		fprintf(stderr, "can't change dir to %s\n", dir);
		return;
	}

	while(entry = readdir(dp))
	{
		// printf("指针为 %p\n", entry);
		if(lstat(entry->d_name, &statbuf) == -1)
		{
			fprintf(stderr, "lstat error on %s\n", entry->d_name);
			return;
		}

		if(entry->d_name[0] == '.')
			continue;		

		if(entry->d_type == DT_DIR)//如果文件是目录
		{
			printdir(entry->d_name, depth + 4);
		}
		else//如果是非目录文件
		{
			printf("%*s", depth + 4, "");
			printinfo(entry->d_name, &statbuf);
		}
	}
	chdir("..");
	closedir(dp);
	return;
}

int main()
{
	printdir(".", 0);
	return 0;
}