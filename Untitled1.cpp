#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <cmath>
#include <conio.h>

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::setprecision;
using std::to_string;
using std::ofstream;
using std::ifstream;
using std::ios;
using std::stringstream;


//returns the balance of the account
double Calculate_Balance(string Account_name)
{
	//opening the account
	ifstream file;
	file.open(Account_name.c_str());

	string date;
	string type;
	string transection_amount;
	string Balance;
	
	
	while(!file.eof())
	{
		file>>date>>type>>transection_amount>>Balance;
	}
	file.close();
	
	stringstream convert;
	convert<<Balance;
	
	double balance;
	convert>>balance;
	
	
	
	return balance;
}

//Diposit or withdraw 
double Newly_Added_Value(int Type, int Amount)
{
	if(Type==1)
	{
		return Amount;
	}
	return -Amount;
}

//creates accounts for each user
void Create_Accounts(vector <string> &All_Accounts)
{
	//creating a open file object
	ifstream file;
	file.open("balance.txt");
	
	//Deleting the file if it exists
	remove("BankAccount.txt");
	
	//Creating the bank
	ofstream BankAccountFile;
	BankAccountFile.open("BankAccount.txt");
	
	while(!file.eof())
	{

	
		//getting input from file into a string
		string tempstr;
		file>>tempstr;

		
		//passing the tempstr to a stringstream object
		stringstream convert;
		convert<<tempstr;
		
		//some variables to work with stringstream object
		
		int date;
		int Acc_Num;
		double Balance;
		char ch;
		
		//break the string into parts
		convert>>date>>ch>>Acc_Num>>ch>>Balance;
		
		//converting date and account number to strings
		string Date;		
		string ACC_NUM;
		
		Date=to_string(date);
		ACC_NUM=to_string(Acc_Num);
		ACC_NUM+=".txt";
		
		//putting account name in a vector for later reference
		bool Exists=true;
		for(int i=0;i<All_Accounts.size();i++)
		{
			if(All_Accounts[i]==ACC_NUM)
			{
				Exists=false;
			}
		}
		if(Exists)
		{
			All_Accounts.push_back(ACC_NUM);
		}
		
		
		
		//Deleting the account if it exists previously
		remove(ACC_NUM.c_str());
		
		//creating accounts for each
		ofstream create_file;
		create_file.open(ACC_NUM.c_str(), ios::app);
		
		//passing initial balance to files
		create_file<<Date<<endl<<0<<endl<<setprecision(12)<<Balance<<endl<<Balance<<endl;
		BankAccountFile<<Date<<endl<<0<<endl<<setprecision(12)<<Balance<<endl<<Calculate_Balance("BankAccount.txt")+Balance<<endl;
		
		create_file.close();
	}
	BankAccountFile.close();
	file.close();
}

