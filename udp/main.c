#include <Windows.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>


char ip[] = "0.0.0.0";
int port = 5678;

//open serial port
/**********************************************************************
 * 功能描述： 打开串口
 * 输入参数：	const char *com：端口号
 *          int baud：波特率
 * 返回值：	HANDLE:端口句柄
 * 链接：
 *   https://liang.blog.csdn.net/article/details/125984648
 */
HANDLE open_serial_port(const char *com, int baud);

//send udp
/**********************************************************************
 * 功能描述： 向给定ip和端口号发送udp字符串
 * 输入参数：	char *ip:ip地址
 * 			int port:端口号
 * 			char* str:发送的字符串
 * 返回值：	int:发送结果
 * 链接：
 *   https://blog.csdn.net/sinat_16643223/article/details/119177283
 *   https://blog.csdn.net/lllkey/article/details/80435802
 ***********************************************************************/
int send_udp_str(char *, int, char *);


int main(void) {
    BOOL b = FALSE;
    DWORD wRLen = 0;
    DWORD wWLen = 0;
    char buf[1024] = {0};

    //这段不加 socket创建必失败 别问↓
    WORD wVersionRequested;
    WSADATA wsaData;
    // Use the MAKEWORD(lowbyte, highbyte) macro declared in Windef.h
    wVersionRequested = MAKEWORD(2, 2);
    int err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0) {
        printf("WSAStartup failed with error: %d\n", err);
        return 1;
    }
    //这段不加 socket创建必失败 别问↑

    HANDLE HCOM = open_serial_port("COM3", 9600);
    if (HCOM == INVALID_HANDLE_VALUE) {
        printf("!!!OpenSerial COM fail!!!\n\n");
        return -1;
    }
    printf("===Open COM Success===\n\n");

    //loop read
    while (1) {
        wRLen = 0;
        //读串口消息
        b = ReadFile(HCOM, buf, sizeof(buf) - 1, &wRLen, NULL);
        if (b && 0 < wRLen) {//读成功并且数据大小大于0
            buf[wRLen] = '\0';
            printf("%s", buf);//打印收到的数据
            send_udp_str(ip, port, buf);
        }
        Sleep(100);
    }
}

HANDLE open_serial_port(const char *com, int baud) {
    DCB dcb;
    BOOL b = FALSE;
    COMMTIMEOUTS CommTimeouts;
    HANDLE comHandle = INVALID_HANDLE_VALUE;

    //打开串口
    comHandle = CreateFile(com,//串口号
                           GENERIC_READ, //允许读
                           0, //指定共享属性，由于串口不能共享，所以该参数必须为0
                           NULL,
                           OPEN_EXISTING, //打开而不是创建
                           FILE_ATTRIBUTE_NORMAL, //属性描述，该值为FILE_FLAG_OVERLAPPED，表示使用异步I/O，该参数为0，表示同步I/O操作
                           NULL);

    if (INVALID_HANDLE_VALUE == comHandle) {
        printf("CreateFile fail\n");
        return comHandle;
    }

    // 设置读写缓存大小
    b = SetupComm(comHandle, 1024, 1024);
    if (!b) {
        printf("SetupComm fail\n");
    }

    //设定读写超时
    CommTimeouts.ReadIntervalTimeout = MAXDWORD;//读间隔超时
    CommTimeouts.ReadTotalTimeoutMultiplier = 0;//读时间系数
    CommTimeouts.ReadTotalTimeoutConstant = 0;//读时间常量
    CommTimeouts.WriteTotalTimeoutMultiplier = 1;//写时间系数
    CommTimeouts.WriteTotalTimeoutConstant = 1;//写时间常量
    b = SetCommTimeouts(comHandle, &CommTimeouts); //设置超时
    if (!b) {
        printf("SetCommTimeouts fail\n");
    }

    //设置串口状态属性
    GetCommState(comHandle, &dcb);//获取当前
    dcb.BaudRate = baud; //波特率
    dcb.ByteSize = 8; //每个字节有位数
    dcb.Parity = NOPARITY; //无奇偶校验位
    dcb.StopBits = ONESTOPBIT; //一个停止位
    b = SetCommState(comHandle, &dcb);//设置
    if (!b) {
        printf("SetCommState fail\n");
    }

    return comHandle;
}


int send_udp_str(char *ip, int port, char *str) {
    int ret = 0;


    //使用socket()函数获取一个socket文件描述符
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    //创建目标结构体(ip,port,ect)
    struct sockaddr_in server;
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = inet_addr(ip);

    //将套接字和IP、端口绑定
    if (bind(sockfd, (struct sockaddr *) &server, sizeof(server)) < 0) {
        printf("bind error\n");
        perror("bindERR");
        ret = -1;
        return ret;
    }

    //socket文件描述符返回值为-1，socket创建失败
    if (sockfd < 0) {
        printf("---socket creat error---\n");
        perror("socketcreatERR");
        ret = -1;
        return ret;
    }


    //调用sendto()函数发送数据
    int sendstate = sendto(sockfd, str, strlen(str), 0, (struct sockaddr *) &server,
                           sizeof(server));
    //sendto()函数返回值为-1，发送失败
    if (sendstate < 0) {
        printf("---sendto error---\n");
        perror("sendtoERR");
        ret = -2;
    }

    close(sockfd);
    return ret;
}
