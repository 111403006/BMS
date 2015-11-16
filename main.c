/*  <Bank Management>  Copyright (C) <2015>  <Rohit Chaudhari>	<rc88413@gmail.com> */


/*This file is part of Bank Management.

    Bank Management is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Bank Management is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Bank Management.  If not, see <http://www.gnu.org/licenses/>.*/



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

void add_ac();
void deposit();
void withdraw();
void fund_transfer();
void transaction();
void view_log();
void ac_info();

void add_user();
void del_user();
void edit_user();

void _strdate(char *str);
void _strtime(char *str);
void date_ip(char date[]); 
void date_op(char date[]); 
int date_check(char date[]);

void passwdhide(char buffer[]);
void increase(char i_id[]);
void currency(char cur[], float n); 
void numtowrd(double n,char word[]);
void ones(int n, char one[]); 
void tens(int n,char ten[]); 
void title();
void intro();
void admin();
void staff();
void update();
char date[13],ttime[10]; 

FILE *fp,*fp1,*tfp;

struct
    {
        char name[30], password[30], uid[4];
    } user;

struct account
    {
        char ac_no[8];
        char fname[30], lname[30];//first name and last name
        char u_date[15];
        char dob[12];
        char sex;
        char address[50],contact[20];
        char ac_type;
        float c_bal,interest,t_bal;
    };

struct trans
    {
        char ac_no[8],ac_t[25],_date[15],_ttime[10],usr[30];
        float amt;
    };

typedef struct
{
    char id[4];
    char name[30];
    char date[15]; //Log in date
    char intime[10];// Log in time
    char outtime[10];// Log out or exit time
}user_log;

#define DASH printf("\n\n") ; for(i = 0; i < 78; i++) printf("_"); printf("\n\n")
#define NEWLINE printf("\n\n");

int main()
{
    char c;
    intro();
    do
    {
        system("clear"); //Clears the screen
        printf("Press A to Log in as ADMINISTRATOR or S to log in as STAFF and esc to exit the program\n\n\n\t\t\t\t\t");
        c = getchar(); 

        if(c == 'A' || c == 'a')
        {
            strcpy(user.name,"Admin");
            admin();
            break;
        }
        if (c=='S'||c=='s')
        {
            staff();
            break;
        }
        if (c==27)
            exit(0); //27 is ASCII code of escape key

    }while(1); 

    return 0;
}

//_________________________________________________________________________________________________________________

void staff()
{
    int i,ch;
//Log in Screen begins...

    char uname[30], pass[30],passwrd[30]={0},id[10],nam[30];
    int c=0 /*Counts the no. of user for which the name and password matches*/ ,count=0; /*count counts the number of attempts of Log in*/
    char ex,intime[9],outtime[9];
    do
    {
        system("clear");
		printf("Only THREE attempts would be allowed to enter username and password.\n\n");
        printf("Enter User name : ");
        scanf("%s",user.name);
        printf("Password        : ");
        scanf("%s", passwrd);
        strcpy(user.password,passwrd);
        count++;

        if (count==3)
        {
            title();
            printf("Invalid User name or Password!\\n");
            printf("Press ENTER to exit the program...\n\n");
            getchar()/**/;
            exit(0);
        }

        c=0;//
        fp=fopen("USER.DAT","r");
        while(fscanf(fp,"%s %s %s\n",id,nam,pass) != EOF)
        {
             /*strcpy(nam,strupr(nam));
             strcpy(pass,strupr(pass));
             strcpy(user.name,strupr(user.name));
             strcpy(user.password,strupr(user.password));*/

             if((strcmp(user.name,nam) == 0) && (strcmp(user.password,pass) == 0)) 
             {
                 c++;
                 strcpy(user.uid,id);
             }
         }

        fclose(fp);

        title();
        if (c==0)// when no records are found c=0
        {
			printf("Cannot find the given combination of USER NAME and PASSWORD!\n\n");
            getchar()/**/;
        }
        else break; 

    } while(1);//not exactly infinite as user is prompted only maximum three times 

    _strtime(intime);


    update(); 
    do
    {
        title();

        printf("1. Create New Account\n");
        printf("2. Cash Deposit\n");
        printf("3. Cash Withdraw\n");
        printf("4. Fund Transfer\n");
        printf("5. Account information\n");
        printf("6. Transaction information\n");
        printf("7. Log out\n");
        printf("8. Exit\n");

		DASH;

        printf("Press a choice between the range [1-8] \n");

        switch(getchar()/**/ - 48)
        {
            case 1:
                add_ac();

            break;

            case 2:
                deposit();

            break;

            case 3:
                withdraw();

            break;

            case 4:
                fund_transfer();

            break;

            case 5:
                ac_info();

            break;

            case 6:
                transaction();
            break;

            case 7:
                title();
                printf("Are you sure you want to Log out? <Y/N> : \n");
                ex=getchar();
                if (ex=='Y'||ex=='y')
                {
                    
                    _strtime(outtime);
                    fp=fopen("LOG.DAT","a");
                    fprintf(fp,"%s %s %s %s\n",user.uid,date,intime,outtime);
                    fclose(fp);
                    staff();
                }
            break;

            case 8:
                title();
                printf("Are you sure you want to exit? <Y/N> : \n\n");
                ex=getchar();
                if (ex=='Y'||ex=='y')
                {
                    
                    _strtime(outtime);
                    fp=fopen("LOG.DAT","a");
                    fprintf(fp,"%s %s %s %s\n",user.uid,date,intime,outtime);
                    fclose(fp);
                    exit(0);
                }
            break;

            default:
                title();
                printf("Your input is out of range! Enter a choice between 1 to 8!\n");
                printf("Press any key to return to main menu...\n");
                getchar()/**/;
        }
    }while(1);
 }

//---------------------------------------------------------------------------------------------------


void update()
{
    struct account ac;
    int i,numyr,nummonth,numdays,n1,n2;
    float r;
    fp=fopen("ACCOUNT.DAT","r");

    if (fp!=NULL)
    {
        fp1=fopen("TEMP.DAT","w");
        i=0;
        while(fscanf(fp,"%s %s %s %s %s %s %c %s %c %f %f %f\n",ac.ac_no,ac.fname,ac.lname,ac.dob,ac.address,ac.contact,&ac.sex,ac.u_date,&ac.ac_type,&ac.c_bal,&ac.interest,&ac.t_bal)!=EOF)
        {
            
            numyr    = (date[6] - 48) * 10 + (date[7] - 48);
            nummonth = (date[0] - 48) * 10 + (date[1] - 48);
            numdays  = (date[3] - 48) * 10 + (date[4] - 48);

            n1  = numyr * 365 + nummonth * 30 + numdays;   
            numyr    = (ac.u_date[6] - 48) * 10 + (ac.u_date[7] - 48);
            nummonth = (ac.u_date[0] - 48) *10 + (ac.u_date[1] - 48);
            numdays  = (ac.u_date[3] - 48) * 10 + (ac.u_date[4] - 48);
            n2  = numyr * 365 + nummonth * 30 + numdays;//n2 is no. of days elasped on last updated date

            if (ac.ac_type == 'S' || ac.ac_type == 's')
                r = 8.0 / 365;
            else
                r = 4.0 / 365;

            ac.t_bal    = ac.c_bal * pow((1 + r / 100), n1 - n2);//calculation of compound interest
            ac.interest += ac.t_bal - ac.c_bal;//calculation and addition of interest
            ac.t_bal    = ac.c_bal + ac.interest;//calculation of total balance
            strcpy(ac.u_date,date);
            fprintf(fp1,"%s %s %s %s %s %s %c %s %c %.2f %.2f %.2f\n",ac.ac_no,ac.fname,ac.lname,ac.dob,ac.address,ac.contact,ac.sex,ac.u_date,ac.ac_type,ac.c_bal,ac.interest,ac.t_bal);

        }
        fclose(fp);
        fclose(fp1);
        system("del ACCOUNT.DAT");
        system("ren TEMP.DAT ACCOUNT.DAT");
    }
}


