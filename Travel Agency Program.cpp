#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
using namespace std;


struct DB
{
	string destination,airline;
	int fromDate, toDate, price, dealEnds, quotaLeft;
	bool search;

};


struct DB2
{
	string countryName;
	int ticketsPurchased;
	unsigned int revenueCollected;
};

void all(int , DB[]);
void today(int&, DB[]);
void buy(int&, DB[], DB2[]);
void cleardb(int , DB[]);
void search(int , DB[]);
void prefer(int ,DB []);
void no(int , DB []);
void add(int&, DB[]);
int changeMonth(string);
void databaseEdit(int&, DB[], DB2[]);
void database2Edit(string, int, int, DB2[]);
void most(DB2 database2[]);
void fillDatabase(int&, DB[]);
void checkZero(int&, DB[]);
void fillDatabase2(DB2[]);


int current=0;



int main()
{

	int n=0;
	string command,dummy;

	DB database[500];
	DB2 database2[500];

	fillDatabase(n,database);

	//ifstream fil2;
	fillDatabase2(database2);
	databaseEdit(n, database, database2);
	checkZero(n,database);
	cout<<endl<<"add promo 'filename' | all | today yyyymmdd | clear | search dest xxx | search date yyyymmdd | prefer airline xxx ";
	cout<<"| no airline xxx |  buy #k n | buy dest-airline-fromdate-todate-price n | most | end"<<endl;
	cin>>command;

	while(command!="end")
	{
		if(command=="all")
			all(n, database);

		else if(command=="today")
			today(n,database);

		else if(command=="buy")
			buy(n, database, database2);

		else if(command=="clear")
			cleardb(n, database);

		else if(command=="search")
			search(n, database);
		else if(command=="prefer")
			prefer(n, database);
		else if(command=="no")
			no(n,database);
		else if(command=="add")
			add(n,database);

		else if(command=="most")
			most(database2);


		else
		{
			cout<<"Enter correct choice, please try again."<<endl;
			getline(cin,dummy);
		}


		cout<<endl<<"add promo 'filename' | all | today yyyymmdd | clear | search dest xxx | search date yyyymmdd | prefer airline xxx ";
		cout<<"| no airline xxx |  buy #k n | buy dest-airline-fromdate-todate-price n | most | end"<<endl;
		cin>>command;
	}//end of while


}//end of main




void fillDatabase2(DB2 database2[])
{


	string countryNameToCheck,dummy,line,line2;
	int numberOfTickets, revenueCollected;
	ifstream fin;
	fin.open("intermediate2.txt");
	if(fin.fail())
	{
		fstream fil;
		fil.open("intermediate2.txt");
		fil.close();
	}

	else
	{
		getline(fin,line);
		while(fin.good())
		{
			getline(fin,countryNameToCheck,',');
			getline(fin,dummy,',');
			stringstream temp;
			temp<<dummy;
			temp>>numberOfTickets;
			getline(fin,dummy);
			stringstream temp2;
			temp2<<dummy;
			temp2>>revenueCollected;

			//cout<<countryNameToCheck<<endl;
			//cout<<numberOfTickets<<endl;
			//cout<<revenueCollected<<endl;

			database2[current].countryName=countryNameToCheck;
			database2[current].ticketsPurchased=numberOfTickets;
			database2[current].revenueCollected=revenueCollected;
			current++;
		}
	}
	fin.close();
}


void checkZero(int& n, DB database[])
{
	for(int i=0;i<n;i++)
	{
		if(database[i].quotaLeft <= 0)
		{
			for(int j=i;j<n-1; j++)
				database[j]=database[j+1];
			n--;
			i=i-1;
		}
	}

}


void fillDatabase(int& n, DB database[])
{
		ifstream fil;
		string dummy,line,command,line2;
		char comma;
		fil.open("mainDB.csv");
		if(fil.fail())
		{
			cout<<"Problem loading file, try again."<<endl;
			exit(1);
		}

		getline(fil,dummy);
		while(fil.good())
		{
			getline(fil,line);
			stringstream temp;
			temp<<line;
			getline(temp, database[n].destination,',');
			getline(temp,database[n].airline,',');
			temp>>database[n].fromDate>>comma>>database[n].toDate>>comma>>database[n].price>>comma;
			temp>>database[n].dealEnds>>comma>>database[n].quotaLeft;
			database[n].search=false;
			n++;
		}

		fil.close();
}





