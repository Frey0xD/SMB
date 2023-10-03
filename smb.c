#pragma runopts (xplink(on))  
#include <iostream>  
#include <iomanip>   
#include <fstream>
#include <cctype>
#include <string>
#include <sys/stat.h> //use with struct stat attrib;
#include <unistd.h> //stat("afile.txt", &attrib);
#include <time.h> //clock = gmtime(&(attrib.st_mtime));
#include <stdio.h>
#include <stddef.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
/* Rads smb  */
using namespace std; 
int i; //backup function file count
int  ReadSmbin(string *empptr, string *qptr);
void SortSmbin(string *empptr, string *qptr, int noRec);
void DisplaySmbin(string *empptr, string *qptr, int noRec);
void WriteSmbout(string *empptr, string *qptr, int noRec);
void ShowMenu();
void BackupSmbout();
int  AddUser(string *empptr, string *qptr, int noRec);
int  RemoveUser(string *empptr, string *qptr, int noRec);
void PressEnter();
void ShowDuplicatesSmbin(string *empptr, string *qptr, int noRec);
string ContinueYN();
void SearchSmbin(string *empptr, string *qptr, int noRec);
string StringToLower(string str);
void CheckQID(string qid);
int  DirLen(char *opndir);
void SortFileNames(string *fldname, int i);
void DeleteOldBackups();
string* ReadFileNames(char* opndir,string* fldname);//from dir to get fi
int  main()
{
   string empid[1200];
   string qid[1200];
   int noRec,sel;
   int x,y;
   string strsel;
   string *empptr;
   string *qptr;
   empptr = &empid[0];
   qptr= &qid[0];

   noRec=ReadSmbin(empptr,qptr);
   SortSmbin(empptr,qptr,noRec);
   for(;;)
   {
      ShowMenu();
      cin >> strsel;

      if (atoi(strsel.c_str()) == 0){ //determines if selection is
         exit(1);  //will exit if not numeric
      }
      else{
         sel=atoi(strsel.c_str());
      }

      if (sel==1 || sel==3 || sel==4 || sel==5 || sel==6 || sel==9){                           
         if (noRec<1){
            cout << "WARNING: There are no records in the smbid file" << endl;
            PressEnter(); //if there are no records
         }
      }   
      if (sel==1){//display (with read and sort)     
         noRec=ReadSmbin(empptr,qptr);
         SortSmbin(empptr,qptr,noRec);
         DisplaySmbin(empptr,qptr,noRec);
      }
      else if (sel==2) //add a user
      {
         noRec=ReadSmbin(empptr,qptr);
         int tmpnoRec= noRec;
         noRec = AddUser(empptr,qptr,noRec);
         if (tmpnoRec < noRec){
            SortSmbin(empptr,qptr,noRec);
            WriteSmbout(empptr,qptr,noRec);
            cout << "Smbidmap user added successfully" << endl;
            PressEnter();                                
         }
         else{
            cout << "Smbidmap user was not added" << endl;
            PressEnter();                                
         }
      }
      else if (sel==3){ //backup
         noRec=ReadSmbin(empptr,qptr);
         BackupSmbout();
         cout << "Smbidmap backeded up successfully" << endl;
         PressEnter();                                
      }
      else if (sel==4){
         int tmpnoRec= noRec;
         noRec = RemoveUser(empptr,qptr,noRec);
         if (tmpnoRec > noRec){
            SortSmbin(empptr,qptr,noRec);
            WriteSmbout(empptr,qptr,noRec);
            cout << "Smbidmap user removed successfully" << endl;
            PressEnter();
         }
         else{
            cout << "Smbidmap user was not removed" << endl;
            PressEnter();
         }

      }
      else if (sel==5){
         SearchSmbin(empptr,qptr,noRec);                   
         PressEnter();
      }
      else if (sel==6){
         ShowDuplicatesSmbin(empptr,qptr,noRec);           
         PressEnter();
      }
      else if (sel==8)
      {
         noRec=ReadSmbin(empptr,qptr);
         SortSmbin(empptr,qptr,noRec);
         cout << "Smbidmap read successfully" << endl;
         PressEnter();                                
      }
      else if (sel==9){ //write (does backup)
         BackupSmbout();
         WriteSmbout(empptr,qptr,noRec);
         cout << "Smbidmap written successfully" << endl;
         PressEnter();
      }
      else if (sel==10){
         system("clear");
         cout << "SMB ID MAP Program by Radomir Wojcik" << endl;
         cout << "Developed October 2006 for RBC Bank" << endl;
         cout << "Quick tips:                        " << endl;
         cout << "   - Hidden Options for advanced users are 8 for read 9 for write." << endl;
         cout << "   - You can do a search by pressing 5  and it searches both qids and employee ids but only accepts 1 word at a time.  " << endl;
         cout << "   - Be carefuly when adding and removing uses, do a search or display before doing so." << endl;
         cout << "   - Refresh system after adding: /f dfs,send dfskern,reload,smbmap   " << endl;                 
         PressEnter();
      }
      else if (sel==11){
         CheckQID("q34jvvw");
         PressEnter();
      }
      else{
         break; //will also exit if not 1 -4
      }
   }
   return 0;
}