//----------------------------------------------------------------------------------------------

void admin()
{
    char uname[30], pass[30]={0},ox,key;
	int c=0,count=0,ch,i;
	/*--------------------------TO CHECK ADMIN's PASSWORD-------------------------------*/
    do{
        
        system("clear");
        printf("Only THREE attempts shall be allowed to enter username and password.\n\n");
        printf("Enter User name : "); scanf("%s",uname);
       // char pass[30]={0};
       	//getchar();
        printf("Password        : "); //passwdhide(pass);
        scanf("%s", pass);
        //strcpy(uname,strupr(uname)); //Changes string uname into uppercase
        //strcpy(pass,strupr(pass));
		
        count++;
        if (count==3)
        {
            title();
            printf("Invalid User name or Password!\n");
            printf("Press any key to exit the program...\n");
            getchar()/**/;
            exit(0); //exits the program
        }
        if(strcmp(uname,"Admin")==0 && strcmp(pass,"RAMA") == 0)
            c=1;
        else
        {
            title();
            printf("Invalid User name or Password!\n");
            getchar()/**/;
        }

    } while(c!=1);
    
/*--------------------------HIS OPTONS AVAILABLE-------------------------*/
    do{
        title();
        printf("1. Add User\n");
        printf("2. Delete User\n");
        printf("3. Edit User name / Password\n");
        printf("4. View User Log\n");
        printf("5. Exit\n");
        DASH;
        printf(" Press a number between the range [1 -5]  \n");
        key = getchar();//Reads a character entered by user and stores in char variable key
        ch = key - 48;//changes char key into integer ch eg. '1'(ascii value 49)--->49 - 48=1

        switch(ch)
        {
            case 1:
                add_user();
                break;

            case 2:
                del_user();
                break;

            case 3:
                edit_user();
                break;

            case 4:
                view_log();
                break;

            case 5:
                title();
                printf("Are you sure you want to exit? <Y/N> : \n");
                ox=getchar();
                if (ox == 'Y'||ox == 'y')
                    exit(0);
                break;

            default:
                title();
                printf("Your input is out of range! Enter a choice between 1 to 5!\n");
                printf("Press ENTER to return to main menu...\n");
                getchar()/**/;

        }

    } while(1);
    return;
}


void _strtime(char *str){
	time_t rawtime;
	struct tm *timeinfo;
	//char *str;
	
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	
	strftime(str, sizeof(str), "%H%M%S", timeinfo);

	
}
void _strdate(char *str){
	time_t rawtime;
	struct tm *timeinfo;
	//char *buff;
	
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	
	strftime(str, sizeof(str), "%d%b%y", timeinfo);
	
}


/*char *strupr(char *s){
	int i = 0;
	while(s[i] != '\0' ){
		toupper(s[i++]);
	}
	return s;
}*/

void passwdhide(char buffer[])
{
    char t;
    int psn = 0;
    do
    {
        t = getchar();
        if( isprint(t) )
        {
            buffer[ psn++ ] = t;
            printf("%c", '*');
        }
        else if( t == 8 && psn )
        {
            buffer[ psn-- ] = '\0';
            printf("%s", "\b \b");
        }

    }while( t != 10 );
}

//-------------------------------------------------------------------------
void intro()
{
    system("clear");
    printf("\n\n\t\tBANK MANAGEMENT SYSTEM\n\n");
   
    printf("\t\t\tPress Any key to continue...");
    getchar()/**/;
}
//-------------------------------------------------------------------------


void numtowrd(double n,char word[])
{
    long int num;
    int a[6],i, m,h,tt,t,o;
    num = (int)n;
    a[5] = num % 1000;
    num = num / 1000;

    for(i = 4; i >= 0; i--)
    {
        a[i] = num % 100;
        num = num/100;
    }

    
    m   = a[5];
    h   = m / 100;
    tt  = m % 100;
    t   = tt / 10;
    o   = tt % 10;
    char one[10],ten[10],hh[20]={0};

    if(tt < 20 && h != 0)
    {
        ones(h,one);
        strcpy(hh,one);
        strcat(hh,"Hundred ");
        ones(tt,one);
        strcat(hh,one);
    }
    else if(tt < 20 && h == 0)
    {
        ones(tt,one);
        strcpy(hh,one);
    }
    else if(tt > 20 && h != 0)
    {
        ones(h,one);
        strcpy(hh,one);
        strcat(hh,"Hundred ");
        tens(t,ten);
        strcat(hh,ten);
        ones(o,one);
        strcat(hh,one);
    }
    else
    {
        tens(t,ten);
        strcpy(hh,ten);
        ones(o,one);
        strcat(hh,one);
    }
//ok
    char aa[5][30];
    for(i=4;i>=0;i--)
    {
        if (a[i]<20)
        {
            ones(a[i],one);
            strcpy(aa[i],one);
        }
        else
        {
            tens(a[i]/10,ten);
            strcpy(aa[i],ten);
            ones(a[i]%10,one);
            strcat(aa[i],one);
        }
    }

    char x[10]={0},w[50]={0};
    int l;
    for(i=0;i<5;i++)
    {
        l=strlen(aa[i]);
        if(l!=0)
        {
            switch(i)
            {
                case 0: strcpy(x,"Kharab "); break;
                case 1: strcpy(x,"Arab "); break;
                case 2: strcpy(x,"Crore "); break;
                case 3: strcpy(x,"Lakh "); break;
                case 4: strcpy(x,"Thousand "); break;
            }
            strcat(w,aa[i]);
            strcat(w,x);
        }
    }
    strcpy(word,w);
    strcat(word,hh);
    word[strlen(word)-1]='\0';
}
//-------------------------------------------------------------
void tens(int n,char ten[])
{
    switch(n)
    {
        case 2: strcpy(ten,"Twenty "); break;
        case 3: strcpy(ten,"Thirty "); break;
        case 4: strcpy(ten,"Forty "); break;
        case 5: strcpy(ten,"Fifty "); break;
        case 6: strcpy(ten,"Sixty "); break;
        case 7: strcpy(ten,"Seventy "); break;
        case 8: strcpy(ten,"Eighty "); break;
        case 9: strcpy(ten,"Ninety "); break;
        default:
        strcpy(ten,"");
    }
}
//---------------------------------------------------------------

void ones(int n, char one[])
{
    switch(n)
    {
        case 1: strcpy(one,"One "); break;
        case 2: strcpy(one,"Two "); break;
        case 3: strcpy(one,"Three "); break;
        case 4: strcpy(one,"Four "); break;
        case 5: strcpy(one,"Five "); break;
        case 6: strcpy(one,"Six "); break;
        case 7: strcpy(one,"Seven "); break;
        case 8: strcpy(one,"Eight "); break;
        case 9: strcpy(one,"Nine "); break;
        case 10: strcpy(one,"Ten "); break;
        case 11: strcpy(one,"Eleven "); break;
        case 12: strcpy(one,"Tweleve "); break;
        case 13: strcpy(one,"Thirteen "); break;
        case 14: strcpy(one,"Fourteen "); break;
        case 15: strcpy(one,"Fifteen "); break;
        case 16: strcpy(one,"Sixteen "); break;
        case 17: strcpy(one,"Seventeen "); break;
        case 18: strcpy(one,"Eighteen "); break;
        case 19: strcpy(one,"Nineteen "); break;
        default:
        strcpy(one,"");
    }
}
//-------------------------------------------------------

