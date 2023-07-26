#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<string>
#include<iostream>
#include<ctime>
#include <bitset>
#include <cmath>
#include <sstream>

#include"md5.cpp"

using namespace std;
 
#define PORT 13334   /*listen port*/
#define MAXSIZE 32
#define MAXDATASIZE 1000

// string private_key;
// string public_key;
MD5 md5;
int state;
const static int Key_SIZE = 16;
const static int ShiftTable_SIZE = 16;
const static int PC_2_SIZE = 48;
const static int IP_SIZE = 64;
const static int E_SIZE = 48;
const static int P_SIZE = 32;
const static int IP_1_SIZE = 64;
const static int ExchangeRules_SIZE = 56;
const static string Bin_Hex[16]=
{
    "0","1","2","3","4","5","6","7","8","9","A","B","C","D","E","F"
};
//交换规则表（8*7）
    
    int ExchangeRules[56] =
{
    57, 49, 41, 33, 25, 17,  9,
    1, 58, 50, 42, 34, 26, 18,
    10,  2, 59, 51, 43, 35, 27,
    19, 11,  3, 60, 52, 44, 36,
    63, 55, 47, 39, 31, 23, 15,
    7, 62, 54, 46, 38, 30, 22,
    14,  6, 61, 53, 45, 37, 29,
    21, 13,  5, 28, 20, 12,  4
};

//移位表

int ShiftTable[16] =
{
    1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1
};

//PC-2（8*6）

int PC_2[48] =
{
    14, 17, 11, 24,  1,  5,
    3, 28, 15,  6, 21, 10,
    23, 19, 12,  4, 26,  8,
    16,  7, 27, 20, 13,  2,
    41, 52, 31, 37, 47, 55,
    30, 40, 51, 45, 33, 48,
    44, 49, 39, 56, 34, 53,
    46, 42, 50, 36, 29, 32
};

//IP（8*8）

int IP[64] =
{
    58, 50, 42, 34, 26, 18, 10,  2,
    60, 52, 44, 36, 28, 20, 12,  4,
    62, 54, 46, 38, 30, 22, 14,  6,
    64, 56, 48, 40, 32, 24, 16,  8,
    57, 49, 41, 33, 25, 17,  9,  1,
    59, 51, 43, 35, 27, 19, 11,  3,
    61, 53, 45, 37, 29, 21, 13,  5,
    63, 55, 47, 39, 31, 23, 15,  7
};

//扩展置换E（8*6）

int E[48] =
{
    32,  1,  2,  3,  4,  5,
    4,  5,  6,  7,  8,  9,
    8,  9, 10, 11, 12, 13,
    12, 13, 14, 15, 16, 17,
    16, 17, 18, 19, 20, 21,
    20, 21, 22, 23, 24, 25,
    24, 25, 26, 27, 28, 29,
    28, 29, 30, 31, 32,  1
};

//S盒
int SBox[8][4][16] =
{
    {
        {14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7},
        {0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8},
        {4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0},
        {15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13}
    },
    {
        {15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10},
        {3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5},
        {0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15},
        {13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9}
    },
    {
        {10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8},
        {13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1},
        {13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7},
        {1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12}
    },
    {
        {7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15},
        {13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9},
        {10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4},
        {3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14}
    },
    {
        {2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9},
        {14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6},
        {4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14},
        {11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3}
    },
    {
        {12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11},
        {10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8},
        {9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6},
        {4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13}
    },
    {
        {4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1},
        {13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6},
        {1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2},
        {6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12}
    },
    {
        {13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7},
        {1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2},
        {7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8},
        {2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11}
    }
};

//P盒（8*4）

int P[32] =
{
    16,  7, 20, 21,
    29, 12, 28, 17,
    1, 15, 23, 26,
    5, 18, 31, 10,
    2,  8, 24, 14,
    32, 27,  3,  9,
    19, 13, 30,  6,
    22, 11,  4, 25
};

//IP-1（8*8）

int IP_1[64] =
{
    40, 8, 48, 16, 56, 24, 64, 32,
    39, 7, 47, 15, 55, 23, 63, 31,
    38, 6, 46, 14, 54, 22, 62, 30,
    37, 5, 45, 13, 53, 21, 61, 29,
    36, 4, 44, 12, 52, 20, 60, 28,
    35, 3, 43, 11, 51, 19, 59, 27,
    34, 2, 42, 10, 50, 18, 58, 26,
    33, 1, 41,  9, 49, 17, 57, 25
};

