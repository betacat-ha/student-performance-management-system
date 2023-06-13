# pragma warning(disable:4996)

#include <stdio.h>
#include <Windows.h>
#include <string.h>
#include <conio.h>

#define MAX_LINE 1024

FILE* fp;

// 全局结构体——学生成绩结构体
struct student {
    char course[10][20]; // 课程名称
    int courseNumber;    // 课程数量
    char className[20];  // 学生班级    
    char number[20];     // 学生学号
    char name[20];       // 学生姓名
    double score[10];    // 学生成绩
    double average;      // 平均分
    double sum;          // 总分
};

// MARK: 函数声明

void scoreInput(struct student* p);             // 输入学生成绩
void scoreModify(struct student* p);            // 修改学生成绩
void scoreQuery(struct student* p);             // 查询学生成绩
void scoreBrowse(struct student* p);            // 浏览学生成绩
void scoreDelete(struct student* p);            // 删除学生成绩
int readScores(struct student* p);              // 读取学生成绩
void importData(struct student* p);             // 导入成绩数据

/**
* 打印学生成绩,从指针p_stuscorelib所指成绩记录开始，显示后面count条学生成绩记录 
* @param p 学生成绩结构体指针
* @param count 打印数目
*/
void printRecord(struct student* p, int count);

void cleanBuffer() {
    while ((getchar()) != '\n');
    fflush(stdin);
}

/**
* 寻找\n并替换为\0
*/
void formatStr(char *str) {
    char* find = strchr(str, '\n');          //查找换行符
    if (find) *find = '\0';                  //如果find不为空指针,就把一个空字符放在这里
}

// 输入学生成绩
void scoreInput(struct student* p) {
    int count, i, m;
    char ch;
    system("cls");
    fflush(stdin);

    // 读取学生成绩
    if ((fp = fopen("学生成绩数据库.dat", "ab+")) == NULL) {
        printf("不能打开文件!\n");
        getchar();
        exit(1);
    }

    rewind(fp);

    // 统计当前学生成绩数据库中学生成绩记录总数
    for (count = 0; fread(&p[count], sizeof(struct student), 1, fp) == 1; count++);
    printf("当前学生成绩数据库中学生成绩记录总数共：%d条\n", count);

    while (1) {
        if (count == 0) {
            m = 0;
            for (i = 0; i <= 9; i++) {
                p[count].course[i][0] = '\0';
            }
            printf("<输入班级本学期课程，注意课程门数最多十门>\n");
            printf("------------------------------------------------\n");

            for (i = 0; i < 9; i++) {
                printf("第%d门课程（按回车键结束）：", i + 1);
                fgets(p[count].course[i], sizeof(p[count].course[i]), stdin);
                formatStr(p[count].course[i]);
                if (p[count].course[i][0] == '\0')
                    break;
                else m++;
            }

            p[count].courseNumber = m;

        }
        else {
            for (i = 0; i < p[0].courseNumber; i++) {
                strcpy(p[count].course[i], p[0].course[i]);
            }

            p[count].courseNumber = p[0].courseNumber;
        }

        printf("请输入学生的班级：");
        fgets(p[count].className, sizeof(p[count].className), stdin);
        formatStr(p[count].className);

        printf("请输入学生姓名：");
        fgets(p[count].name, sizeof(p[count].name), stdin);
        formatStr(p[count].name);


        printf("请输入学生学号：");
        while (1) {
            scanf("%s", &p[count].number);

            for (i = 0; i < count; i++) {
                if (strcmp(p[count].number, p[i].number) == 0) {
                    cleanBuffer();
                    printf("学号已存在，请重新输入：");
                    break;
                }
            }

            if (i == count) break;
        }

        printf("请输入该学生成绩：\n");
        printf("------------------------------------------------\n");
        p[count].sum = 0;
        for (i = 0; i < p[count].courseNumber; i++) {
            fflush(stdin);
            printf("第%d门课程%s成绩：", i + 1, p[count].course[i]);
            scanf("%lf", &p[count].score[i]);
            p[count].sum += p[count].score[i];
        }

        // 计算平均分
        p[count].average = p[count].sum / p[0].courseNumber;

        cleanBuffer();

        // 追加写入文件
        fwrite(&p[count++], sizeof(struct student), 1, fp);

        printf("------------------------------------------------\n");

        printf("是否继续输入下一个学生的成绩？（y/N）：");
        scanf("%c", &ch);
        cleanBuffer();
        if (ch == 'y' || ch == 'Y') {
            system("cls");
        } else {
            fclose(fp);
            return;
        }
    }
}