void currency(char cur[], float n)
{

    int num,i,x,c;
    char temp[30];
    num=(int)n;
    int a[6];
    a[5]=num%1000;
    num=num/1000;

    for(i=4;i>=0;i--)
    {
        a[i]=num%100;
        num=num/100;
    }

    for(i=0;i<6;i++)
        if (a[i]!=0)
            break;

    x=i;
    c=0;
    for(i=x;i<6;i++)
    {
        if(i==5)
        {
            if (a[i]>=100||i!=x)
                cur[c++]=(a[i]/100)+48;
            if (a[i]%100>=10||i!=x)
                cur[c++]=(a[i]%100)/10+48;
            if (a[i]%100<10&&i==x)
                cur[c++]=48;
            cur[c++]=(a[i]%100)%10+48;
        }
        else
        {
            if (a[i]>=10||i!=x)
                cur[c++]=(a[i]/10)+48;
            cur[c++]=(a[i]%10)+48;
            cur[c++]=',';
        }

    }

    cur[c++]='.';
    num=(n-(int)n)*100;
    cur[c++]=num/10+48;
    cur[c++]=num%10+48;
    cur[c]='\0';
    if (n==0)
        strcpy(cur,"0.00");
    strcpy(temp,"Rs. ");
    strcat(temp,cur);
    strcpy(cur,temp);
}

//-------------------------------------------------------------

void increase(char i_id[])
{
    int l;
    l=strlen(i_id)-1;
    do
    {
        if(i_id[l]!='9')
            i_id[l]+=1;
        else
            i_id[l]='0';
        l--;

    }while(i_id[l+1]=='0');

}
//-------------------------------------------------------------


void title()
{
    int i;
    system("clear");
    printf("\t\t\tBanking Management System");
    if (strcmp(user.name,"Admin") == 0)
        printf("Current User : Admin");
    else
        printf("Current User : %s",user.name);
    printf("\t\t\t\tDate : ");
    
    _strdate(date);
    date_op(date);
    printf("%s",date);
    _strdate(date);

}




void date_ip(char date[]) // dd/mm/yyyy ----> mm/dd/yy
{
    char temp[15];
//for date
    temp[3] = date[0];
    temp[4] = date[1];
//for month
    temp[0] = date[3];
    temp[1] = date[4];
//for year where only last two are taken
    temp[6] = date[8];
    temp[7] = date[9];
//end the string
    temp[8] = '\0';
    temp[2] = temp[5] = '/';

    strcpy(date,temp);
}
//------------------------------------------------------------------------
void date_op(char date[]) //mm/dd/yy ---> dd MMM, yyyy
{
    struct{
        int dd;
        char mm[4];
        int yyyy;
    }_date;

    char temp[15];
    int mm,c,i;
    _date.dd = (date[3] - 48) * 10 + (date[4] - 48);//converts here from string to a number
    _date.yyyy = 2000 + (date[6] - 48) * 10 + (date[7] - 48);

    mm = (date[0] - 48) * 10 + (date[1] - 48);
    switch(mm)
    {
        case 1: strcpy(_date.mm,"Jan"); break;
        case 2: strcpy(_date.mm,"Feb"); break;
        case 3: strcpy(_date.mm,"Mar"); break;
        case 4: strcpy(_date.mm,"Apr"); break;
        case 5: strcpy(_date.mm,"May"); break;
        case 6: strcpy(_date.mm,"Jun"); break;
        case 7: strcpy(_date.mm,"Jul"); break;
        case 8: strcpy(_date.mm,"Aug"); break;
        case 9: strcpy(_date.mm,"Sep"); break;
        case 10: strcpy(_date.mm,"Oct"); break;
        case 11: strcpy(_date.mm,"Nov"); break;
        case 12: strcpy(_date.mm,"Dec"); break;
    }

    temp[0] = (int)(_date.dd / 10) + 48;
    temp[1] = (int)(_date.dd % 10) + 48;
    temp[2] = 32;// for a space
    temp[3] = '\0';
    strcat(temp,_date.mm);
    temp[6] = ',';
    c=0;
    temp[7] = 32;
    for(i=3;i>=0;i--)
    {
        temp[8+c]=(int)(_date.yyyy / pow(10,i))+48;
        c++;
        _date.yyyy %= (int) pow(10,i);
    }
    temp[12] = '\0';
    strcpy(date,temp);
    }
//---------------------------------------------------------------------------------
int date_check(char _date[]) //_date[] is in format dd/mm/yyyy
{
    int err=0,mm,dd,yyyy;
    if(strlen(_date) != 10)
        err++;
    if(_date[2] != '/' && _date[5] != '/')
        err++;
    mm = (_date[3] - 48) * 10 + (_date[4] - 48);
    if (mm > 12)
        err++;
    dd = (_date[0] - 48) * 10 + (_date[1] - 48);

    yyyy = (_date[6] - 48) * 1000 + (_date[7] - 48) * 100 + (_date[8] - 48) * 10 + (_date[9] - 48);

    switch(mm)
    {
        case 1: if (dd>31) err++; break;
        case 2:
            if (dd>28)
                err++;
            if (yyyy%4==0&&dd==29)
                err--;
            break;
        case 3: if (dd>31) err++; break;
        case 4: if (dd>30) err++; break;
        case 5: if (dd>31) err++; break;
        case 6: if (dd>30) err++; break;
        case 7: if (dd>31) err++; break;
        case 8: if (dd>31) err++; break;
        case 9: if (dd>30) err++; break;
        case 10: if (dd>31) err++; break;
        case 11: if (dd>30) err++; break;
        case 12: if (dd>31) err++; break;
    }

    if (err==0)
        return 1;
    else
        return 0;

}


void add_user()
{
        int err1 = 0,c = 0;
        char passwrd1[30] = {0};
        char passwrd2[30] = {0};
        char nam[30],pass[30];
        do{
            title();
            err1 = 0;
            printf("User Name        : ");
            scanf("%s",user.name);
            fp = fopen("USER.DAT","r");//opens the datafile USER.DAT to check if the user name already exits or not

            c = 0;
            while(fscanf(fp,"%s %s %s\n",user.uid,nam,pass) != EOF){
               /* strcpy(nam,strupr(nam));
                strcpy(user.name,strupr(user.name));*/
                if(strcmp(nam,user.name) == 0)
                    c++;//if given username and the username of datafile matches, c is increased by 1
            }
            fclose(fp);
            if(c != 0)//it is neccessary for c to be zero to avoid duplicate user name
            {
                printf("User name already exists! Please enter a new user name.\n");
                getchar()/**/;
                err1++;//this is an error flag
                continue;// skips all the commands below and go for next loop of do..while @line 174
            }
            printf("Password         : ");
            passwdhide(passwrd1);
            strcpy(user.password,passwrd1);
            printf("Confirm Password : ");
            passwdhide(passwrd2);
            if (strcmp(user.password,passwrd2) != 0)
            {
                title();
                printf("Password and confirm password does not match!\a");
                getchar()/**/;
                err1++;
            }
        }while(err1 != 0);
        fp = fopen("USER.DAT","r");//data file again opened to read the last user id
        char id[4];

        if (fp == NULL)
            strcpy(user.uid,"U01");//if USER.DAT does not exists userid is set to first i.e. U01
        else{
            while(fscanf(fp,"%s %s %s\n",user.uid,nam,pass) != EOF);//last user id is read
                increase(user.uid);
        }
        fclose(fp);
        fp = fopen("USER.DAT","a");//data file open to append (add) record
        fprintf(fp,"%s %s %s\n",user.uid,user.name,user.password); //record written in datafile
        fclose(fp);
        title();
        printf("Record ADDED successfully!");
        getchar()/**/;

}

//---------------------------------------------------------------------------------------------------------------