//below 1000 and negative charges
void Charges_on_customer_account(vector <string> All_Accounts, int Date, string privious_Day)
{
	for(int i=0;i<All_Accounts.size();i++)
	{
		string ACC_NUM=All_Accounts[i];
		
		//for user account
		ofstream Account_File;
		Account_File.open(ACC_NUM.c_str(), ios::app);
		
		//For bank account
		ofstream BankAccountFile;
		BankAccountFile.open("BankAccount.txt", ios::app);
		
		//checking whether the day is over or not(if it is over we check 0<balance<1000 and balance<0 conditions
		if(Calculate_Balance(ACC_NUM)<=1000&&Calculate_Balance(ACC_NUM)>=0)  //0<balance<1000 condition
		{
			//removing from user account
			Account_File<<Date<<endl<<4<<endl<<setprecision(12)<<10<<endl<<Calculate_Balance(ACC_NUM)-10<<endl;
			//removing from bank
			BankAccountFile<<Date<<endl<<4<<endl<<setprecision(12)<<-10<<endl<<Calculate_Balance("BankAccount.txt")-10<<endl;
			//adding again into the bank account
			BankAccountFile<<Date<<endl<<4<<endl<<setprecision(12)<<+10<<endl<<Calculate_Balance("BankAccount.txt")+10<<endl;
		}	
		else
		{
			if(Calculate_Balance(ACC_NUM)<0)  //balance<0 condition
			{
				//removing from user account
				Account_File<<Date<<endl<<4<<endl<< setprecision(12)<<50<<endl<<Calculate_Balance(ACC_NUM)-50<<endl;
				//removing from bank
				BankAccountFile<<Date<<endl<<4<<endl<<setprecision(12)<<-50<<endl<<Calculate_Balance("BankAccount.txt")-50<<"   "<<endl;
				//adding again into the bank account
				BankAccountFile<<Date<<endl<<4<<endl<<setprecision(12)<<+50<<endl<<Calculate_Balance("BankAccount.txt")+50<<"   "<<endl;
			}			
		}
		
		if(Calculate_Balance(ACC_NUM)>0)  //positive balance condition
		{
			double adding_value=Calculate_Balance(ACC_NUM);
			adding_value*=0.02;
			adding_value*=100;
			long temp=round(adding_value);
			adding_value=float(temp);
			adding_value/=100;		
//			
//			//Adding to user account
			Account_File<<Date<<endl<<3<<endl<<setprecision(12)<<adding_value<<endl<<Calculate_Balance(ACC_NUM)+adding_value<<endl;
//			//removing from bank
			BankAccountFile<<Date<<endl<<3<<endl<<setprecision(12)<<-adding_value<<endl<<Calculate_Balance("BankAccount.txt")-adding_value<<endl;

		}	
		
		
		Account_File.close();
		BankAccountFile.close();	
	}
}

//make all transections, interests and charges
void Bank_Work_function(vector <string> All_Accounts)
{	
	//creating a open file object
	ifstream file;
	file.open("transaction.txt");

	//Creating the bank
	ofstream BankAccountFile;
	BankAccountFile.open("BankAccount.txt", ios::app);
	
	string privious_Day="";

	
	while(!file.eof())
	{
		//getting input from file into a string
		string tempstr;
		file>>tempstr;

		
		//passing the tempstr to a stringstream object
		stringstream convert;
		convert<<tempstr;
		
		//some variables to work with stringstream object
		int date;
		int Acc_Num;
		int Type;
		double Amount;
		char ch;
		
		//break the string into parts
		convert>>date>>ch>>Acc_Num>>ch>>Type>>ch>>Amount;
		
		//converting date and account number to strings
		string Date;		
		string ACC_NUM;
		
		Date=to_string(date);
		ACC_NUM=to_string(Acc_Num);
		ACC_NUM+=".txt";
		
		
		if(privious_Day=="")
		{
			privious_Day=Date;
		}
		
		ofstream Account_File;
		Account_File.open(ACC_NUM.c_str(), ios::app);

		
		//Adding interests and charges on accounts
		if(privious_Day!=Date)
		{
			
			//There can be days which no transection is happend. we have to add interests and charges for those days too.
			date--;
			int Date_as_a_Number;
			stringstream convert;
			convert<<privious_Day;
			convert>>Date_as_a_Number;
			
			int date_differance=abs(Date_as_a_Number-date);
			
			for(int j=0;j<=date_differance;j++)
			{
				//  0<balance<1000   state and balance<0 state and positive balance states
				Charges_on_customer_account(All_Accounts, ++Date_as_a_Number, privious_Day);
			}
		}
		
		//entering the transection to correct accounts(bank and user's
		double Balance=Calculate_Balance(ACC_NUM) + Newly_Added_Value(Type, Amount);
		double Bank_Balance=Calculate_Balance("BankAccount.txt") + Newly_Added_Value(Type, Amount);
		
		Account_File<<Date<<endl<<Type<<endl<<setprecision(12)<<Newly_Added_Value(Type, Amount)<<endl<<Balance<<endl;
		BankAccountFile<<Date<<endl<<Type<<endl<<setprecision(12)<<Newly_Added_Value(Type, Amount)<<endl<<Bank_Balance<<endl;
		
		privious_Day=Date;
	
	}
	file.close();
}