void databaseEdit(int& n, DB database[], DB2 database2[] )
{

	ifstream fil;
	string destinationToCheck,airlineToCheck,dummy,line;
	int fromDateToCheck,toDateToCheck,priceToCheck,dealEndsToCheck,quotaToDeduct,todayDate,quotaToAdd;
	char type;
	fil.open("intermediate1.txt");

	if(fil.fail())
	{
		ofstream fil2;
		cout<<"Creating a new intermediate file called 'intermediate1.txt'"<<endl;
		fil2.open("intermediate1.txt");
		fil2.close();
	}

	else
	{
		while(fil>>type)
		{
			if(type=='b')
			{
				fil>>destinationToCheck;
				fil>>airlineToCheck;
				getline(fil,dummy);
				stringstream temp;
				temp<<dummy;
				temp>>fromDateToCheck>>toDateToCheck>>priceToCheck>>dealEndsToCheck>>quotaToDeduct;
				temp>>dummy;
				for(int i=0;i<n;i++)
					if(database[i].destination==destinationToCheck && database[i].airline==airlineToCheck && database[i].toDate==toDateToCheck && database[i].fromDate == fromDateToCheck && database[i].price== priceToCheck && database[i].dealEnds==dealEndsToCheck)
						database[i].quotaLeft=database[i].quotaLeft-quotaToDeduct;
			}

			if(type=='t')
			{
				fil>>dummy;
				stringstream temp;
				temp<<dummy;
				temp>>todayDate;
				for(int i=0;i<n;i++)
				{
					if(database[i].dealEnds < todayDate)
						{
							for(int j=i;j<n-1; j++)
								database[j]=database[j+1];
							n--;
							i=i-1;
						}
				}
			}


			if(type=='a')
			{


				string airline,dummy,monthDummy,fileName,line,dayDummy,destination,priceDummy,quotaDummy;
				int year,month,day, dealEnds,fromDate,pos,pos2,toDate,price,counter=1;
				fil>>fileName;

				ifstream fil2;
				fil2.open(fileName.c_str());



				int lineNumber=1;

				getline(fil2,line);

				while(fil2.good())
				{

					int found=line.find('#');
					if(found==0)
						counter=0;

					if(lineNumber==1 && counter)
					{
						airline= line.substr(8,2);
					}


					if(lineNumber==2 && counter)
					{
						dummy=line.substr(10,11);
						monthDummy=line.substr(15,3);
						dayDummy=line.substr(19);
						stringstream temp;
						temp<<dummy;
						temp>>year;

						stringstream temp2;
						temp2<<dayDummy;
						temp2>>day;
						month=changeMonth(monthDummy);
						dealEnds=(year*10000)+(month*100)+day;

					}


					if(lineNumber>=3 && counter)
					{
						int counter2=0;
						destination=line.substr(0,3);
						dummy=line.substr(4,11);
						monthDummy=line.substr(9,3);
						dayDummy=line.substr(13,2);

						stringstream temp;
						temp<<dummy;
						temp>>year;

						stringstream temp2;
						temp2<<dayDummy;
						temp2>>day;
						month=changeMonth(monthDummy);
						fromDate=(year*10000)+(month*100)+day;

						pos=line.find("to");
						pos=pos+3;
						pos2=line.find("HKD");
						pos2--;
						dummy=line.substr(pos, pos2-pos);

						stringstream temp3;
						temp3<<dummy;
						temp3>>year;

						monthDummy=dummy.substr(5,3);

						month=changeMonth(monthDummy);
						dayDummy= dummy.substr(9);

						stringstream temp4;
						temp4<<dayDummy;
						temp4>>day;

						toDate=(year*10000)+(month*100)+day;

						pos=line.find("quota");
						pos=pos-1;
						pos2=pos2+5;
						priceDummy= line.substr(pos2, pos-pos2);


						stringstream temp5;
						temp5<<priceDummy;
						temp5>>price;

						pos=pos+7;
						quotaDummy=line.substr(pos);

						stringstream temp6;
						temp6<<quotaDummy;
						temp6>>quotaToAdd;


						//cout<<airline<<" "<<destination<<" "<<fromDate<<" "<<toDate<<" "<<price<< " "<<dealEnds<<" "<<quotaToAdd<<endl;

						for(int i=0; i<n;i++)
						{
							if(database[i].airline==airline && database[i].destination==destination && database[i].toDate==toDate && database[i].fromDate==fromDate && database[i]. price==price && database[i].dealEnds ==dealEnds)
							{
								database[i].quotaLeft=database[i].quotaLeft +quotaToAdd;
								i=n;
								counter2=1;

							}
						}

						if(counter2==0)
						{
							database[n].airline=airline;
							database[n].destination=destination;
							database[n].toDate=toDate;
							database[n].fromDate=fromDate;
							database[n].price=price;
							database[n].dealEnds=dealEnds;
							database[n].quotaLeft=quotaToAdd;
							n++;

						}

					}

					if(!counter)
					{
						lineNumber=1;
						counter=1;
					}

					lineNumber++;
					getline(fil2,line);

				}

				fil2.close();


			}
		}
	}
	fil.close();
}