void del_user()
{
    char nam[30], pass[30],ch,id[5],passwrd[30] = {0};
    int c = 0;
    user_log log;
    title();
    printf("User Name  : ");
    scanf("%s",user.name);
    printf("Password  : ");
    passwdhide(passwrd);
    strcpy(user.password,passwrd);

    fp = fopen("USER.DAT","r");
    while(fscanf(fp,"%s %s %s\n",user.uid,nam,pass) != EOF)
    {
       /* strcpy(nam,strupr(nam));
        strcpy(pass,strupr(pass));
        strcpy(user.name,strupr(user.name));
        strcpy(user.password,strupr(user.password));*/
        if(strcmp(user.name,nam) == 0 && strcmp(user.password,pass) == 0)
            c++;
    }

    fclose(fp);
    title();
    if (c == 0)
    {
        printf("Cannot find the given combination of USER NAME and PASSWORD!\a");
        getchar()/**/;
        return;
    }

    printf("Are you sure you want to DELETE this user? <Y/N> : ");
    ch = getchar();
    if(ch == 'Y'||ch == 'y')
    {
        fp = fopen("USER.DAT","r");
        tfp = fopen("temp.dat","a");
        while(fscanf(fp,"%s %s %s\n",user.uid,nam,pass) != EOF)
        {
            /*strcpy(nam,strupr(nam));
            strcpy(pass,strupr(pass));*/
            if(strcmp(user.name,nam) != 0 || strcmp(user.password,pass) != 0)
                fprintf(tfp,"%s %s %s\n",user.uid,nam,pass);
            else
                strcpy(id,user.uid);
        }
        fclose(fp);
        fclose(tfp);
        system("del USER.DAT");
        system("ren temp.dat USER.DAT");

        fp = fopen("LOG.DAT","r");
        tfp = fopen("temp.dat","w");
        while(fscanf(fp,"%s %s %s %s",log.id,log.date,log.intime,log.outtime) != EOF)
        {
           /* strupr(log.id);
            strupr(id);*/
            if(strcmp(log.id,id) != 0)
                fprintf(tfp,"%s %s %s %s\n",log.id,log.date,log.intime,log.outtime);
        }
        fclose(fp);
        fclose(tfp);
        system("del LOG.DAT");
        system("ren temp.dat LOG.DAT");
        title();
        printf("Record DELETED successfully!");
        getchar()/**/;
    }
}

//------------------------------------------------------------------------------------------------------------------

void edit_user()
{
    char nam[30], pass[30],ch, n_nam[30], n_pass[30] = {0},passwrd[30] = {0},n_pass1[30] = {0};
    int c = 0,err = 0;
    title();
    printf("User Name  : ");
    scanf("%s",user.name);
    printf("Password  : ");
   passwdhide(passwrd);
    strcpy(user.password,passwrd);

    fp = fopen("USER.DAT","r");
    while(fscanf(fp,"%s %s %s\n",user.uid,nam,pass) != EOF)
    {
        /*strcpy(nam,strupr(nam));
        strcpy(pass,strupr(pass));
        strcpy(user.name,strupr(user.name));
        strcpy(user.password,strupr(user.password));*/
        if(strcmp(user.name,nam) == 0&&strcmp(user.password,pass) == 0)
            c++;
    }

    fclose(fp);
    title();
    if (c == 0){
        printf("Cannot find the given combination of USER NAME and PASSWORD!\a");
        getchar()/**/;
    }
    else{
        printf("Are you sure you want to CHANGE user name and/or password? <Y/N> : ");
        ch = getchar();
        if(ch == 'Y'||ch == 'y')
        {
            do{
                title();
                err = 0;

                printf("NEW User Name        : ");
                scanf("%s",n_nam);

                printf("NEW Password         : ");
                passwdhide(n_pass);

                printf("Confirm NEW Password : ");
                passwdhide(n_pass1);

                if (strcmp(n_pass,n_pass1) != 0)
                {
                    title();

                    printf("Password and confirm password does not match!\a");
                    getchar()/**/;
                    err++;
                }

            } while(err != 0);

            fp = fopen("USER.DAT","r");
            tfp = fopen("temp.dat","a");
            while(fscanf(fp,"%s %s %s\n",user.uid,nam,pass) != EOF)
            {
               /* strcpy(nam,strupr(nam));
                strcpy(pass,strupr(pass));*/
                if(strcmp(user.name,nam) != 0 || strcmp(user.password,pass) != 0)
                    fprintf(tfp,"%s %s %s\n",user.uid,nam,pass);
                else
                    fprintf(tfp,"%s %s %s\n",user.uid,n_nam,n_pass);
            }
            fclose(fp);
            fclose(tfp);
            system("del USER.DAT");
            system("ren temp.dat USER.DAT");

            title();
            printf("Record has been EDITED successfully!");
            getchar()/**/;
        }
    }
}



/*Copyright (C) <2015>  <Rohit Chaudhari>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.*/

//------------------------------------------------------VIEW LOG----------------------------------

void view_log()
{
    int nr;

    user_log *log,*selected,*temp;

    log         = (user_log *)malloc(sizeof(user_log)*nr);
    selected    = (user_log *)malloc(sizeof(user_log)*nr);
    temp        = (user_log *)malloc(sizeof(user_log)*nr);

    int i = 0,l,j,c = 0,n = 0,l1,count = 0;
    char uid[4],nam[30],pass[30],key,ch;

    do {
        i = 0;c = 0;n = 0;count = 0;
        tfp = fopen("LOG.DAT","r");//note that 2 datafiles LOG.DAT and USER.DAT are opened to store necessary informations in structure variable log[] all informations are availeble on LOG.DAT but to access user name we have to open USER.DAT also. After storing necessary information in structure variable log, there is no need to further open these datafiles

        while(fscanf(tfp,"%s %s %s %s\n",log[i].id,log[i].date,log[i].intime,log[i].outtime) !=  EOF)
        {
            fp = fopen("USER.DAT","r");
            while(fscanf(fp,"%s %s %s\n",uid,nam,pass) !=  EOF)
                if(strcmp(log[i].id,uid) == 0)
                    strcpy(log[i].name,nam);
            i++;
            fclose(fp);
        }
        l = i;//thus l is num of rec.s
        fclose(tfp);

        title();
        printf("1. View by USER NAME");
        printf("2. View by DATE");
        printf("3. View ALL User history");
        printf("4. Return to main menu");
        DASH;
        printf(" Press a number between the range [1 -4]  ");

        key = getchar() - 48;
        j = 0;
        title();

        switch(key)
        {
            case 1:
                printf("Enter user name : "); scanf(" %s",nam);
               /* strcpy(nam,strupr(nam));*/
                for(i = 0;i < l;i++)
                {   /*strcpy(log[i].name,strupr(log[i].name));*/
                    if(strcmp(log[i].name,nam) == 0)
                        selected[j++] = log[i];//selected[] is a structure array variable that stores selected (that are to be displayed) data from log[]
                }
                l1 = j;//l1 is number of selected (that are to be displayed) records
            break;

            case 2:

                do
                {
                    printf("Enter Date (dd/mm/yyyy) : "); scanf(" %s",nam);
                    if(date_check(nam) == 0) {
                        printf("\a");
                    }

                }while(date_check(nam) == 0);

                date_ip(nam);
                for(i = 0,j = 0;i < l;i++)
                      if(strcmp(log[i].date,nam) == 0)
                       		selected[j++] = log[i];
                l1 = j;
            break;

            case 3:

                for(i = 0;i < l;i++)
                      selected[j++] = log[i];
                l1 = j;
            break;

            case 4:

                return;

            break;

            default:

                title();
                printf("Your input is out of range! Enter a choice between 1 to 4!");
                printf("Press ENTER to return to main menu...");
                getchar()/**/;
                c = 1;
        }

        if(l1 == 0)//l1 == 0 means no supporing data available
        {
            title();
            printf("No related record(s) found!\a");
            getchar()/**/;
            view_log();
        }
        if(c == 0)
        {
            for(i = 0,j = l1 - 1;i < l1;i++,j--)
                temp[i] = selected[j];
            for(i = 0;i < l1;i++)
                selected[i] = temp[i];
            do
            {
                title();
                if (l1 < 12)
                {
                    printf("Press S to toggle Sorting between ascending or descending order.");
                }
                else
                {
                printf("Press SPACE BAR to view more data");
                }
                printf("SN\t User Name\tDate\t\tStart time\tEnd Time");
                DASH;

                if(count != 0)
                {
                        for(i = l1 - 1; i >= 0; i--)
                            temp[l1 - i - 1] = selected[i];
                        for(i = 0; i < l1; i++)
                            selected[i] = temp[i];
                }
                if(n  > l1)
                    n = 0;

                for(i = n, j = 0; i < l1; i++, j++)
                {
                    printf("%d.", i+1);
                    selected[i].intime[5] = '\0';
                    selected[i].outtime[5] = '\0';
                    if (strlen(selected[i].date) < 10)
                    {
                        date_op(selected[i].date);
                    }
                    printf("%s\t\t%s\t%s\t\t%s",selected[i].name,selected[i].date,selected[i].intime,selected[i].outtime);
                    if(i  >= n+9) {
                        n = n+10;
                        break;
                    }
                }
                ch = getchar();
                if(ch == 'S' || ch == 's')
                    count = 1;
                if(ch == 32 && i == l1)
                    n = 0;

            } while(ch == 'S' || ch == 's' || ch == 32);
        }
        else
            c = 0;

    } while(1);

}

