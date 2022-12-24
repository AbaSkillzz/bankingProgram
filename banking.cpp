#include <iostream>
#include <iomanip> //to set 2 decimals in balance
#include <fstream>
#include <cstring>
#include <sstream>

//function prototypes(declaration)
void showBalance(double *balance);
void deposit(double *balance, double amount);
void withdraw(double *balance, double amount);
void addTransactionToFile(char trx[50]);
void getAllTransactions();
void getLastNTransactions(int n);
int numberOfTransactions();

int main(){
   double balance = 100;
   int action;
   double amount; //for case 2 and 3
   char transaction[50]; //single transation, used to store each trx
   std::string trxString;
   std::stringstream ss;
   //std::stringstream ss2;

   std::cout<<"Banking actions: 1:show the balance | 2:deposit | 3:withdraw | 4:get transaction list | 0:exit the banking interface"<<std::endl;
   while(true){
      std::cout<<"Banking action(input 0-4): ";
      std::cin>>action;

      switch(action){
         case 0:
            std::cout<<"Exiting banking interface, thanks for choosing us."<<std::endl;
            return 0;

         case 1:
            showBalance(&balance);
            break;

         case 2:
            std::cout<<"Amount to deposit: ";
            std::cin>>amount;
            deposit(&balance, amount);
            //trx string creation
            ss.str(std::string());
            ss<<amount; //assigning amount to ss
            trxString = "Deposited " + ss.str() + "$\n";
            strcpy(transaction, trxString.c_str()); //string to char_arr
            addTransactionToFile(transaction); 
          
            std::cout<<"Your new balance is: ";
            showBalance(&balance);
            break;

         case 3:
            do{
               std::cout<<"Amount to withdraw: ";
               std::cin>>amount;
               if(amount > balance){
                  std::cout<<"Requsting to withdraw more than balance! Must withdraw lower amount."<<std::endl;
               }
            }while(amount > balance);
            withdraw(&balance, amount);   
            //trx string
            ss.str(std::string());
            ss<<amount;
            trxString = "Withdraw of " + ss.str() + "$\n";

            strcpy(transaction, trxString.c_str());
            addTransactionToFile(transaction);

            std::cout<<"Your new balance is: ";
            showBalance(&balance);
            break;
         
         case 4:
            int choice;
            do{
               std::cout<<"1: to access to all transactions | 2: to access last n transactions: ";
               std::cin>>choice;
               if(choice!=1 && choice!=2){
                  std::cout<<"Entered invalid option, must choose 1 or 2!"<<std::endl;
               }
            }while(choice!=1 && choice!=2);
            
            if(choice == 1){
               getAllTransactions();
            }else{
               int n;
               int totalTrxN = numberOfTransactions();
               do{
                  std::cout<<"Number of last transactions you want to see(must be less than total trx number "<<"("<<totalTrxN<<")"<<"): ";
                  std::cin>>n;
               }while(n > totalTrxN);
               getLastNTransactions(n);
            }

            break;

         default:
            std::cout<<"Choosed an invalid banking action!"<<std::endl;
      }   
   }
}

//function definitions
void showBalance(double *balance){
   std::cout<<std::setprecision(2)<<std::fixed<<*balance<<"$"<<std::endl;
}

void deposit(double *balance, double amount){
   *balance += amount;
}

void withdraw(double *balance, double amount){
   *balance -= amount;
}

void addTransactionToFile(char transaction[50]){
   std::ofstream outf("transactions.bin", std::ios::binary | std::ios::app);
   char trx[50];
   strcpy(trx, "");
   strcpy(trx, transaction);

   outf.write((char*)&trx, sizeof(trx)); //*trx, dereferencing trx, arr are passed by reference
   outf.close();
}

//option 4
void getAllTransactions(){
   std::ifstream inf("transactions.bin", std::ios::binary);
   char trx[50];
   if(inf.is_open()){
      while(inf){
         strcpy(trx, "");
         inf.read((char*)&trx, sizeof(trx));
         std::cout<<trx<<std::endl;
      }
   }else{
      std::cout<<"Error occured while opening transations file!"<<std::endl;
   }
   inf.close();
}

int numberOfTransactions(){
   std::ifstream inf("transactions.bin", std::ios::binary);
   char trx[50];
   int count = 0;
   if(inf.is_open()){
      while(inf){
         strcpy(trx, "");
         inf.read((char*)&trx, sizeof(trx));
         count++;
      }
   }else{
      std::cout<<"Error occured while opening transations file!"<<std::endl;
   }
   inf.close();
   return count-1;
}

void getLastNTransactions(int n){
   std::ifstream inf("transactions.bin", std::ios::binary);
   char trx[50];

   if(inf.is_open()){
      //positioning back of buffer to read n trxs
      inf.seekg(-(n*sizeof(trx)), std::ios::end);
      for(int i=0; i<n; i++){
         inf.read((char*)&trx, sizeof(trx));
         std::cout<<trx<<std::endl;
      }
      inf.close();
   }else{
      std::cout<<"Error occured while opening transactions file!"<<std::endl;
   }
}