class DES{
public:
    

    /***int转四位string  +  int十进制转string二进制**/
    string int2BinString(int n)
    {
        bitset<4> bit(n);
        return bit.to_string();
    }
    /***string十六进制转string二进制**/
    string hexToTwo(string str)
    {
        string twoBin = "";
        int i;
        for(i=0; i<16; i++)
        {
            if(str[i]>='0'&&str[i]<='9')
                twoBin.append(int2BinString(str[i]));
            else if(str[i]>='A'&&str[i]<='Z')
                twoBin.append(int2BinString(str[i]-'A'+10));
            else if(str[i]>='a'&&str[i]<='z')
                twoBin.append(int2BinString(str[i]-'a'+10));
    
        }
        return twoBin;
    }
    /*** string二进制转int十进制**/
    int binToDec(string bin)
    {
        int sum = 0;
        for(int i=0; i<bin.size(); i++)
        {
            if(bin[i]=='0' || bin[i]=='1')
            {
                sum += (bin[i]-'0') * pow(2, bin.size()-i-1);
            }
            else
            {
                cout<<"非法二进制字符！"<<endl;
                return 0;
            }
        }
        return sum;
    }
 
    /** * 01字符转十进制**/
    int str2Dec(string str)
    {
        bitset<64> bst(str);
        return (int)bst.to_ulong();
    }

    /*** 64位密文转十六进制**/
    //Bin2Hex转换表
    string Bin2Hex(string strBin)
    {
        string hex;
        int a = strBin.length()/4;
        string trans;
        for(int i = 0; i < a; i++)
        {
            trans.clear();
            trans = strBin.substr(i*4, 4);
            hex += Bin_Hex[str2Dec(trans)];
        }
        return hex;
    }
    
    /***利用交换表进行置换**/
    string exchange(string str, int rule[], int x)
    {
        string exchangedStr = "";
        int i, temp;
        for(i=0; i<x; i++)
        {
            temp = rule[i]-1;
            exchangedStr.append(1, str[temp]);
        }
        return exchangedStr;
    }
    
    /***依据移位表进行移位**/
    string circleMove(string str, int j)
    {
    
        string targetString = "";
        targetString.append(str.substr(j));
        targetString.append(str.substr(0, j));
        return targetString;
    }

    /***左右两部分移位**/
    string spiltShift(string str, int j)
    {
        string targetStr = "";
        string leftString = str.substr(0, str.length()/2);
        string rightString = str.substr(str.length()/2);
        targetStr.append(circleMove(leftString, j));
        targetStr.append(circleMove(rightString, j));
        return targetStr;
    }
    /*** string 异或**/
    string XOR(string str1, string str2)
    {
        string targetString = "";
        for(int j=0; j<str1.length(); j++)
        {
            targetString += ((str1[j] - '0') ^ (str2[j] - '0')) + '0';
    
        }
        return targetString;
    }
    /*** S盒工作**/
    string SBoxWork(string str, int SBox[][4][16])
    {
        string targetString = "";
        string temp = "";
        string x = "", y = "";
        int col = 0, row = 0;
        for(int i=0; i<str.size()/6; i++)
        {
            temp = str.substr(6*i, 6);
            x = temp.substr(0, 1)+temp.substr(5, 1);
            y = temp.substr(1, 4);
            row = binToDec(x);
            col = binToDec(y);
            targetString.append(int2BinString(SBox[i][row][col]));
        }
        return targetString;
    }