void database2Edit(string destination, int numberOfTickets, int price, DB2 database2[])
{
	ifstream fil;
	fil.open("airportcode.txt");
	string line,destinationToCheck,countryNameToCheck,dummy;
	int revenueCollected, counter=0;

	while(getline(fil,line))
	{
		destinationToCheck= line.substr(0,3);
		if(destinationToCheck==destination)
			countryNameToCheck=line.substr(4);
	}
	//cout<<countryNameToCheck<<endl;

	fil.close();

	ifstream fin;
	fin.open("intermediate2.txt");


		if(current==0)
		{
			database2[current].countryName=countryNameToCheck;
			database2[current].ticketsPurchased=numberOfTickets;
			database2[current].revenueCollected=0.9*numberOfTickets*price;
			counter=1;
			current++;
		}

		if(counter==0)
		{
			int counter2=0;
			for(int i=0;i<current;i++)
			{
				if(database2[i].countryName==countryNameToCheck)
				{
					revenueCollected=0.9*numberOfTickets*price;;
					database2[i].revenueCollected=database2[i].revenueCollected+revenueCollected;
					database2[i].ticketsPurchased=database2[i].ticketsPurchased+numberOfTickets;
					counter2=1;
				}
			}

			if(counter2==0)
			{
				database2[current].countryName=countryNameToCheck;
				database2[current].ticketsPurchased=numberOfTickets;
				database2[current].revenueCollected=revenueCollected=0.9*numberOfTickets*price;
				current++;
			}
		}
	fin.close();

	ofstream fout;
	fout.open("intermediate2.txt");

	for(int i=0;i<current;i++)
		{
			//cout<<database2[i].countryName<<" "<<database2[i].ticketsPurchased<<" "<<database2[i].revenueCollected<<endl;
			fout<<endl<<database2[i].countryName<<","<<database2[i].ticketsPurchased<<","<<database2[i].revenueCollected;
		}
	fout.close();

}



void all(int n, DB database[])
{
	int i;
	for( i=0; i<n; i++)
		cout<<"["<<i<<"]"<<" "<<database[i].destination<<","<<database[i].airline<<","<<database[i].fromDate<<","<<database[i].toDate<<","<<database[i].price<<","<<database[i].dealEnds<<","<<database[i].quotaLeft<<endl;
	cout<<"Total Records :"<<i<<endl;
}




void today(int& n,DB database[])
{
	int dateToCheck;
	cin>>dateToCheck;

	for(int i=0;i<n;i++)
	{

		if(database[i].dealEnds < dateToCheck)
		{
			for(int j=i;j<n-1; j++)
			{
				database[j]=database[j+1];
			}
			n--;
			i=i-1;
		}

	}
	fstream fil;
	fil.open("intermediate1.txt",ios::app);
	fil<<"t"<<" "<<dateToCheck<<endl;
	fil.close();

}