int ReadSmbin(string *empptr, string *qptr)
{
   ifstream smbIn ("/XAS2/etc/dfs/home/dfskern/smbidmap");
// ifstream smbIn ("./smbidmap1");                   
   int x,y;
   string dummy,qid;
   if (!smbIn)
   {
      cout << "could not open file" << endl;
      PressEnter();                                
      exit(1);
   }
   x=0; 
   while (!smbIn.eof()) 
   { 
      qid="";
      smbIn >> *(empptr+x) >> qid;       
      if (*(empptr+x)!="" && qid !=""){    
         qid=StringToLower(qid);
     //  for (y=0;y<qid.length();y++){
     //     qid[y]=tolower(qid[y]);
     //  }
         *(qptr+x) = qid;
    //   cout << x << *(qptr+x) << *(empptr+x) << endl;
    //   PressEnter();
         x++;
      }
   } 
   smbIn.close(); 
   return x;   
}
void SortSmbin(string *empptr, string *qptr, int noRec)
{
string temp;
int x,y;
   for (x=0;x<noRec;x++)
   {
       for (y=x+1;y<noRec;y++)
       {
          if (*(empptr+x) > *(empptr+y))
          {
             temp = *(empptr+x);
             *(empptr+x) = *(empptr+y);
             *(empptr+y) = temp;

             temp = *(qptr+x);
             *(qptr+x) = *(qptr+y);
             *(qptr+y) = temp;
          }
       }
    }
}

void DisplaySmbin(string *empptr, string *qptr, int noRec)
{
   int x;
   string yn;
   system("clear");
   for (x=0;x<noRec;x=x+2)
   {
      printf("%03d",x); 
      cout << ": " << *(empptr+x) << "\t"<< *(qptr+x) << "\t\t\t";                    
      if (x+1 < noRec){ 
         printf("%03d",x+1);
         cout << ": " << *(empptr+1+x) << "\t"<< *(qptr+1+x) << endl;                     
      }
      if (((x%30) == 0 && x>0) || noRec==(x+1) || noRec==(x+2)){
         yn=ContinueYN();
         if (yn=="n") break;
      }
   }
 //if ((x%30) != 0) PressEnter();
}

void WriteSmbout(string *empptr, string *qptr, int noRec)
{
   int x;
// ofstream  smbOut("./smbidmap1", ios::out);                    
   ofstream  smbOut("/XAS2/etc/dfs/home/dfskern/smbidmap", ios::out);

   for (x=0;x<noRec;x++)
   {
      smbOut << *(empptr+x) << endl << *(qptr+x);
      if (x!=noRec-1) smbOut << endl << endl;
   }
    smbOut.close();
    return ;

}