    //加密
    string encryption(string MingWen,string Key)
    {
        string M = hexToTwo(MingWen);
        string K = hexToTwo(Key);
    
        /***处理密钥，生成16个子密钥 **/
        /* 利用规则交换表（8*7）将K转换成 K0 ;  K0(56位) = C0(28位) + D0(28位) */
        string KKK = exchange(K, ExchangeRules, ExchangeRules_SIZE);
    
        /* 利用移位表转换得C1D1----C16D16,存入K_arr */
        int i = 0;
        // string K_arr[Key_SIZE];
        string K_arr[Key_SIZE+1];
        K_arr[0] = KKK;
        for(i=1; i<=Key_SIZE; i++)
        {
            K_arr[i] = spiltShift(K_arr[i-1], ShiftTable[i-1]);
        }
        /* Kn（48位）= PC-2（8*6）处理 CnDn得16个子密钥，存入Key_arr */
    
        string Key_arr[Key_SIZE];
        for(i=0; i<Key_SIZE; i++)
        {
            Key_arr[i] = exchange(K_arr[i+1], PC_2, PC_2_SIZE);
        }
        /*** 用子密钥对明文加密**/
        /* 通过IP（8*8）处理M得L0（32位）  R0（32位） */
        string IP_M = exchange(M, IP, IP_SIZE);
    
        /* Ln= R(n-1); Rn= L(n-1) + f(R(n- 1); Kn)迭代16次 */
        string L[Key_SIZE+1];
        string R[Key_SIZE+1];
        L[0] = IP_M.substr(0, M.length()/2);
        R[0] = IP_M.substr(M.length()/2);
        string it = "";
        for(i=1; i<=Key_SIZE; i++){
            it = exchange(R[i-1], E, E_SIZE);//将R0通过扩展置换E（8*6）从32位扩展到48位
            it = XOR(it, Key_arr[i-1]);//R0（48位）与 K1异或得E0（48位）           
            it = SBoxWork(it, SBox);//将E0（48位）通过S盒转换成32位              
            it = exchange(it, P, P_SIZE);//P盒（8*4）置换，得P0              
            it = XOR(it, L[i-1]);//P0与L0进行异或，得J0              
            L[i] = R[i-1];//左右交换位置，即R1 = J0; L1 = R0
            R[i] = it;
        }
        /* 对R16 L16进行一次IP-1（8*8）排序得密文 */
        string res = "";
        res += R[16];
        res += L[16];
        string finalRes = Bin2Hex(exchange(res, IP_1, IP_1_SIZE));
        return finalRes;
    }


    /***解密**/
    string decryption( string MiWen,string Key)
    {
        string M = hexToTwo(MiWen);
        string K = hexToTwo(Key);
    
        /***处理密钥，生成16个子密钥 **/
        /* 利用规则交换表（8*7）将K转换成 K0 ; K0(56位) = C0(28位) + D0(28位) */
        string KKK = exchange(K, ExchangeRules, ExchangeRules_SIZE);
        /* 利用移位表转换得C1D1----C16D16,存入K_arr */
        int i = 0;
        string K_arr[Key_SIZE+1];
        K_arr[0] = KKK;
        for(i=1; i<=Key_SIZE; i++)
        {
            K_arr[i] = spiltShift(K_arr[i-1], ShiftTable[i-1]);
        }
        /* Kn（48位）= PC-2（8*6）处理 CnDn得16个子密钥，存入Key_arr */
        string Key_arr[Key_SIZE];
        for(i=0; i<Key_SIZE; i++)
        {
            Key_arr[i] = exchange(K_arr[i+1], PC_2, PC_2_SIZE);
        }
        /*** 用子密钥对明文加密**/
        /* 通过IP（8*8）处理M得L0（32位）  R0（32位） */
        string IP_M = exchange(M, IP, IP_SIZE);
        /* Ln= R(n-1); Rn= L(n-1) + f(R(n- 1); Kn)迭代16次 */
        string L[Key_SIZE+1];
        string R[Key_SIZE+1];
        L[0] = IP_M.substr(0, M.length()/2);
        R[0] = IP_M.substr(M.length()/2);
        string it = "";
        for(i=1; i<=Key_SIZE; i++)
        {                
            it = exchange(R[i-1], E, E_SIZE);//将R0通过扩展置换E（8*6）从32位扩展到48位               
            it = XOR(it, Key_arr[16-i]); //R0（48位）与 K1异或得E0（48位）                
            it = SBoxWork(it, SBox);//将E0（48位）通过S盒转换成32位              
            it = exchange(it, P, P_SIZE);//P盒（8*4）置换，得P0             
            it = XOR(it, L[i-1]); //P0与L0进行异或，得J0               
            L[i] = R[i-1];
            R[i] = it;//左右交换位置，即R1 = J0; L1 = R0
        }   
        /* 对R16 L16进行一次IP-1（8*8）排序得密文 */
        string res = "";
        res += R[16];
        res += L[16];
        string finalRes = Bin2Hex(exchange(res, IP_1, IP_1_SIZE));
        return finalRes;           
    }

    string get_des_key(){
        int i;
        string res="";
        srand((unsigned)time(NULL));
        for(i=0;i<16;i++){
            res=res+Bin_Hex[rand()%16];
        }
        return res;           
    }
};