// 修改学生成绩
void scoreModify(struct student* p) {
    int i, j, k, m = 0, count;
    char tempNumber1[20], tempNumber2[20];

    // 读取学生成绩,并统计当前学生成绩数据库中学生成绩记录总数
    count = readScores(p);

    if (count == 0) {
        printf("\n还没有输入成绩，按任意键返回主菜单！");
        getchar();
        return;
    }

    printf("请输入要修改成绩的学生学号：");
    scanf("%s", tempNumber1);
    cleanBuffer();

    for (i = 0; i < count; i++) {
        if (strcmp(tempNumber1, p[i].number) != 0) {
            continue;
        }
        else {
            printf("学号为%s的学生的成绩：\n", tempNumber1);
            printRecord(p+i, 1);
            printf("修改该学生的成绩信息\n");
            printf("------------------------------------------------\n");
            printf("学生学号：");
            scanf("%s", p[i].number);
            cleanBuffer();
            strcpy(tempNumber2, p[i].number);

            printf("学生姓名：");
            scanf("%s", p[i].name);
            cleanBuffer();

            p[i].sum = 0;
            for (j = 0; j < p[0].courseNumber; j++) {
                printf("第%d门课程%s成绩：", j + 1, p[i].course[j]);
                scanf("%lf", &p[i].score[j]);
                p[i].sum += p[i].score[j];
            }

            // 计算平均分
            p[i].average = p[i].sum / p[0].courseNumber;

            printf("------------------------------------------------\n");
            printf("修改后学号为%d学生的成绩信息：\n", tempNumber1);
            printRecord(p + i, 1);

            fp = fopen("学生成绩数据库.dat", "w+b");
            for (k = 0; k < count; k++) {
                if (fwrite(&p[k], sizeof(struct student), 1, fp) != 1) {
                    printf("修改失败，按任意键返回主菜单！\n");
                    getchar();
                    system("cls");
                    return;
                }

                fclose(fp);

                for (k = 0; k < count; k++) {
                    if (tempNumber2 == p[k].number) m++;
                }

                if (m > 1) {
                    printf("提示：发现[%d]位同学的学号重复,按任意键返回主菜单重新修改\n", m);
                } else {
                    printf("修改成功，按任意键返回主菜单！\n");
                }

                getchar();
                system("cls");
                return;
            }
        }

        printf("没有找到学号为%s的学生，按任意键返回主菜单！", tempNumber1);

        getchar();
        system("cls");
        return;
    }
}

// 查询学生成绩
void scoreQuery(struct student* p) {
    char* tempName, tempNumber[20];
    int choice;
    int i, j, k, count;
    count = readScores(p);

    if (count == 0) {
        printf("\n数据库中无成绩，按任意键返回主菜单！");
        getchar();
        return;
    }

    while (1) {
        system("cls");
        printf("<【1】按姓名查询 【2】按学号查询 【3】返回主菜单>\n");
        printf("请按数字键1~3，进行相应操作：");
        scanf("%d", &choice);
        cleanBuffer();

        if (choice == 1) {
            tempName = (char*)malloc(sizeof(char) * 20);
            printf("请输入要查询成绩的学生姓名：");
            scanf("%s", tempName);
            cleanBuffer();
            //formatStr(tempName);

            k = 0;
            for (i = 0; i < count; i++) {
                if (strcmp(tempName, p[i].name) == 0) {
                    printf("学号为%s的学生的成绩：\n", p[i].number);
                    printRecord(p + i, 1);
                    k++;
                } else {
                    continue;
                }
            }

            if (k > 0) {
                printf("查询完成，共查询到%d条记录，按任意键继续！", k);
            }
            else {
                printf("没有找到学生%s的成绩，按任意键继续！", tempName);
            }
            getchar();
            system("cls");
        }
        else if (choice == 2) {
            printf("请输入要查询成绩的学生学号：");
            scanf("%s", &tempNumber);
            cleanBuffer();

            k = 0;
            for (i = 0; i < count; i++) {
                if (strcmp(tempNumber, p[i].number) == 0) {
                    printf("学号为%s的学生的成绩：\n", tempNumber);
                    printRecord(p + i, 1);
                    k++;
                } else {
                    continue;
                }
            }

            //cleanBuffer();

            if (k > 0) {
                printf("查询完成，共查询到%d条记录，按任意键继续！", k);
            }
            else {
                printf("没有找到学号为%s的学生的成绩，按任意键继续！", tempNumber);
            }

            getchar();
            system("cls");
        }
        else if(choice ==3) {
            system("cls");
            return;
        } else {
            system("cls");
        }
    }
}

