#include <stdio.h>

// 全局结构体——学生成绩结构体
struct student {
    char course[10][20];    // 课程名称
    int courseNumber;       // 课程数量
    int number;             // 学生学号
    char name[8];           // 学生姓名
    double score[10];       // 学生成绩
    double average;         // 平均分
    double sum;             // 总分
};

/****************函数说明******************/
void scoreInput(struct student *p);     // 输入学生成绩
void scoreModify(struct student *p);    // 修改学生成绩
void scoreQuery(struct student *p);     // 查询学生成绩
void scoreBrowse(struct student *p);    // 浏览学生成绩
void scoreDelete(struct student *p);    // 删除学生成绩
void printRecord(struct student *p);  // 打印学生成绩
int readScores(struct student *p);    // 读取学生成绩
int main() {
}