class RSA{
public:
    string int_to_string(int x){
        string str;
        stringstream ss;
        ss << x;
        ss >> str;
        return str;
    }

//加密
    string Encode(int e,int n,string plaintext) {
        string ciphertext_comma="";
        int flag = 1;
        for (int i = 0; i < plaintext.size(); i++) {
            for (int j = 0; j < e; j++) {
                flag = flag * (int)plaintext.at(i) % n;
            }
            //ciphertext=ciphertext+to_string(flag);
            if(i!=0){
                ciphertext_comma=ciphertext_comma+","+to_string(flag);
            }else{
                ciphertext_comma=ciphertext_comma+to_string(flag);
            }
            flag = 1;
        }
        return ciphertext_comma;
    }
};


void login(int &sockfd,string des_key){
    DES des;
    cout<<"please log in first..."<<endl;
    cout<<"**************** Login Interface ********************"<<endl;

    // char username_log[MAXDATASIZE];
    // char password_log[MAXDATASIZE];
    string username_log;
    string password_log;
    cout<<"please input the username: ";
    // fgets(username_log,sizeof(username_log),stdin);
    // write(sockfd,username_log,strlen(username_log));
    cin>>username_log;
    
    send(sockfd,username_log.c_str(),username_log.size(),0);
    cout<<"username_log: "<<username_log<<endl;
    write(sockfd,des.encryption(md5.get_md5(username_log),des_key).c_str(),des.encryption(md5.get_md5(username_log),des_key).size());
    cout<<endl;
    cout<<"please input the password: ";
    // fgets(password_log,sizeof(password_log),stdin);
    // write(sockfd,password_log,strlen(password_log));
    cin>>password_log;
    
    send(sockfd,password_log.c_str(),password_log.size(),0);  
    cout<<"password_log: "<<password_log<<endl;
    write(sockfd,des.encryption(md5.get_md5(password_log),des_key).c_str(),des.encryption(md5.get_md5(password_log),des_key).size());
    cout<<endl;

    
    cout<<"encry_username: "<<des.encryption(md5.get_md5(username_log),des_key)<<endl;
    cout<<"encry_passoword: "<<des.encryption(md5.get_md5(password_log),des_key)<<endl;
    cout<<"md5_username: "<<md5.get_md5(username_log)<<endl;
    cout<<"md5_password: "<<md5.get_md5(password_log)<<endl;
    cout<<"decry_username: "<<des.decryption(des.encryption(md5.get_md5(username_log),des_key),des_key)<<endl;
    cout<<"decry_password: "<<des.decryption(des.encryption(md5.get_md5(password_log),des_key),des_key)<<endl;

    cout<<"waiting for the Certification from server......"<<endl;
    int response_login_from_server_len;
    char response_login_from_server[MAXDATASIZE];
    response_login_from_server_len=read(sockfd,response_login_from_server,sizeof(response_login_from_server));
    response_login_from_server[response_login_from_server_len]='\0';

    if(strcmp(response_login_from_server,"200")==0){
        cout<<"login successfully!"<<endl;
        state=1;
    }else{
        cout<<"fail to login!"<<endl;
        state=2;
    }

    // printf("send msg to server: \n");
    // fgets(message_to_client,sizeof(message_to_client),stdin);//用户输入的所有东西都读到buf里
    // string s="hello linux!";
    // write(sockfd,s.c_str(),s.size());
}

void regist(int &sockfd,string des_key){
    DES des;
    cout<<"**************** Register Interface ********************"<<endl;
    // char username_regis[MAXDATASIZE];
    // char password_regis[MAXDATASIZE];
    string username_regis;
    string password_regis;

    cout<<"please input the username: ";
    // fgets(username_regis,sizeof(username_regis),stdin);
    // write(sockfd,username_regis,strlen(username_regis));
    cin>>username_regis;

    // des.encryption(md5.get_md5(username_regis),des_key);

    write(sockfd,des.encryption(md5.get_md5(username_regis),des_key).c_str(),des.encryption(md5.get_md5(username_regis),des_key).size());
    cout<<endl;
    cout<<"please input the password: ";
    // fgets(password_regis,sizeof(password_regis),stdin);
    // write(sockfd,password_regis,strlen(password_regis));
    cin>>password_regis;
    
    // des.encryption(md5.get_md5(password_regis),des_key);

    write(sockfd,des.encryption(md5.get_md5(password_regis),des_key).c_str(),des.encryption(md5.get_md5(password_regis),des_key).size());
    cout<<endl;
    cout<<"register account successfully! "<<endl;
    state=0;
}