void buy(int& n, DB database[], DB2 database2[])
{
	string destinationToCheck, airlineToCheck,line;
	int fromDateToCheck, toDateToCheck,numberOfTickets,priceToCheck, searchNumber;
	char dummy;
	getline(cin,line);
	line=line.substr(1);  //since enter is counted as a character, we need to remove it

	int pos=line.find('#');

	if(pos==-1)
	{
		destinationToCheck=line.substr(0,3);
		airlineToCheck=line.substr(4,2);

		line=line.substr(7);//remaining line

		stringstream temp;
		temp<<line;
		temp>>fromDateToCheck>>dummy>>toDateToCheck>>dummy;

		getline(temp,line);

		stringstream temp2;
		temp2<<line;
		temp2>>priceToCheck>>numberOfTickets;

		for(int i=0;i<n;i++)
		{
			if(database[i].destination==destinationToCheck && database[i].airline==airlineToCheck && database[i].toDate==toDateToCheck && database[i].fromDate == fromDateToCheck && database[i].price== priceToCheck)
			{
				database[i].quotaLeft=database[i].quotaLeft -numberOfTickets;
				if(database[i].quotaLeft < 0)
				{
					database[i].quotaLeft=database[i].quotaLeft+numberOfTickets;
					cout<<"Not Enough Tickets"<<endl;
				}
				else
				{
					cout<<numberOfTickets<<" ticket(s) purchased"<<endl;
					database2Edit(destinationToCheck, numberOfTickets, priceToCheck, database2);
					fstream fil;
					fil.open("intermediate1.txt",ios::app);
					fil<<"b"<<" "<<destinationToCheck<<" "<<airlineToCheck<<" "<<fromDateToCheck<<" "<<toDateToCheck<<" "<<priceToCheck<<" "<<database[i].dealEnds<<" "<<numberOfTickets<<endl;
					fil.close();
				}
			}
		}


		checkZero(n,database);

	}//end of if

	else             //the second type of buy command used with search
	{
		stringstream temp;
		temp<<line;
		temp>>dummy>>searchNumber>>numberOfTickets;
		int numberForDisplay=0;

		for(int i=0; i<n;i++)
		{
			if(database[i].search)
			{
				numberForDisplay++;
				if(numberForDisplay==searchNumber)
				{
					cout<<"<"<<numberForDisplay<<">"<<" "<<database[i].destination<<","<<database[i].airline<<","<<database[i].fromDate<<","<<database[i].toDate<<","<<database[i].price<<","<<database[i].dealEnds<<","<<database[i].quotaLeft<<" "<<database[i].search<<endl;
					database[i].quotaLeft=database[i].quotaLeft -numberOfTickets;
					if(database[i].quotaLeft < 0)
					{
					database[i].quotaLeft=database[i].quotaLeft+numberOfTickets;
					cout<<"Not Enough Tickets"<<endl;
					}
					else
					{
						cout<<numberOfTickets<<" ticket(s) purchased"<<endl;
						database2Edit(database[i].destination,numberOfTickets, database[i].price, database2);
						fstream fil;
						fil.open("intermediate1.txt",ios::app);
						fil<<"b"<<" "<<database[i].destination<<" "<<database[i].airline<<" "<<database[i].fromDate<<" "<<database[i].toDate<<" "<<database[i].price<<" "<<database[i].dealEnds<<" "<<numberOfTickets<<endl;
						cleardb(n,database);
						fil.close();

					}


				}

			}


		}

		checkZero(n,database);


		if(numberForDisplay==0)
				cout<<"Wrong input, try again"<<endl;
	}
}//end of buy









void cleardb(int n, DB database[])
{
	for(int i=0;i<n;i++)
	{
		database[i].search=false;
	}
}