// 成绩浏览
void scoreBrowse(struct student* p) {
    int i, j, count, choice, flag = -1;
    char course[10];
    struct student t;
    count = readScores(p);
    if (count == 0) {
        printf("\n空白成绩单,按任意键返回主菜单!");
        getchar();
        return;
    }
    
    while (1){
        printf("<【1】按学号顺序浏览  【2】按总成绩由大到小顺序浏览  【3】按科目成绩大小浏览  【4】返回主菜单>\n");
        printf("请按数字键1-4,选择浏览方式:");
        scanf("%d", &choice);
        //getchar();
        // 学号，冒泡排序
        if (choice == 1) {
            for (i = 1; i <= count - 1; i++) {
                for (j = count - 1; j >= i; j--) {
                    if (strcmp(p[j].number, p[j - 1].number) < 0) {
                        t = p[j];
                        p[j] = p[j - 1];
                        p[j - 1] = t;
                    }
                }
            }
        }
        //总成绩，冒泡排序
        else if (choice == 2) { 
            for (i = 1; i < count - 1; i++) {
                for (j = count - 1; j >= i; j--) {
                    if (p[j].sum > p[j - 1].sum) {
                        t = p[j];
                        p[j] = p[j - 1];
                        p[j - 1] = t;
                    }
                }
            }
        }
        //根据科目浏览，先寻找是否由这门科目，冒泡排序
        else if (choice == 3) {
            printf("请输入要浏览的科目：");
            scanf("%s", course);
            cleanBuffer();

            for (i = 0; i < p[0].courseNumber; i++) {
                if (strcmp(course, p[0].course[i]) == 0) {
                    flag = i;	//记录科目的标号
                    break;
                }
            }

            if (flag == -1) {
                printf("未查询到科目，退出查询界面！");
                Sleep(1000);
                return;
            }

            //对科目成绩排序
            for (i = 0; i < count - 1; i++)	{
                for (j = 0; j < count - 1 - i; j++) {
                    if (p[j].score[flag] < p[j + 1].score[flag]) {
                        t = p[j];
                        p[j] = p[j + 1];
                        p[j + 1] = t;
                    }
                }
            }
        } else {
            system("cls");
            return;
        }
        cleanBuffer();
        printRecord(p, count);	//打印数据
    }

}

// 显示学生成绩记录
void printRecord(struct student* p, int count) {
    int i, j, k, m[10] = { 0 }, m1;
    char ch;

    // p[0].coursenumber为当前指针所指学生成绩记录中在课程总数   
    for (j = 0; j < p[0].courseNumber; j++) {
        m[j] = strlen(p[0].course[j]) + 1;
    }

    //先获取取出表格中表头课程部分打印在字符数，以便后面在程序实现表格在动态输出
    printf("课程总数%d", p[0].courseNumber);
    
    //在学生成绩数据文件中找到学生成绩记录并显示出来 
    for (i = 0; i < count; i++) {
        // 在学生成绩数据文件中找到第一条学生成绩记录或每显示完八条学生成绩记录且还有学生成绩未显示，就显示表头
        if (i == 0 || i % 8 == 0) {
            printf("\n\t\t\t\t学生成绩\n");
            printf("|---------------|--------------------|");
            for (j = 0; j < p[0].courseNumber; j++) {
                for (k = 0; k < m[j]; k++) {
                    printf("-");
                }
                printf("|");
            }
            printf("-------|-----|\n");
            printf("|     学 号     |        姓 名       ");
            for (j = 0; j < p[0].courseNumber; j++) {
                printf("|%s ", p[0].course[j]);
            }
            printf("|平均分 |总分 |\n");
            printf("|---------------|--------------------|");
            for (j = 0; j < p[0].courseNumber; j++) {
                for (k = 0; k < m[j]; k++) {
                    printf("-");
                }
                printf("|");
            }
            printf("-------|-----|\n");
        }
        printf("|%-15s|%-20s", p[i].number, p[i].name);
        for (j = 0; j < p[0].courseNumber; j++) {
            m1 = printf("|%-.0f", p[i].score[j]);
            for (k = 0; k < m[j] - m1 + 1; k++)
                printf(" ");
        }
        printf("|%7.0f|%5.0f|\n", p[i].average, p[i].sum);
        printf("|---------------|--------------------|");
        for (j = 0; j < p[0].courseNumber; j++) {
            for (k = 0; k < m[j]; k++) {
                printf("-");
            }
            printf("|");
        }
        printf("-------|-----|\n");
        //每显示完八条学生成绩且还有学生成绩未显示，就显示提示 
        if ((i + 1) % 8 == 0 && (i + 1) < count) {
            printf("<要返回主菜单，请按【Q】键。继续浏览按其他键！>");
            ch = getchar();
            fflush(stdin);
            if (ch == 'q' || ch == 'Q') return;
            else system("cls");
        }
    }
}

