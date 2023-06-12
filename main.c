#include <stdio.h>

FILE *fp;

// 全局结构体——学生成绩结构体
struct student
{
    char course[10][20]; // 课程名称
    int courseNumber;    // 课程数量
    int number;          // 学生学号
    char name[8];        // 学生姓名
    double score[10];    // 学生成绩
    double average;      // 平均分
    double sum;          // 总分
};

/****************函数说明******************/
void scoreInput(struct student *p);  // 输入学生成绩
void scoreModify(struct student *p); // 修改学生成绩
void scoreQuery(struct student *p);  // 查询学生成绩
void scoreBrowse(struct student *p); // 浏览学生成绩
void scoreDelete(struct student *p); // 删除学生成绩
void printRecord(struct student *p); // 打印学生成绩
int readScores(struct student *p);   // 读取学生成绩
int main(void)
{
    struct student stusorelib[100]; // 学生成绩库
    int choice, flag = 0;            // 选择

    printf("\n\n\n\n\n\n\n\n\t\t\t################################\n");
    printf("\t\t\t#                              #\n");
    printf("\t\t\t#  --欢迎使用学生成绩管理系统--  #\n");
    printf("\t\t\t#                              #\n");
    printf("\t\t\t################################\n");

    while (1)
    {
        system("cls");
        printf("\n\n\n\n");
        printf("\n\n\n\n");
        printf("\t\t *++++++++++++++++++++++++++++++++++++*\n");
        printf("\t\t *                                    *\n");
        printf("\t\t *            学生成绩管理系统          *\ n");
        printf("\t\t *                                    *\n");
        printf("\t\t *++++++++++++++++++++++++++++++++++++*\n");
        printf("\t\t *                                    *\n");
        printf("\t\t *              1.成绩录入             *\n");
        printf("\t\t *                                    *\n");
        printf("\t\t *              2.成绩修改             *\n");
        printf("\t\t *                                    *\n");
        printf("\t\t *              3.成绩查询             *\n");
        printf("\t\t *                                    *\n");
        printf("\t\t *              4.成绩浏览             *\n");
        printf("\t\t *                                    *\n");
        printf("\t\t *              5.成绩删除             *\n");
        printf("\t\t *                                    *\n");
        printf("\t\t *              6.退出系统             *\n");
        printf("\t\t *                                    *\n");
        printf("\t\t *++++++++++++++++++++++++++++++++++++*\n");
        printf("\t\t  请按数字键1~6，进行相应操作：");
        scanf("%d", &choice);
        switch (choice)
        {
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
            flag = 1;
            break;
        default:
            printf("\t\t    <没有此选项，按任意键重新选择！>");
            getch();
            break;
        }
        if (flag == 1)
            break;
    }

    /* 学生成绩管理系统结束界面 */
    printf("\n\n\n\n\n\n\n\n\t\t\ ################################\n");
    printf("\t\t\t #                              #\n");
    printf("\t\t\t # --    已退出成绩管理系统    -- #\n");
    printf("\t\t\t # --        谢谢使用！       -- #\n");
    printf("\t\t\t #                              #\n");
    printf("\t\t\t ################################\n");

    return 0;
}