//Gives outputs as user required
void Outputs(vector <string> All_Accounts)
{	
	while(true)
	{	
		cout<<endl<<endl<<"\t\t\t\t  urban development bank";
		cout<<endl<<endl;
		cout<<endl<<"\t\tShow Bank Valut balance  				: Press 1 "<<endl;
		cout<<endl<<"\t\tShow current balances of all the accounts 		: Press 2 "<<endl;
		cout<<endl<<"\t\tShow the Starting balance of YYYYMMDD date		: Press 3"<<endl;
		cout<<endl<<"\t\tShow the end of the day balance of YYYYMMDD date	: Press 4"<<endl;
		cout<<endl<<"\t\tQuit                                             	: Press 5"<<endl;
				
		char Input;
		cout<<"\n\t\t\t\t\tInput : ";
		cin>>Input;
		
		if(Input=='5')
		{
			break;
		}
		
		if(Input!='1'&&Input!='2'&&Input!='3'&&Input!='4')
		{
			cout<<"\n\n\t\t\t\tPlease enter a Valid input";
			getch();
			system("cls");
			continue;
		}
			
		//Perform as given input	
		if(Input=='1')
		{
			cout<<setprecision(12)<<"\n\t\t\t\tBank Valut Balance is : Rs "<<Calculate_Balance("BankAccount.txt");
		}
		if(Input=='2')
		{
			cout<<"\n\n\t\tEnter the number for access the account : "<<endl;
			for(int i=0;i<All_Accounts.size();i++)
			{
				cout<<"  \t\t  "<<All_Accounts[i]<<"\b\b\b\b     \t: "<<i<<endl;
			}
			int AccountNumber;
			cout<<"\n\t\t\t\t\tInput : ";
			cin>>AccountNumber;
			cout<<setprecision(12)<<"\n\n\t\t\t\t"<<All_Accounts[AccountNumber]<< "account Balance is : Rs "<<Calculate_Balance(All_Accounts[AccountNumber]);
		}
		if(Input=='3'||Input=='4')
		{
			cout<<"\n\n\t\tEnter the number for access the account : "<<endl;
			for(int i=0;i<All_Accounts.size();i++)
			{
				cout<<"  \t\t  "<<All_Accounts[i]<<"\b\b\b\b     \t: "<<i<<endl;
			}
			
			int AccountNumber;
			cout<<"\n\t\t\t\t\tInput : ";
			cin>>AccountNumber;
			
			cout<<"\n\n\t\tEnter the Date required in YYYYMMDD format (Do not include spaces or other characters)  : ";
			long Date_Required;
			cin>>Date_Required;
	
			//opening the account
			ifstream file;
			file.open(All_Accounts[AccountNumber].c_str());
			
			string date="", PreDate;
			string type="", PreType;
			string transection_amount="", PreTransection_amount;
			string Balance="", PreBalnce;
			
			long Date;
			while(!file.eof())
			{
				PreDate=date;
				PreType=type;
				PreTransection_amount=transection_amount;
				PreBalnce=Balance;
				
				file>>date>>type>>transection_amount>>Balance;
				
				stringstream convert;
				convert<<date;
				convert>>Date;
				
				if(Input=='3'&&Date==Date_Required)
				{
					break;
				}
				if(Input=='4'&&Date==Date_Required+1)
				{
					break;
				}
				
			}
			file.close();
			
			if(Input=='3') // If user wants Starting balance
			{
				cout<<setprecision(12)<<"\n\t\t\t\tStarting balance when "<<Date<<" Balance is : Rs "<<PreBalnce;
			}
			if(Input=='4') // If user wants Ending balance
			{
				cout<<setprecision(12)<<"\n\t\t\t\tStarting balance when "<<Date<<" Balance is : Rs "<<PreBalnce;
			}
		}
		getch();
		system("cls");	
	}
}

int main()
{
	//A vector to save account numbers for later reference
	vector <string> All_Accounts;
	
	//Accounts will be created for each user
	Create_Accounts(All_Accounts);	
	
	//Perform all account based processes and creates accounts
	Bank_Work_function(All_Accounts);
	
	//giving user outputs
	Outputs(All_Accounts);
	
	
	
	

	return 0;
}