// 成绩删除                               
void scoreDelete(struct student* p) {
    int i, j, count;
    char ch, tempNumber[20];
    count = readScores(p); //读出学生成绩，并返回学生成绩记录数 
    if (count == 0) {
        printf("\n数据库中还没有成绩,按任意键返回主菜单!");
        getchar();
        return;
    }
    printRecord(p, count);   //显示当前学生成绩数据全部记录 
    printf("确定要进行删除成绩的操作吗?(y/N)");
    while (1) {
        ch = getchar();
        if (ch == 'y' || ch == 'Y') {
            printf("请输入要删除成绩记录在学生学号（输入无效学号返回主菜单）");
            scanf("%s", &tempNumber);

            for (i = 0; i < count; i++) {
                if (strcmp(tempNumber, p[i].name) != 0)
                    continue;
                else {
                    for (j = i; j < count - 1; j++) {
                        p[j] = p[j + 1];
                    }
                    count--;  //学生成绩记录自减1 
                    fp = fopen("学生成绩数据库.dat", "w+b");
                    for (i = 0; i < count; i++) {
                        if (fwrite(&p[i], sizeof(struct student), 1, fp) == 1)	//写入数据库
                            continue;
                        else {
                            printf("删除失败，按任意键返回主菜单！");
                            fclose(fp);
                            getchar();
                            system("cls");
                            return;
                        }
                    }
                    printf("删除成功，按任意键返回主菜单！");
                    fclose(fp);
                    getchar();
                    system("cls");
                    return;
                }
            }
        } else {
            system("cls");
            return;
        }
    }
}

// 读入学生成绩，并返回学生成绩记录数
int readScores(struct student* p) {
    int count;		//count记录成绩记录数
    system("cls");	//清屏操作
    if ((fp = fopen("学生成绩数据库.dat", "rb")) == NULL) {
        printf("打不开学生成绩数据库文件！！！");
        getchar();
        return 0;
    }
    rewind(fp);  //文件位置指针回到文件起始位置 
    for (count = 0; fread(&p[count], sizeof(struct student), 1, fp) == 1; count++); //循环获取记录数
    fclose(fp);
    return count;  //把读到在成绩记录总数返回 
}

//数据导入
void importData(struct student* p)
{
    system("cls");
    int count = 0;
    char file[100];	//导入的文件路径和名称
    char buf[MAX_LINE];	//存放txt文件内一行的数据
    int len;	//buf的长度
    FILE* fp1;

    printf("\n\n\n\n\n\n\n\n\t\t\    #################################\n");
    printf("\t\t    #                               #\n");
    printf("\t\t    #                               #\n");
    printf("\t\t    #                               #\n");
    printf("\t\t    # 将文件拖拽到这里,并按下回车键 #\n");
    printf("\t\t    #       输入【#】取消导入       #\n");
    printf("\t\t    #                               #\n");
    printf("\t\t    #                               #\n");
    printf("\t\t    #                               #\n");
    printf("\t\t    #################################\n\n\n");
    printf("\t\t    ");
    scanf("%s", file);	//输入导入文件的路径

    if (file[0] == '#') {
        return;
    }

    if ((fp1 = fopen(file, "r")) == NULL) {
        printf("文件打开失败\n");
        exit(1);
    }
    // 读取文件
    while (fgets(buf, MAX_LINE, fp1) != NULL) {
        len = strlen(buf);
        buf[len - 1] = '\0';
        if (count == 0) {
            strcpy(p[0].number, strtok(buf, ","));
            strcpy(p[0].className, strtok(NULL, ","));
            strcpy(p[0].name, strtok(NULL, ","));
            if (strcmp(p[0].number, "学号") != 0 ||
                strcmp(p[0].className, "班别") != 0 ||
                strcmp(p[0].name, "姓名") != 0) {
                printf("文件数据格式不正确，正确的表头应由\"学号,班别,姓名\"开始\n导入失败，按任意键返回主菜单。\n");
                getchar();
                return;
            }

            // 读入科目名称
            int courseNumber = 0;
            char* courseName = NULL;
            for (courseName = strtok(NULL, ","); courseName != NULL; courseName = strtok(NULL, ",")) {
                strcpy(p[0].course[courseNumber], courseName);
                courseNumber++;
            }

        } else {
            strcpy(p[count - 1].number, strtok(buf, ","));
            strcpy(p[count - 1].className, strtok(NULL, ","));
            strcpy(p[count - 1].name, strtok(NULL, ","));
            //复制成绩，并且将字符串转为数字
            for (int i = 0; i < p[0].courseNumber; i++) {
                p[count - 1].score[i] = atof(strtok(NULL, ","));
            }
            //计算总分和平均分
            p[count - 1].sum = p[count - 1].score[0] + p[count - 1].score[1] +
                p[count - 1].score[2] + p[count - 1].score[3] + p[count - 1].score[4] +
                p[count - 1].score[5];
            p[count - 1].average = p[count - 1].sum / 6;
            //将课程和课程数目
            memcpy(p[count - 1].course, p[0].course, sizeof(p[0].course));
            p[count - 1].courseNumber = 6;
        }
        count++;
    }
    fclose(fp1);
    count--;//减去一位记录数
    fp = fopen("学生成绩数据库.dat", "w+b");//将记录存进数据库内
    for (int i = 0; i < count; i++)
    {
        fwrite(&p[i], sizeof(struct student), 1, fp);
    }
    fclose(fp);
}