// 输入学生成绩
void scoreInput(struct student *p) {
    int count, i, m;
    char ch;
    system("cls");
    fflush(stdin);

    // 读取学生成绩
    if((fp=fopen("学生成绩数据库.dat","ab+"))==NULL){
        printf("不能打开文件!\n");
        getch();
        exit(1);
    }

    rewind(fp);

    // 统计当前学生成绩数据库中学生成绩记录总数
    for (count = 0; fread(&p[count],sizeof(struct student),1,fp) == 1; count++);
    printf("当前学生成绩数据库中学生成绩记录总数共：%d条\n", count);
    
    while(1){
        if(count==0) {
            m = 0;
            for(i = 0; i <= 9; i++) {
                p[count].course[i][0] = '\0';
            }
            printf("<输入班级本学期课程，注意课程门数最多十门>\n");
            printf("------------------------------------------------\n");

            for (i = 0; i < 9; i++) {
                printf("第%d门课程（按回车键结束）：", i + 1);
                gets(p[count].course[i]);
                if (p[count].course[i][0] == '\0')
                    break;
                else m++;
            }

            p[count].courseNumber = m;
            
        } else {
            for (i = 0; i < p[0].courseNumber; i++) {
                strcpy(p[count].course[i], p[0].course[i]);
            }

            p[count].courseNumber = p[0].courseNumber;
        }

        printf("请输入学生姓名：");
        gets(p[count].name);

        printf("请输入学生学号：");
        while(1) {
            scanf("%d", &p[count].number);
            if(p[count].number < 0) {
                printf("学号不能为负数，请重新输入：");
            };

            for (i = 0; i < count; i++) {
                if(p[count].number == p[i].number) {
                    fflush(stdin);
                    printf("学号已存在，请重新输入：");
                    break;
                }
            }
            
            if(i==count) break;
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

        fflush(stdin);

        // 追加写入文件
        fwrite(&p[count++], sizeof(struct student), 1, fp);

        printf("------------------------------------------------\n");

        printf("是否继续输入下一个学生的成绩？（y/N）：");

        fflush(stdin);
        scanf("%c", &ch);
        if (ch == 'y' || ch == 'Y') {
            system("cls");
        } else {
            fclose(fp);
            return;
        }
    }
}

// 修改学生成绩
void scoreModify(struct student *p) {
    int i, j, k, m = 0, count, tempNumber, tempNumber2;

    // 读取学生成绩,并统计当前学生成绩数据库中学生成绩记录总数
    count = readScores(p);

    if(count == 0) {
        printf("\n还没有输入成绩，按任意键返回主菜单！");
        getch();
        return;
    }

    printf("请输入要修改成绩的学生学号：");
    scanf("%d", &tempNumber);
    fflush(stdin);

    for (i = 0; i < count; i++) {
        if (tempNumber != p[i].number) {
            continue;
        } else {
            printf("学号为%d的学生的成绩：\n", tempNumber);
            printRecord(&p[i]);
            printf("修改该学生的成绩信息\n");
            printf("------------------------------------------------\n");
            printf("学生学号：");
            scanf("%d", &p[i].number);
            tempNumber2 = p[i].number;
            fflush(stdin);

            printf("学生姓名：");
            gets(p[i].name);
            fflush(stdin);

            p[i].sum = 0;
            for (j = 0; j < p[0].courseNumber; j++) {
                printf("第%d门课程%s成绩：", j + 1, p[i].course[j]);
                scanf("%lf", &p[i].score[j]);
                p[i].sum += p[i].score[j];
            }

            // 计算平均分
            p[i].average = p[i].sum / p[0].courseNumber;

            printf("------------------------------------------------\n");

            printf("修改后学号为%d学生的成绩信息：\n", tempNumber);
            printRecord(p+i,1);
            fp = open("学生成绩数据库.dat", "w+b");

            for (k = 0; k < count; k++) {
                if (fwrite(&p[k], sizeof(struct student), 1, fp) != 1) {
                    printf("修改失败，按任意键返回主菜单！\n");
                    getch();
                    system("cls");
                    return;
                }

                fclose(fp);

                for(k = 0; k < count; k++) {
                    if(tempNumber2 == p[k].number) m++;
                }

                if(m>1) {
                    printf("管理员提示：有[%d]位同学的学号重号了！！按任意键返回主菜单重新修改\n", m);
                } else {
                    printf("修改成功，按任意键返回主菜单！\n");
                }

                getch();
                system("cls");
                return;
            }
        }

        printf("没有找到学号为%d的学生，按任意键返回主菜单！", tempNumber);

        getch();
        system("cls");
        return;
    }

}