//---------------------------------------------TRANSACTION-----------------------------------------

void transaction()
{
    char acn[8],mon[10],ch,camt[25],temp[40];
    int c,i,j,l,n,x,pg = 1;
    int nr;


    struct trans t, *selected;
    selected = (struct trans *)malloc(sizeof(struct trans)*nr);

    struct account ac,sac;

    title();
    printf("A/C number         : "); scanf("%s",acn);
    /*strcpy(acn,strupr(acn));*/
    fp = fopen("ACCOUNT.DAT","r");
    c = 0;
    while(fscanf(fp,"%s %s %s %s %s %s %c %s %c %f %f %f\n",ac.ac_no,ac.fname,ac.lname,ac.dob,ac.address,ac.contact,&ac.sex,ac.u_date,&ac.ac_type,&ac.c_bal,&ac.interest,&ac.t_bal) !=  EOF)
        if(strcmp(acn,ac.ac_no) == 0)
        {
            c++;
            sac = ac;
        }
    fclose(fp);
    if (c == 0)
    {
        title();
        printf("Given A/C number does not exits!");
        getchar()/**/;
    }
    else
    {
        fp = fopen("TRANSACTION.DAT","r");
        i = 0;
        while(fscanf(fp,"%s %s %s %s %f %s\n",t.ac_no,t.ac_t,t._date,t._ttime,&t.amt,t.usr) !=  EOF)
                if(strcmp(acn,t.ac_no) == 0)
                    selected[i++] = t;
        l = i;
        fclose(fp);

        strcpy(selected[l].ac_no,t.ac_no);
        strcpy(selected[l].ac_t,"Interest");
        strcpy(selected[l]._date,date);
        _strtime(ttime);
        strcpy(selected[l]._ttime,ttime);
        selected[l].amt = sac.interest;
        l++;
        strcpy(selected[l].ac_no,t.ac_no);
        strcpy(selected[l].ac_t,"Closing balance");
        strcpy(selected[l]._date,date);
        _strtime(ttime);
        strcpy(selected[l]._ttime,ttime);
        selected[l].amt = sac.c_bal+sac.interest;
        l++;
        n = 0; c = 0;
        float dr = 0,cr = 0;
        do
        {
            title();

            puts("SN");
            puts("     Details");
            puts("Date");
            puts("Time");
            puts("   Dr. (NRs.)");
            puts("   Cr. (NRs.)");
            DASH;
            DASH;
            if(n  > l){
                n = 0;
                pg = 1;
            }
            i = n;

            printf("Page : %d out of %d",pg,(int)l/10+1);
            if (l  > 9){
                if (pg !=  (int)l/10+1) {
                    printf("Press SPACE BAR to view next page");
                }
                else {
                    printf("Press SPACE BAR to view first page");
                }
            }
            printf("A/C holder : %s %s",sac.fname,sac.lname);
            currency(camt,sac.t_bal);
            strcpy(temp,"Bank Balance : ");
            strcat(temp,camt);
            printf("%s",temp);


            for(j = 0;j < 10;j++)
            {
                printf("%d.",i+1);
                for(x = 0;x < strlen(selected[i].ac_t);x++)
                    if(selected[i].ac_t[x] = '+')
                        selected[i].ac_t[x] = 32;

                puts(selected[i].ac_t);

                if (c == 0)
                    date_op(selected[i]._date);

                puts(selected[i]._date);
                selected[i]._ttime[5] = '\0';
                printf("%s",selected[i]._ttime);
                if(selected[i].ac_t[0] == 'T')
                {
                    printf("%13.2f",selected[i].amt);
                    cr += selected[i].amt;
                }
                else if (selected[i].ac_t[0] == 'C' && (selected[i].ac_t[5] == 'W' || selected[i].ac_t[1] == 'l'))
                {
                    printf("%13.2f",selected[i].amt);
                    cr += selected[i].amt;
                }
                else
                {
                    printf("%13.2f",selected[i].amt);
                    dr += selected[i].amt;
                }
                i++;
                if(i  >= l)
                    break;

            }
            n += 10;
            if (i  >= l)
            {
                for(x = 0;x < 78;x++) printf("_");
                printf("TOTAL");
                printf("%13.2f",dr);
                printf("%13.2f",cr);
                dr = cr = 0;
                for(x = 1;x < 79;x++) printf("_");
                c++;
            }
            ch = getchar();

            if (ch == 32)
                pg++;

        } while(ch == 32);

    }
}

//------------------------------------------------DEPOSIT--------------------------------------------

void deposit()
{
    title();
    struct account ac;
    char acn[10],curr[35],ch,csh[80],temp[80],nam[35];
    int c = 0;
    float amt;

    printf("Deposit to A/C number            : "); scanf("%s",acn);
    //strcpy(acn,strupr(acn)); //changing a/c no. to uppercase
    fp = fopen("ACCOUNT.DAT","r");
    while(fscanf(fp,"%s %s %s %s %s %s %c %s %c %f %f %f\n",ac.ac_no,ac.fname,ac.lname,ac.dob,ac.address,ac.contact,&ac.sex,ac.u_date,&ac.ac_type,&ac.c_bal,&ac.interest,&ac.t_bal) !=  EOF){
        if(strcmp(acn,ac.ac_no) == 0) {
            c++;
            strcpy(nam,ac.fname);
            strcat(nam," ");
            strcat(nam,ac.lname);
        } //variable c counts if the given a/c no exist in data file or not. also if available, the full name is extracted
    }
    fclose(fp);

    if(c == 0)//c = 0 means no given a/c no. in data file
    {
        title();
        printf("Given A/C number does not exits!");
        getchar()/**/;
        return;
    }

    printf("[ %s ]",nam);
    printf("Amount to be Deposited (in NRs.) : "); scanf("%f",&amt);
    title();
    printf("Confirm Transaction");
    currency(curr,amt);
    printf("%s to be deposited in A/C number : %s [ %s ]",curr,acn,nam);

    numtowrd((double)amt,csh);
    strcpy(temp,"[In words : ");
    strcat(temp,csh);
    strcat(temp,"]");
    puts(temp);

    printf("Are you sure you want to perform this tranasction?  < Y/N  > ");
    ch = getchar();
    if (ch == 'Y' || ch == 'y')
    {
        fp = fopen("ACCOUNT.DAT","r");
        fp1 = fopen("TEMP.DAT","a");
        while(fscanf(fp,"%s %s %s %s %s %s %c %s %c %f %f %f\n",ac.ac_no,ac.fname,ac.lname,ac.dob,ac.address,ac.contact,&ac.sex,ac.u_date,&ac.ac_type,&ac.c_bal,&ac.interest,&ac.t_bal) !=  EOF)
        {
            if (strcmp(ac.ac_no,acn) == 0)
                ac.c_bal += amt;//balance is increased

            ac.t_bal = ac.c_bal+ac.interest;
            fprintf(fp1,"%s %s %s %s %s %s %c %s %c %.2f %.2f %.2f\n",ac.ac_no,ac.fname,ac.lname,ac.dob,ac.address,ac.contact,ac.sex,ac.u_date,ac.ac_type,ac.c_bal,ac.interest,ac.t_bal);
        }
        fclose(fp1);
        fclose(fp);
        system("del ACCOUNT.DAT");
        system("ren TEMP.DAT ACCOUNT.DAT");

        fp = fopen("TRANSACTION.DAT","a");//transaction is added
        _strtime(ttime);
        fprintf(fp,"%s %s %s %s %.2f %s\n",acn,"Cash+Deposited",date,ttime,amt,user.name);
        fclose(fp);
        title();
        printf("Transaction completed successfully!");
        getchar()/**/;
    }

}