int main(void) {
    struct student stusorelib[100]; // 学生成绩库
    int choice, flag = 0;            // 选择

    printf("\n\n\n\n\n\n\n\n\t\t    ################################\n");
    printf("\t\t    #                              #\n");
    printf("\t\t    # --欢迎使用学生成绩管理系统-- #\n");
    printf("\t\t    #                              #\n");
    printf("\t\t    ################################\n");
    //getchar();

    while (1)
    {
        system("cls");
        printf("\n\n\n\n");
        printf("\n\n\n\n");
        printf("\t\t *++++++++++++++++++++++++++++++++++++*\n");
        printf("\t\t *                                    *\n");
        printf("\t\t *           学生成绩管理系统         *\n");
        printf("\t\t *                                    *\n");
        printf("\t\t *++++++++++++++++++++++++++++++++++++*\n");
        printf("\t\t *                                    *\n");
        printf("\t\t *              1.成绩录入            *\n");
        printf("\t\t *                                    *\n");
        printf("\t\t *              2.成绩修改            *\n");
        printf("\t\t *                                    *\n");
        printf("\t\t *              3.成绩查询            *\n");
        printf("\t\t *                                    *\n");
        printf("\t\t *              4.成绩浏览            *\n");
        printf("\t\t *                                    *\n");
        printf("\t\t *              5.成绩删除            *\n");
        printf("\t\t *                                    *\n");
        printf("\t\t *              6.成绩导入            *\n");
        printf("\t\t *                                    *\n");
        printf("\t\t *              9.退出系统            *\n");
        printf("\t\t *                                    *\n");
        printf("\t\t *++++++++++++++++++++++++++++++++++++*\n");
        printf("\t\t  请按数字键1~6，进行相应操作：");
        scanf("%d", &choice);
        cleanBuffer();
        switch (choice) {
        case 1:
            scoreInput(stusorelib);
            break;
        case 2:
            scoreModify(stusorelib);
            break;
        case 3:
            scoreQuery(stusorelib);
            break;
        case 4:
            scoreBrowse(stusorelib);
            break;
        case 5:
            scoreDelete(stusorelib);
            break;
        case 6:
            importData(stusorelib);
            break;
        case 9:
            flag = 1;
            break;
        default:
            printf("\t\t    <没有此选项，按任意键重新选择！>");
            getchar();
            break;
        }
        if (flag == 1)
            break;
    }

    while (1) {
        system("cls");
        /* 学生成绩管理系统结束界面 */
        printf("\n\n\n\n\n\n\n\n\t\t\    ################################\n");
        printf("\t\t    #                              #\n");
        printf("\t\t    # --   已退出成绩管理系统   -- #\n");
        printf("\t\t    # --       谢谢使用！       -- #\n");
        printf("\t\t    #                              #\n");
        printf("\t\t    ################################\n");

        printf("\n\n\t\t          现在可以安全关闭程序");
        getchar();
    }

    return 0;
}