void search(int n, DB database[])
{

	int counter=0,dateToCheck;
	string searchType,destinationToCheck;
	int numberForDisplay=0;
	string line;
	getline(cin,line);
	line=line.substr(1);					//need to get rid of enter
	searchType=line.substr(0,4);

	for(int i=0;i<n;i++)
	{
		if(database[i].search)
			counter=1;
	}


	if(!counter)
	{
		if(searchType=="dest")
		{
			destinationToCheck=line.substr(5);
			cout<<"Loading Search Results..."<<endl;
			for(int i=0;i<n;i++)
			{
				if(database[i].destination==destinationToCheck)
				{
					database[i].search=true;
				}
				if(database[i].search)
				{
					numberForDisplay++;
					cout<<"<"<<numberForDisplay<<">"<<" "<<database[i].destination<<","<<database[i].airline<<","<<database[i].fromDate<<","<<database[i].toDate<<","<<database[i].price<<","<<database[i].dealEnds<<","<<database[i].quotaLeft<<endl;
				}


			}

			if(numberForDisplay==0)
				cout<<"Sorry! No such results.."<<endl;


		}

		else if(searchType=="date")
		{
			line=line.substr(5);

			stringstream temp;
			temp<<line;
			temp>> dateToCheck;
			cout<<"Loading Search Results..."<<endl;
			for(int i=0;i<n;i++)
			{
				if(database[i].fromDate <= dateToCheck  && dateToCheck <= database[i].toDate)
				{
					database[i].search=true;
				}
				if(database[i].search)
				{
					numberForDisplay++;
					cout<<"<"<<numberForDisplay<<">"<<" "<<database[i].destination<<","<<database[i].airline<<","<<database[i].fromDate<<","<<database[i].toDate<<","<<database[i].price<<","<<database[i].dealEnds<<","<<database[i].quotaLeft<<endl;
				}


			}
			if(numberForDisplay==0)
				cout<<"Sorry! No such results.."<<endl;

		}


	}

	else
		cout<<endl<<"Kindly use clear command before starting a new search"<<endl;
}







void prefer(int n,DB database[])
{
	string line,airlineToCheck; int numberForDisplay=0;
	int counter=0;
	getline(cin, line);
	line=line.substr(1);
	airlineToCheck= line.substr(8);
	cout<<airlineToCheck;

	for(int i=0;i<n;i++)
		if(database[i].search)
			counter=1;

	if(!counter)
		cout<<"Kindly search first..."<<endl;

	else
	{
		cout<<"Refining search results according to airline preference.."<<endl;
		for(int i=0;i<n;i++)
		{
			if(database[i].search)
			{
				if(database[i].airline==airlineToCheck)
				{
					numberForDisplay++;
					cout<<"<"<<numberForDisplay<<">"<<" "<<database[i].destination<<","<<database[i].airline<<","<<database[i].fromDate<<","<<database[i].toDate<<","<<database[i].price<<","<<database[i].dealEnds<<","<<database[i].quotaLeft<<endl;

				}
				else
				{
					database[i].search=false;
				}
			}


		}

		if(numberForDisplay==0)
			cout<<"Sorry no such results.."<<endl;

	}


}



void no(int n, DB database[])
{
	string line, airlineToCheck;
	int numberForDisplay=0;
	int counter=0;
	getline(cin,line);
	line=line.substr(1);
	airlineToCheck= line.substr(8);

	for(int i=0;i<n;i++)
		if(database[i].search)
			counter=1;

	if(!counter)
		cout<<"Kindly search first..."<<endl;

	else
	{
		cout<<"Refining search results according to airline preference.."<<endl;
		for(int i=0;i<n;i++)
		{
			if(database[i].search)
			{
				if(database[i].airline!=airlineToCheck)
				{
					numberForDisplay++;
					cout<<"<"<<numberForDisplay<<">"<<" "<<database[i].destination<<","<<database[i].airline<<","<<database[i].fromDate<<","<<database[i].toDate<<","<<database[i].price<<","<<database[i].dealEnds<<","<<database[i].quotaLeft<<endl;

				}
				else
				{
					database[i].search=false;
				}
			}
		}
		if(numberForDisplay==0)
			cout<<"Sorry no such results.."<<endl;
	}

}