void tips(int &sockfd,string des_key){
    cout<<"The system detected that this host is not logged in! "<<endl;
    
    cout<<"If you do not have account,you cannot use any function!"<<endl;
    cout<<"Press 0 to register,or press 1 to login: "<<endl;
    char choice[MAXDATASIZE];
    fgets(choice,sizeof(choice),stdin);
    write(sockfd,choice,strlen(choice));


    switch(choice[0]){
        case '0':
            regist(sockfd,des_key);
            break;
        case '1':
            login(sockfd,des_key);
            break;
        default:
            break;
    }
}


int main ( int argc, char *argv[]){
    time_t t;
    t=time(NULL);
    cout<<"Current Time: "<<ctime(&t)<<endl;
    cout<<"The client is connecting to the server..."<<endl;
    int sockfd,num;
    struct hostent *he;
    struct sockaddr_in server;
 
	char buf[MAXSIZE];
    
    // int private_len;
    int public_len;
    int des_key_len;

    // char private_key[MAXDATASIZE];
    char rsa_public_key[MAXDATASIZE];
    string des_key;
    string dir;

    if(argc!=2){
        printf("please in put the ip address! \n",argv[0]);
        return 0;
    }
    /*get ip address*/
    if((he = gethostbyname(argv[1])) == NULL){
        herror("gethostbyname error !\n");
        return 0;
    }
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        perror("socket() error !\n");
        return 0;
    }

    int opt = SO_REUSEADDR;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    // cout<<"before"<<endl;
    server.sin_addr = *((struct in_addr *)he->h_addr_list[0]);
    // cout<<"after"<<endl;

    if(connect(sockfd, (struct sockaddr *)&server, sizeof(server)) == -1){
        perror("connect() error! \n");
        return 0;
    }

    cout<<"connect success!"<<endl;

    if((public_len=recv(sockfd,rsa_public_key,MAXDATASIZE,0))==-1){
            perror("failed to accept RSA: public_key!");
            cout<<"failed to accept RSA: public_key!"<<endl;
        }else{
            cout<<"Accept RSA: public_key successfully!"<<endl;
        }
        // private_key[private_len]='\0';
        rsa_public_key[public_len]='\0';

        // cout<<"private_key: "<<private_key<<endl;
        cout<<"RSA public_key: "<<rsa_public_key<<endl;
        string e;
        string n;
        int index=0;
        bool flag=true;
        while(rsa_public_key[index]!='\0'){
            if(rsa_public_key[index]==','){
                flag=false;
                index++;
                continue;
            }
            if(flag){
                e=e+rsa_public_key[index];
            }else{
                n=n+rsa_public_key[index];
            }
            index++;
        }
        cout<<"e: "<<e<<endl;
        cout<<"n: "<<n<<endl;

        RSA rsa;
        
        DES des;
        des_key=des.get_des_key();
        cout<<"DES key: "<<des_key<<endl;
        string encrypted_des_key=rsa.Encode(atoi(e.c_str()),atoi(n.c_str()),des_key);

        if((des_key_len=write(sockfd,encrypted_des_key.c_str(),encrypted_des_key.size()))==-1){
            perror("failed to send DES: key!");
            cout<<"failed to send DES: key!"<<endl;
        }else{
            cout<<"Send DES: key successfully!"<<endl;
        }     


    tips(sockfd,des_key);

    if(state==1){      
            char res[MAXSIZE];
            //file backup
            cout<<"Please input file's directory: "<<endl;
            cin>>dir;
            FILE *fd= fopen(dir.c_str(),"r+"); 
            if(fd==NULL){
                cout<<"The refered file didn't exist!"<<endl;
                strcpy(res,"404");
                cout<<"response: "<<res<<endl;
            }else{
                strcpy(res,"200");
                cout<<"response: "<<res<<endl;
            }
            write(sockfd,res,strlen(res));
            
            if(strcmp(res,"200")==0){
                while((num=fread(buf,1,MAXSIZE,fd))>0){
                    send(sockfd,buf,num,0);
                }            
                cout<<"The file specified by the user has been sent to the server!"<<endl;
                cout<<"please check in about 10 seconds, or later..."<<endl;
                fclose(fd);
            }else{
                cout<<"The refered file didn't exist.Program terminated!"<<endl;
            }
    }else{
        if(state==0){
            cout<<"Congratulations, you have completed the registration, restart the client to use the file backup function!"<<endl;
        }
        if(state==2){
            cout<<"Login failed, please restart the client and try again!"<<endl;
        }
    }



    
    close(sockfd);
	return 1;
}

