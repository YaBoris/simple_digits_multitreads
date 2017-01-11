#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <algorithm>
#include <Windows.h>
#include <clocale>

using namespace std;
const int THREAD_MAX = 8;
int iQueue = 0;
mutex iQueueMutex;
unsigned int iAllResult = 0;

bool check()
{
	lock_guard <mutex> guard(iQueueMutex);
	do
	{
		if(iQueue >= THREAD_MAX)
		{
			Sleep(50);
		}
		else
		{
			++iQueue;
			return true;
		}
	}while(true);
}


int multiple_calculate_simple_digit(unsigned int& new_digit, vector <unsigned int>& iResultIn)
{	
    bool x = false;	
	for(unsigned int i = 3; i <= new_digit/2; i++)
	{
		if(!(new_digit%i))
		{
			x = true;
			break;
		}
    }

	lock_guard<mutex> guard(iQueueMutex);
	if(!x)
	{
		iResultIn.push_back(new_digit);
		iAllResult++;
		cout << new_digit << " - new_simple digit" << endl;
	}
	--iQueue;

	return 0;
}

int single_calculate_simple_digit(unsigned int& new_digit, vector <unsigned int>& iResultIn)
{
	++iQueue;
    bool x = false;
	for(unsigned int uiSimple = 3; uiSimple <= new_digit; uiSimple+=2)
	{
		x = false;
		for(unsigned int i = 3; i <= uiSimple/2 ; i++)
		{
			if(!(uiSimple%i))
			{
				x = true;
				break;
			}
		}
		if(!x)
		{
			iResultIn.push_back(uiSimple);
			iAllResult++;
			cout << uiSimple << " - new_simple digit" << endl;
		}
	}
	--iQueue;
	return 0;
}


int main()
{   
	setlocale(LC_CTYPE, "rus");
    vector <unsigned int> iResult;
//    unsigned int iLimitCounter = SIZE_MAX;
	unsigned int iUpperLimitCounter = 100;
	unsigned int iLowerLimitCounter = 5;
	int iCountThreads = 0;

	cout << "Поиск всех простых чисел от  "<< iLowerLimitCounter << " до " << iUpperLimitCounter <<"\n" << endl;
	cout << "Ищем в один поток или несколько?\n";
	cout << "1. Один\n";
	cout << "2. Несколько\n";
	cin >> iCountThreads;

	switch (iCountThreads)
	{
	case 1:
		{
			thread calcThread(single_calculate_simple_digit, std::ref(iUpperLimitCounter), std::ref(iResult));
			//calcThread.join();
			calcThread.detach();
			bool bEnd = true;
			do
			{
				if(iQueue != 0)
					Sleep(50);
				else
					bEnd = false;
			}while(bEnd);
		}
		break;
	case 2:
		{
			iResult.push_back(3);
			iAllResult++;
			for(unsigned int i = iLowerLimitCounter; i <= iUpperLimitCounter; i+=2)
			{
				//cout << iQueue << "   ";
				if(check())
				{
					thread calcThread(multiple_calculate_simple_digit, std::ref(i), std::ref(iResult));
					calcThread.detach();
					//calcThread.join();
				}
			}
			bool bEnd = true;
			do
			{
				if(iQueue != 0)
					Sleep(50);
				else
					bEnd = false;
			}while(bEnd);
		}
		break;
	default:
		{
			cout << "Incorrect option\n\n";
			system("pause");
			return 0;
		}
		break;
	}
	
	//for(auto i = iResult.begin(); i != iResult.end(); i++)
 //   {
	//	cout << *i << "\t";
 //   }

	unsigned int iDoubleResult = 0;

	for(auto i = iResult.begin(); i != iResult.end(); i++)
    {
		if((i+1) != iResult.end())
		{
			if(*i == *(i+1))
			{
				iDoubleResult++;
			}
		}
    }
	cout << "\n\nAll double res: " << iDoubleResult << endl;
	cout << "All res: " << iAllResult << "\n\n" << endl;
	system("pause");
    return 0;
}