void add(int& n, DB database[])
{
	ifstream fil;

	string airline,dummy,monthDummy,fileName,line,dayDummy,destination,priceDummy,quotaDummy;
	int year,month,day, dealEnds,fromDate,pos,pos2,toDate,price,quota,counter=1,counter3=1;

	cin>>dummy;
	getline(cin,fileName);
	fileName=fileName.substr(1);
	pos= fileName.find('.');

	if(pos!=-1)
		fil.open(fileName.c_str());
	else
	{
		fileName=fileName+".txt";
		fil.open(fileName.c_str());
	}

	cout<<fileName<<endl<<endl<<endl;

	if(fil.fail())
		cout<<"Problem loading file, create file first and try again.."<<endl;

	int lineNumber=1;

	getline(fil,line);

	while(fil.good())
	{

		int found=line.find('#');
		if(found==0)
			counter=0;

		if(lineNumber==1 && counter)
		{
			airline= line.substr(8,2);
		}


		if(lineNumber==2 && counter)
		{
			dummy=line.substr(10,11);
			monthDummy=line.substr(15,3);
			dayDummy=line.substr(19);
			stringstream temp;
			temp<<dummy;
			temp>>year;

			stringstream temp2;
			temp2<<dayDummy;
			temp2>>day;
			month=changeMonth(monthDummy);
			dealEnds=(year*10000)+(month*100)+day;

		}


		if(lineNumber>=3 && counter)
		{
			int counter2=0;
			destination=line.substr(0,3);
			dummy=line.substr(4,11);
			monthDummy=line.substr(9,3);
			dayDummy=line.substr(13,2);

			stringstream temp;
			temp<<dummy;
			temp>>year;

			stringstream temp2;
			temp2<<dayDummy;
			temp2>>day;
			month=changeMonth(monthDummy);
			fromDate=(year*10000)+(month*100)+day;

			pos=line.find("to");
			pos=pos+3;
			pos2=line.find("HKD");
			pos2--;
			dummy=line.substr(pos, pos2-pos);

			stringstream temp3;
			temp3<<dummy;
			temp3>>year;

			monthDummy=dummy.substr(5,3);

			month=changeMonth(monthDummy);
			dayDummy= dummy.substr(9);

			stringstream temp4;
			temp4<<dayDummy;
			temp4>>day;

			toDate=(year*10000)+(month*100)+day;

			pos=line.find("quota");
			pos=pos-1;
			pos2=pos2+5;
			priceDummy= line.substr(pos2, pos-pos2);


			stringstream temp5;
			temp5<<priceDummy;
			temp5>>price;

			pos=pos+7;
			quotaDummy=line.substr(pos);

			stringstream temp6;
			temp6<<quotaDummy;
			temp6>>quota;


			cout<<airline<<" "<<destination<<" "<<fromDate<<" "<<toDate<<" "<<price<< " "<<dealEnds<<" "<<quota<<endl;

			for(int i=0; i<n;i++)
			{
				if(database[i].airline==airline && database[i].destination==destination && database[i].toDate==toDate && database[i].fromDate==fromDate && database[i]. price==price && database[i].dealEnds ==dealEnds)
				{
					database[i].quotaLeft=database[i].quotaLeft +quota;
					i=n;


					if(counter3==1)
					{
						fstream fil;
						fil.open("intermediate1.txt",ios::app);
						fil<<"a"<<" "<<fileName<<endl;
						fil.close();
						counter3++;
					}
					counter2=1;

				}
			}

			if(counter2==0)
			{
				database[n].airline=airline;
				database[n].destination=destination;
				database[n].toDate=toDate;
				database[n].fromDate=fromDate;
				database[n].price=price;
				database[n].dealEnds=dealEnds;
				database[n].quotaLeft=quota;
				n++;
				if(counter3==1)
					{
						fstream fil;
						fil.open("intermediate1.txt",ios::app);
						fil<<"a"<<" "<<fileName<<endl;
						fil.close();
						counter3++;
					}
			}

		}

		if(!counter)
		{
			lineNumber=1;
			counter=1;
		}

		lineNumber++;
		getline(fil,line);

	}//end of while

	fil.close();

}//end of function


void most(DB2 database2[])
{
	DB2 max;

	max.ticketsPurchased=0;

	for(int i=0;i<current;i++)
	{
		if(database2[i].ticketsPurchased> max.ticketsPurchased)
		{
			max=database2[i];
		}
	}

	ofstream fil;
	fil.open("most.txt");

	fil<<max.countryName<<endl;
	fil<<"#"<<max.ticketsPurchased<<endl;
	fil<<"$"<<max.revenueCollected<<endl;

	fil.close();

}



int changeMonth(string monthDummy)
{
	int month;
	if(monthDummy=="Jan")
		month=1;
	if(monthDummy== "Feb")
		month=2;
	if(monthDummy== "Mar")
		month=3;
	if(monthDummy== "Apr")
		month=4;
	if(monthDummy== "May")
		month=5;
	if(monthDummy== "Jun")
		month=6;
	if(monthDummy== "Jul")
		month=7;
	if(monthDummy== "Aug")
		month=8;
	if(monthDummy== "Sep")
		month=9;
	if(monthDummy== "Oct")
		month=10;
	if(monthDummy== "Nov")
		month=11;
	if(monthDummy== "Dec")
		month=12;

	return month;
}