//-----------------------------------------------WITHDRAW---------------------------------------

void withdraw()//exactly similar to deposit. only difference is amount is subtracted.
{
    title();
    struct account ac;
    char acn[10],ch,curr[35],csh[80],temp[80], nam[50];
    int c = 0;
    float amt;

    printf("Withdraw from A/C number          : "); scanf("%s",acn);
    //strcpy(acn,strupr(acn));
    fp = fopen("ACCOUNT.DAT","r");
    while(fscanf(fp,"%s %s %s %s %s %s %c %s %c %f %f %f\n",ac.ac_no,ac.fname,ac.lname,ac.dob,ac.address,ac.contact,&ac.sex,ac.u_date,&ac.ac_type,&ac.c_bal,&ac.interest,&ac.t_bal) !=  EOF){
        if(strcmp(acn,ac.ac_no) == 0){
            c++;
            strcpy(nam,ac.fname);
            strcat(nam," ");
            strcat(nam,ac.lname);
        }
    }
    fclose(fp);

    if(c == 0)
    {
        title();
        printf("Given A/C number does not exits!");
        getchar()/**/;
        return;
    }

    printf("[ %s ]",nam);
    printf("Amount to be Withdrawn (in NRs.)  : "); scanf("%f",&amt);

    fp = fopen("ACCOUNT.DAT","r");

    c = 0;
    while(fscanf(fp,"%s %s %s %s %s %s %c %s %c %f %f %f\n",ac.ac_no,ac.fname,ac.lname,ac.dob,ac.address,ac.contact,&ac.sex,ac.u_date,&ac.ac_type,&ac.c_bal,&ac.interest,&ac.t_bal) !=  EOF)
    {
        if (strcmp(ac.ac_no,acn) == 0 && ac.t_bal < amt)// when given amount is higher than bank balance
        {
            title();
            printf("Sorry, the current balance is Rs. %.2f only!",ac.t_bal);
            printf("Transaction NOT completed!");
            c = 1;
            getchar()/**/;
            return;
        }

    }
    fclose(fp);
    title();
    printf("Confirm Transaction");
    currency(curr,amt);
    printf("%s to be Withdrawn from A/C number : %s [%s]",curr,acn,nam);
    numtowrd((double)amt,csh);
    strcpy(temp,"[In words : ");
    strcat(temp,csh);
    strcat(temp,"]");
	puts(temp);
	printf("Are you sure you want to perform this tranasction?  < Y/N  > ");
    ch = getchar();

    if (ch == 'Y' || ch == 'y')
    {
        fp = fopen("ACCOUNT.DAT","r");
        fp1 = fopen("TEMP.DAT","w");
        c = 0;
        while(fscanf(fp,"%s %s %s %s %s %s %c %s %c %f %f %f\n",ac.ac_no,ac.fname,ac.lname,ac.dob,ac.address,ac.contact,&ac.sex,ac.u_date,&ac.ac_type,&ac.c_bal,&ac.interest,&ac.t_bal) !=  EOF)
        {
            if (strcmp(ac.ac_no,acn) == 0)
                ac.c_bal -= amt;
            if (ac.c_bal < 0){
                ac.interest += ac.c_bal;
                ac.c_bal = 0;
            }
            ac.t_bal = ac.c_bal + ac.interest;

            fprintf(fp1,"%s %s %s %s %s %s %c %s %c %.2f %.2f %.2f\n",ac.ac_no,ac.fname,ac.lname,ac.dob,ac.address,ac.contact,ac.sex,ac.u_date,ac.ac_type,ac.c_bal,ac.interest,ac.t_bal);
        }
        fclose(fp1);
        fclose(fp);
        system("del ACCOUNT.DAT");
        system("ren TEMP.DAT ACCOUNT.DAT");
        fp = fopen("TRANSACTION.DAT","a");
        _strtime(ttime);
        fprintf(fp,"%s %s %s %s %.2f %s\n",acn,"Cash+Withdrawn",date,ttime,amt,user.name);
        fclose(fp);
        title();
        printf("Transaction completed successfully!");

        getchar()/**/;
    }
}

//--------------------------------------------------FUND TRANSFER----------------------------------------

void fund_transfer()
{
    char f_acn[8],t_acn[8],ch,curr[35],rem[25],csh[80] = {0},temp[80] = {0},fnam[35],tnam[35];
    struct account ac;
    float amt;
    int c = 0;

    title();

    printf("Transferred from (A/C no. )        : "); scanf("%s",f_acn);

   // strcpy(f_acn,strupr(f_acn));
    fp = fopen("ACCOUNT.DAT","r");
    c = 0;
    while(fscanf(fp,"%s %s %s %s %s %s %c %s %c %f %f %f\n",ac.ac_no,ac.fname,ac.lname,ac.dob,ac.address,ac.contact,&ac.sex,ac.u_date,&ac.ac_type,&ac.c_bal,&ac.interest,&ac.t_bal) !=  EOF){
        if(strcmp(f_acn,ac.ac_no) == 0){
            c++;
            strcpy(fnam,ac.fname);
            strcat(fnam," ");
            strcat(fnam,ac.lname);
        }
        fclose(fp);
    }
    if (c == 0)
    {
        title();
        printf("Given A/C number does not exits!");
        getchar()/**/;
        return;
    }

    printf("[ %s ]",fnam);
    printf("Transferred to  (A/C no. )         : "); scanf("%s",t_acn);

   // strcpy(t_acn,strupr(t_acn));
    fp = fopen("ACCOUNT.DAT","r");
    c = 0;
    while(fscanf(fp,"%s %s %s %s %s %s %c %s %c %f %f %f\n",ac.ac_no,ac.fname,ac.lname,ac.dob,ac.address,ac.contact,&ac.sex,ac.u_date,&ac.ac_type,&ac.c_bal,&ac.interest,&ac.t_bal) !=  EOF){
        if(strcmp(t_acn,ac.ac_no) == 0){
            c++;
            strcpy(tnam,ac.fname);
            strcat(tnam," ");
            strcat(tnam,ac.lname);
        }
    }
    fclose(fp);

    if (c == 0)
    {
            title();
            printf("Given A/C number does not exits!");
            getchar()/**/;
            return;
    }
    printf("[ %s ]",tnam);
    printf("Amount to be transferred (in NRs.) : "); scanf("%f",&amt);
    fp = fopen("ACCOUNT.DAT","r");
    c = 0;
    while(fscanf(fp,"%s %s %s %s %s %s %c %s %c %f %f %f\n",ac.ac_no,ac.fname,ac.lname,ac.dob,ac.address,ac.contact,&ac.sex,ac.u_date,&ac.ac_type,&ac.c_bal,&ac.interest,&ac.t_bal) !=  EOF)
    {
                if (strcmp(ac.ac_no,f_acn) == 0)
                {
                    if (ac.t_bal < amt)
                    {
                        title();
                        printf("Sorry, the current balance is Rs. %.2f only!",ac.t_bal);
                        printf("Transaction NOT completed!");
                        getchar()/**/;
                        return;
                    }
                }
    }
    fclose(fp);
    title();
    printf("Confirm Transaction");
    currency(curr,amt);
    printf("%s to be Transfered",curr);
    numtowrd((double)amt,csh);
    strcpy(temp,"[In words : ");
    strcat(temp,csh);
    strcat(temp,"]");
    puts(temp);
    printf("FROM A/C number %s [ %s ]",f_acn,fnam);
    printf("TO   A/C number %s [ %s ]",t_acn,tnam);
    printf("Are you sure you want to perform this tranasction?  < Y/N  > ");
    ch = getchar();
    if (ch == 'Y' || ch == 'y')
    {
                fp = fopen("ACCOUNT.DAT","r");
                fp1 = fopen("TEMP.DAT","w");
                c = 0;
                while(fscanf(fp,"%s %s %s %s %s %s %c %s %c %f %f %f\n",ac.ac_no,ac.fname,ac.lname,ac.dob,ac.address,ac.contact,&ac.sex,ac.u_date,&ac.ac_type,&ac.c_bal,&ac.interest,&ac.t_bal) !=  EOF)
                {
                    if (strcmp(ac.ac_no,f_acn) == 0)
                        ac.c_bal -= amt;
                    if (ac.c_bal < 0){
                        ac.interest += ac.c_bal;
                        ac.c_bal = 0;
                    }
                    ac.t_bal = ac.c_bal + ac.interest;

                    fprintf(fp1,"%s %s %s %s %s %s %c %s %c %.2f %.2f %.2f\n",ac.ac_no,ac.fname,ac.lname,ac.dob,ac.address,ac.contact,ac.sex,ac.u_date,ac.ac_type,ac.c_bal,ac.interest,ac.t_bal);
                }
                fclose(fp1);
                fclose(fp);
                system("del ACCOUNT.DAT");
                system("ren TEMP.DAT ACCOUNT.DAT");

                fp = fopen("ACCOUNT.DAT","r");
                fp1 = fopen("TEMP.DAT","w");
                c = 0;
                while(fscanf(fp,"%s %s %s %s %s %s %c %s %c %f %f %f\n",ac.ac_no,ac.fname,ac.lname,ac.dob,ac.address,ac.contact,&ac.sex,ac.u_date,&ac.ac_type,&ac.c_bal,&ac.interest,&ac.t_bal) !=  EOF)
                {
                    if (strcmp(ac.ac_no,t_acn) == 0)
                        ac.c_bal += amt;
                    ac.t_bal = ac.c_bal + ac.interest;

                    fprintf(fp1,"%s %s %s %s %s %s %c %s %c %.2f %.2f %.2f\n",ac.ac_no,ac.fname,ac.lname,ac.dob,ac.address,ac.contact,ac.sex,ac.u_date,ac.ac_type,ac.c_bal,ac.interest,ac.t_bal);
                }
                fclose(fp1);
                fclose(fp);
                system("del ACCOUNT.DAT");
                system("ren TEMP.DAT ACCOUNT.DAT");
                fp = fopen("TRANSACTION.DAT","a");
                _strtime(ttime);
                strcpy(rem,"Transfered+to+");
                strcat(rem,t_acn);
                fprintf(fp,"%s %s %s %s %.2f %s\n",f_acn,rem,date,ttime,amt,user.name);
                strcpy(rem,"Received+from+");
                strcat(rem,f_acn);
                fprintf(fp,"%s %s %s %s %.2f %s\n",t_acn,rem,date,ttime,amt,user.name);
                fclose(fp);
                title();
                printf("Transaction completed successfully!");
                getchar()/**/;
    }
}