void BackupSmbout()
{
   time_t t = time(0);
   tm time = *localtime(&t);
   int n;
   char buffer [150];
   int d,M,y,m,s,h;
   d=time.tm_mday;
   M=time.tm_mon+1;
   y=time.tm_year+1900;
   h=time.tm_hour;
   m=time.tm_min;
   s=time.tm_sec;
   n=sprintf (buffer, "cp /XAS2/etc/dfs/home/dfskern/smbidmap /XAS2/etc/dfs/home/dfskern/smbbkp/smbbkp%d%02d%02d_%02d%02d%02d",y,M,d,h,m,s);
   system(buffer);
   DeleteOldBackups();  //calls function to delete old backups  
}
int AddUser(string *empptr, string *qptr, int noRec)
{
   int x,i;
   string newEmpId,newQid,strError,yn;
   strError="";
   yn="y";
   cout << "Please type in a 9 digit employee id: ";
   cin >> newEmpId;

   cout << "Please type in a valid tso qid: ";
   cin >> newQid;

   for (x =0;x<newQid.length();x++)
   { //converts all to lower case for comparison
      newQid[x]=tolower(newQid[x]);
   }

   for (x=0;x<noRec;x++) //error checking begins
   {
      if (*(empptr+x) == newEmpId && *(qptr+x) == newQid && yn=="y")        {
         strError += "Duplicate Record already exists\n";
         break;
      }
   }
   if (strError=="" && yn=="y"){
      for (x=0;x<noRec;x++){//error checking continues
         if (*(empptr+x) == newEmpId && yn=="y"){       
           cout << "Duplicate LAN/Employee ID already exists\n";
           yn=ContinueYN();
           break;
         }
      }
      for (x=0;x<noRec;x++){//error checking continues
         if (*(qptr+x) == newQid && yn=="y"){       
           cout <<  "Duplicate Host QId already exists\n";
           yn=ContinueYN();
           break;
         }
      }
   }
   if (strError=="" && yn=="y"){
      for (i = 0; i<newEmpId.length(); i++){
         if (!isdigit(newEmpId[i])){
            cout << "Employee ID entered is not all numeric\n";    
            yn=ContinueYN();
            break;
        }
      }
   }
   if (atoi(newQid.c_str()) != 0) strError +=          
      "Host QID cannot  be all numeric\n";
   if (newEmpId.length() != 9 && strError=="" && yn=="y"){
      cout << "Employee ID entered is not 9 in length\n";
      yn=ContinueYN();
   }
   if (newQid.length() != 7 && strError=="" && yn=="y"){
       cout << "Host QID entered is not 7 in length\n";
       yn=ContinueYN();
   }
   if (newQid[0] != 'q' && newQid[0] != 'z' && newQid[0] != 'r' && strError=="" && yn=="y"){
      cout << "Host QID entered does not start with a q or z\n";
      yn=ContinueYN();
   }
   if (strError == "" && yn=="y")
   {
      BackupSmbout();  //do a backup then add user
      *(empptr+noRec)=newEmpId;
      *(qptr+noRec)=newQid;
      noRec++;      
   }
   else
   {
      cout << endl << strError << endl;
   }

   return noRec;
}
void ShowMenu()
{
   system("clear");
   cout << "**********************************SMB ID MAP**********************************" << endl;
   cout << "*                                                                            *" << endl;
   cout << "*                                  Main Menu                                 *" << endl;
   cout << "*                                     V1R8                                   *" << endl;
   cout << "*                                  Feb. 2007                                 *" << endl;
   cout << "*                               Created Oct. '06                             *" << endl;
   cout << "******************************************************************************" << endl;
   cout << endl << "  1. Display SMB ID Map\t\t\t 10. Help" << endl;
   cout << "  2. Add to SMB ID Map" << endl;;
   cout << "  3. Backup SMB ID Map" << endl;
   cout << "  4. Remove from SMB ID Map" << endl;
   cout << "  5. Search SMB ID Map" << endl;      
   cout << "  6. Show SMB ID Map Duplicate Summary" << endl;      
   cout << "  7. Exit the SMB ID Map Application" << endl << endl;
   cout << "Choice : ";
}
void PressEnter()
{
   string dummy;
   cout << "Press the Enter key to continue\n";       
   getline (cin.ignore(1000, '\n'), dummy); //cin.ignore(1000, '\n');
}
void ShowDuplicatesSmbin(string *empptr, string *qptr, int noRec)
{                               
   int x,y; //dosn't delete duplicates yet
   system("clear");
   cout << "Duplicate Summary: " << endl << endl;
   int cnt=0;
   string yn;
   for (x=0;x<noRec;x++){
      for (y=x+1;y<noRec;y++){
         if (*(empptr+x) == *(empptr+y)){
            printf("%03d",x);
            cout << "E: " << *(empptr+x) << "\t\t" << *(qptr+x) << "\t\t";                                                         
            printf("%03d",y);
            cout << ": " << *(empptr+y) << "\t\t" << *(qptr+x) << endl;       
            cnt++;
         }
         if (*(qptr+x) == *(qptr+y)){
            printf("%03d",x);
            cout  << "H: " << *(empptr+x) << "\t\t" << *(qptr+x) << "\t\t";                                                        
            printf("%03d",y);
            cout << " : " << *(empptr+y) << "\t\t" << *(qptr+x) << endl;      
            cnt++;
         }
      }
      if (cnt>=14){//|| noRec==(x+1)){
         yn=ContinueYN();
         if (yn=="n") break;
         cnt=0;
      }
   }
   if (cnt>=0) cout << endl;   
  // cout << "Duplicate analysis finished" << endl;
}
int  RemoveUser(string *empptr, string *qptr, int noRec)
{
   int x,y,z,start;
   string empid,qid,yn; 
   cout << "Enter a valid lan/employee id" << endl;
   cin  >> empid;
   cout << "Enter a valid host qid" << endl;
   cin  >> qid;
   qid=StringToLower(qid);
   empid=StringToLower(empid);
   start=-1;
   for (x=0;x<noRec;x++)
   {
      if (*(empptr+x)== empid && *(qptr+x)==qid)                       
      {
         BackupSmbout(); //user found so backup b4 remove
         start=x;
         cout << "Are you sure you want to remove user " << empid << "/" << qid << "(y/n)?" << endl;
         cin >> yn;
         if (yn !="y") start=-1; //user will not be removed
         break;
      }
   }
   if (start !=-1){
      noRec--;   
      for (x=start;x<noRec;x++)
      {
         *(empptr+x) = *(empptr+x+1);
         *(qptr+x) = *(qptr+x+1);
      }
      *(empptr+noRec)="";
      *(qptr+noRec)="";
   }
   return noRec;
}
string ContinueYN(){
   string yn;
   cout << endl << "Continue (y/n)" << endl;
   cin >> yn;
   system("clear");
   return yn;
}
void SearchSmbin(string *empptr, string *qptr, int noRec)
{
   int x,cnt,tcnt;
   string yn;
   string search,empid,qid;
   cout << "Enter a search term (1 word): " << endl;
   cin >> search;
   search=StringToLower(search);
   cnt=0;
   tcnt=0;
   system("clear");
   cout << "SMB Search Results: " << endl << endl;
   for (x=0;x<noRec;x++){  
      empid=*(empptr+x);
      empid=StringToLower(empid);
      qid=*(qptr+x);
      qid=StringToLower(qid);
      if(empid.find(search,0) != string::npos || qid.find(search,0) != string::npos){
         printf("%03d",x);
         cout << ": " << *(empptr+x) << "\t"<< *(qptr+x) << endl;                     
         cnt++;
      }
      if (cnt>=14){ //|| noRec==(x+1)){
         yn=ContinueYN();
         tcnt+=cnt;    
         cnt=0;
         if (yn=="n") break;
      }
      else if (noRec==(x+1)){
         tcnt+=cnt;
      }
   }
   //if (cnt!=0 && tcnt==0) tcnt=cnt;
   cout << endl << "Search returned " << tcnt << " users." << endl;  
}
string StringToLower(string str){
   int y;
   for (y=0;y<str.length();y++){
      str[y]=tolower(str[y]);
   }
   return str;
}
void CheckQID(string qid)
{
   string x;
   cout << "Function not yet implemented";
   //system("tso lu q34jvvw > tmpcmd.out");
   //ifstream cmdIn ("./tmpcmd.out");
 //while (!cmdIn.eof())
 //{
 //     cmdIn >> x;
 //}
 //  cmdIn.close();
 //  cout << x;
}
void SortFileNames(string *fldname, int i)
{
   int x,y;
   string tmp="";
   for(x=0;x<i;x++) //sort
   {
      for(y=x+1;y<i;y++)
      {
         if(*(fldname+x) < *(fldname+y))
         {
            tmp= *(fldname+x);
             *(fldname+x)= *(fldname+y);
            *(fldname+y)=tmp;
         }
      }
   }
}
int DirLen(char* opndir)
{
   int dl=0; //dir lenght function start (dir string length)
   for(;;)  
   {  
      if (*(opndir+dl)==NULL) break;      
      dl++;      
   } //dir lenght function end  
   return dl;
}
string* ReadFileNames(char* opndir,string* fldname) //from dir to get filenames
{
   DIR *pdir;
   struct dirent *pent;
   pdir=opendir(opndir); //opens directory           
   i=0; //number of files found                                      
   while ((pent=readdir(pdir)))  
   {
       i++;  
   } 
   fldname = new string [i];              
   i=0; 
   pdir=opendir(opndir); 
   while ((pent=readdir(pdir)))  
   {
      *(fldname+i)= (pent->d_name); //reads files  
      //cout << i << " " << *(fldname+i) << endl;
       i++;  
    }
    closedir(pdir);
    return fldname;
}
void DeleteOldBackups()
{
   int x,y,dirlen; //temps & length of dir below
   string tempstr;
   char opndirchar[256]="/XAS2/etc/dfs/home/dfskern/smbbkp/";
   char* opndir(&opndirchar[0]); //ptr to above
   string* fldname= new string [1]; //filename array ptrs
   dirlen=DirLen(opndir); //gets dirlen from DirLen fnct
   fldname=ReadFileNames(opndir,fldname);
   SortFileNames(fldname,i);
   for(x=12;x<i;x++) //12 is 10 plus . and ..start at whe
   {
      tempstr=fldname[x]; //shows files to be deleted
      for(y=0;y<tempstr.length();y++)
      {
         opndirchar[dirlen+y]=tempstr[y];
      }
      opndirchar[dirlen+y]='\0';
      remove(opndirchar);
   }
}
