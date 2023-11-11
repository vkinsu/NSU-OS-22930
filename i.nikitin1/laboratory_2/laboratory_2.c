#include <sys/types.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
extern char *tzname[];

main()
{
    // получение текущей даты в секундах
    time_t now_in_seconds = time(NULL);
    // текущая дата, представленная в нормальной форме
    struct tm* time_normal = localtime(&now_in_seconds);
    // отображение часового пояса (в нашем случае +7 часов по Новосибирску)
    printf("Текущий часовой пояс и дата (%d): ", atoi(tzname[0]));
    // перевод в 'читаемую' дату 
    printf("%s", ctime(&now_in_seconds));

    // смещаем на 7 часов время часового пояса до времени UTC
    now_in_seconds -= atoi(tzname[0])*3600;
    // смещаем на 7 часов время часового пояса до времени Калифорнии
    now_in_seconds -= 7*3600;
    // смещаем на 1 час в случае если стоит летнее время
    now_in_seconds -= (time_normal->tm_isdst)*3600;
    //printf("%s", ctime(&now_in_seconds));

    printf("Время в Калифорнии (%d): ", (-7 - time_normal->tm_isdst));
    printf("%s", ctime(&now_in_seconds) );
    // time_t now;
    // struct tm *sp;
    // (void) time( &now );
    // printf("%s", ctime( &now ) );
    // sp = localtime(&now);
    // printf("%d/%d/%02d %d:%02d %s\n",
    //     sp->tm_mon + 1, sp->tm_mday,
    //     sp->tm_year, sp->tm_hour,
    //     sp->tm_min, tzname[sp->tm_isdst]);
    exit(0);
}