//------------------------------------------------------ADD ACCOUNT ---------------------------------

void add_ac()
{
     struct account ac;//structure variable ac created
     char acn[8],curr[35],ch;
     int i;
     float intrate;

     fp = fopen("ACCOUNT.DAT","r");
     if(fp == NULL)
        strcpy(ac.ac_no,"AC00001");//if "ACCOUNT.DAT" does not exist i.e. there are no records at all then A/C no. is taken AC00001 for 1st rec
     else //otherwise a/c no of last record is accessed and increased by unit value which becomes the new a/c no.
     {
        while(fscanf(fp,"%s %s %s %s %s %s %c %s %c %f %f %f\n" , ac.ac_no , ac.fname , ac.lname , ac.dob , ac.address , ac.contact ,&ac.sex , ac.u_date, &ac.ac_type, &ac.c_bal, &ac.interest, &ac.t_bal) !=  EOF);//note the while statement is terminated without any statement inside it. This helps in geting the last rec. of the data file
        increase(ac.ac_no);
     }

     fclose(fp);

     title();
     printf("PERSONAL DETAILS");
     DASH;
     printf("Full name        :    "); scanf("%s",ac.fname); scanf("%s",ac.lname);
     printf("Gender  < M/F  >      :    "); scanf(" %c",&ac.sex);

     int temp1,temp2;

     do{
        printf("DOB (dd/mm/yyyy) :    "); scanf(" %s",ac.dob);
        temp1 = (ac.dob[6] - 48) * 1000 + (ac.dob[7] - 48) * 100 + (ac.dob[8] - 48) * 10 + (ac.dob[9] - 48);
        temp2 = 2000 + (date[6]  - 48) * 10 + (date[7] - 48);
        if(date_check(ac.dob) == 0 || temp1  > temp2)//checks if dob format is correct, on invalid format screen is partially cleared, beep tone is produced and asked to re-enter date
        		printf("\a");

     }while(date_check(ac.dob) == 0 || temp1  > temp2);//asks date unless format is correct

     _strdate(date);
     date_ip(ac.dob);//to change dd/mm/yyyy format to mm/dd/yy format to store in file
     printf("Address          :    "); scanf(" %[^\n]s",ac.address);

     for(i = 0;i < strlen(ac.address);i++){//User may enter a space key for address (eg. New Baneswor). bt on reading formatted data from file space is taken as terminating char (eg. "New" and "Baneswor" are taken as separate variables. To remove this error, spaces entered by user is replace by '+'. Now "New+Baneswor is single string
        if (ac.address[i] == 32)
            ac.address[i] = '+';
    }
    int err;
    do
    {
        printf("Contact number   :    "); scanf(" %s",ac.contact);
        err = 0;
        for(i = 0;i < strlen(ac.contact);i++){
            if(!((ac.contact[i] >= '0'  &&  ac.contact[i] <= '9'  ||  ac.contact[i] == '-'  ||  ac.contact[i] == '+')  &&  strlen(ac.contact) >= 6))
                err++;
            if (err !=  0)
                printf("\a");
        }
    } while(err !=  0);
     title();
     printf("ACCOUNT DETAILS");
     DASH;
     printf("A/C number                     :   %s",ac.ac_no);
     int flag;
     do {
         printf("A/C type  < S/C  >                  :   ");  scanf(" %c",&ac.ac_type);
         flag = 0;

         if (ac.ac_type == 'S' || ac.ac_type == 's')
         {
             intrate = 8; //interest rate 8% for saving a/c
             printf("Saving Account");
         }
         else if (ac.ac_type == 'C' || ac.ac_type == 'c')
         {
             intrate = 4; //interest rate 4% for current saving
             printf("Current Saving");
         }
         else
         {
             printf("\a");
             flag = 1;//note that flag was initially zero. on error, its turned to 1
         }

     } while(flag == 1);//for repeated asking of a/c type unless user enters correct input

     date_op(date);
     printf("Account Opened Date            :   %s",date);
     _strdate(date);
     strcpy(ac.u_date,date);//copies current date to last updated date
     printf("Interest rate                  :   %.2f%c per annum", intrate,37);
     do
     {
        printf("Opening Balance (in NRs.)      :   "); scanf("%f",&ac.c_bal);
        if (ac.c_bal < 0)
            printf("\a");

     }while(ac.c_bal < 0);
     ac.interest = 0;
     ac.t_bal = ac.c_bal+ac.interest;

    title();
    printf("Full name      : %s %s\n",ac.fname,ac.lname);
    printf("Gender  < M/F  >    : %c\n",ac.sex);
    printf("DOB (mm/dd/yy) : %s\n",ac.dob);
    printf("Address        : \n");
    for(i = 0;i < strlen(ac.address);i++){//In order to display address, '+' is replaced by space
            if (ac.address[i] == '+')
                putchar(32);
            else
                putchar(ac.address[i]);
    }
    printf("Contact number : %s",ac.contact);

    printf("A/C number      : %s",ac.ac_no);
    printf("A/C type        : ");
    if (ac.ac_type == 'S' || ac.ac_type == 's')
        printf("Saving Account");
    if (ac.ac_type == 'C' || ac.ac_type == 'c')
        printf("Current Saving");

    printf("Current Date    : %s",date);
    printf("Interest rate   : %.2f%c per annum", intrate,37);
    currency(curr,ac.c_bal);
    printf("Opening Balance : %s",curr);
    DASH;

    printf("Are the informations above all correct?  < Y/N  >  ");
    ch = getchar();

    if (ch == 'Y' || ch == 'y')//On confirmation, new a/c (in account.dat) + transaction showing "A/C Opened" is added (in transaction.dat)
    {
        fp = fopen("ACCOUNT.DAT","a");
        fp1 = fopen("TRANSACTION.DAT","a");
        _strtime(ttime);
        fprintf(fp1,"%s %s %s %s %.2f %s\n",ac.ac_no,"A/C+Opened",date,ttime,ac.c_bal,user.name);
        fprintf(fp,"%s %s %s %s %s %s %c %s %c %.2f %.2f %.2f\n",ac.ac_no,ac.fname,ac.lname,ac.dob,ac.address,ac.contact,ac.sex,ac.u_date,ac.ac_type,ac.c_bal,ac.interest,ac.t_bal);
        fclose(fp);
        fclose(fp1);
        title();
        printf("Record Added Successfully!");
        getchar()/**/;
    }
}

//---------------------------------------------ACCOUNT INFORMATION--------------------------------------------------

void ac_info()
{   int i , pos  =  0,n = 0,m = 0,count;
    char buffer[30] = {0},curr[35],curr1[35],curr2[35], c,acn[8];
    struct account ac, srh;//structure variable srh stores the matched data from data file
    float intrate;

    title();//sub menu to search records
    printf("1. Search by A/C number");
    printf("2. Search by Name");
    printf("3. Return to main menu");
    printf("Press a choice between 1-3 ");
    c = getchar();
    switch(c - 48)
    {
        case 1:

            title();
            printf("Enter A/C number : ");
            scanf("%s",acn);
           //
            //strcpy(acn,strupr(acn));
            fp = fopen("ACCOUNT.DAT","r");
            count  =  0;
            while(fscanf(fp,"%s %s %s %s %s %s %c %s %c %f %f %f\n",ac.ac_no,ac.fname,ac.lname,ac.dob,ac.address,ac.contact,&ac.sex,ac.u_date,&ac.ac_type,&ac.c_bal,&ac.interest,&ac.t_bal) !=  EOF)
            {
                if (strcmp(acn,ac.ac_no) == 0){
                srh = ac;
                count++;
                }// if a/c no matches, srh stores all values of ac
            }

            if (count == 0)
            {
                title();
                printf("Given A/C number does not exits!");
                getchar()/**/;
                return;
            }

            ac = srh;
            title();
            printf("Full name      : %s %s",ac.fname,ac.lname);
            printf("Gender  < M/F  >    : %c",ac.sex);
            printf("DOB (mm/dd/yy) : %s",ac.dob);
            printf("Address        : ");
            for(i = 0;i < strlen(ac.address);i++){
                if (ac.address[i] == '+')
                    putchar(32);
                else
                    putchar(ac.address[i]);
            }
            printf("Contact number : %s",ac.contact);

            printf("A/C number      : %s",ac.ac_no);
            printf("A/C type  < S/F  >   : ");
            if (ac.ac_type == 'S' || ac.ac_type == 's'){
                printf("Saving Account");
                intrate = 8.0;
            }
            if (ac.ac_type == 'C' || ac.ac_type == 'c'){
                printf("Current Saving");
                intrate = 4.0;
            }


            printf("Interest rate   : %.2f%c per annum", intrate,37);
            currency(curr,ac.c_bal);
            printf("Current Balance : %s",curr);
            currency(curr1,ac.interest);
            printf("Interset        : %s",curr1);
            currency(curr2,ac.t_bal);
            printf("Total Balance   : %s",curr2);
            DASH;
            printf("Press ENTER to search again or ANY OTHER KEY to return to main menu");
            c = getchar()/**/;
            if(c == 13)
                ac_info();//13 ascii code of enter
            else
                return;
        break;

        case 2:
         	title();
         	printf("Enter first name : ");
           	do{
            	    DASH;
                    DASH;
                    printf("Press ENTER to view detailed information of FIRST record");
                    printf(" or press ESCAPE to return to main menu");
            	    c  =  getchar()/**/;
            		if( isprint(c) )
            		{
            			buffer[ pos++ ]  =  c;
            			printf("%c", c);
            			n++;
            		}
            		else if( c  ==  8  &&  pos )
            		{
            			buffer[ pos-- ]  =  '\0';
            			printf("%s", "\b \b");
            			n--;
                    }

                    m = 0;
                    printf("A/C no.");
           		    printf("Full name");
           		    printf("Contact no.");
                    printf("Net Balance");
                    DASH;

                    fp = fopen("ACCOUNT.DAT","r");

                    while(fscanf(fp,"%s %s %s %s %s %s %c %s %c %f %f %f\n",ac.ac_no,ac.fname,ac.lname,ac.dob,ac.address,ac.contact,&ac.sex,ac.u_date,&ac.ac_type,&ac.c_bal,&ac.interest,&ac.t_bal) !=  EOF)
                    {
                        count = 0;
                        for(i = 0;i < strlen(buffer);i++)
                        {
                            if(buffer[i]  >= 'a' && buffer[i] <= 'z')
                                buffer[i] = buffer[i]-32;
                            if(ac.fname[i]  >= 'a' && ac.fname[i] <= 'z')
                                ac.fname[i] = ac.fname[i]-32;
                            if(buffer[i] !=  ac.fname[i])
                                count++;
                        }

                        if(count == 0){
                            if(m == 0)
                                srh = ac;
                            if(m <= 9){
                                printf("%s",ac.ac_no);
                                printf("%s %s",ac.fname,ac.lname);
                                printf("%s",ac.contact);
                                currency(curr,ac.t_bal);
                                printf("%20s",curr);
                                m++;
                            }
                        }

                    }
                    fclose(fp);

            }while( c != 13 && c != 27 );

            if (c == 13){
                ac = srh;
                title();
                printf("Full name      : %s %s",ac.fname,ac.lname);
                printf("Gender  < M/F  >    : %c",ac.sex);
                printf("DOB (mm/dd/yy) : %s",ac.dob);
                printf("Address        : ");
                for(i = 0;i < strlen(ac.address);i++){
                        if (ac.address[i] == '+')
                            putchar(32);
                        else
                            putchar(ac.address[i]);
                }
                printf("Contact number : %s",ac.contact);

                printf("A/C number      : %s",ac.ac_no);
                printf("A/C type  < S/F  >   : ");
                if (ac.ac_type == 'S' || ac.ac_type == 's'){
                    printf("Saving Account");
                    intrate = 8.0;
                }
                if (ac.ac_type == 'C' || ac.ac_type == 'c'){
                    printf("Current Saving");
                    intrate = 4.0;
                }


                printf("Interest rate   : %.2f%c per annum", intrate,37);
                currency(curr,ac.c_bal);
                printf("Current Balance : %s",curr);
                currency(curr1,ac.interest);
                printf("Interset        : %s",curr1);
                currency(curr2,ac.t_bal);
                printf("Total Balance   : %s",curr2);

                DASH;
                printf("Press ENTER to search again or ANY OTHER KEY to return to main menu");
                c = getchar()/**/;
                if(c == 13)
                    ac_info();
                else
                    return;
            }

            break;

        case 3:
            return;
            break;

        default:
            title();
            printf("Your input is out of range! Enter a choice between 1 to 8!");
            printf("Press any key to return to main menu...");
            getchar()/**/;
            ac_info();
    }
}

