#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <algorithm>
#include <Windows.h>
#include <clocale>

using namespace std;
const int THREAD_MAX = 8;
unsigned int iUpperLimitCounter = 100;
//    unsigned int iLimitCounter = SIZE_MAX;
unsigned int const LOWER_LIMIT_COUNTER = 3;
	struct state_sdigit_type
	{
		unsigned int simple_digit;
		bool not_empty_digit;
	} curren;


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
int add_current_simple_digit_to_vector(state_sdigit_type& state_sd, vector <unsigned int>& iResultIn)
{
	while(!state_sd.not_empty_digit)
	{
		Sleep(10);
	}
	iResultIn.push_back(state_sd.simple_digit);
	state_sd.not_empty_digit = false;

	return 0;
}

int multiple_calculate_simple_digit(unsigned int& new_digit, state_sdigit_type& state_sd, vector <unsigned int>& iResultIn)
{	
    bool x = false;	
	for(unsigned int i = 3; i*2 <= new_digit; i++)
	{
		if(!(new_digit%i))
		{
			x = true;
			lock_guard<mutex> guard(iQueueMutex);
			cout << new_digit << " - it's not a simple digit" << endl;
			break;
		}
    }
	if(!x)
	{
		lock_guard<mutex> guard(iQueueMutex);
		while(state_sd.not_empty_digit)
		{
			Sleep(20);
		}
		state_sd.simple_digit = new_digit;
		state_sd.not_empty_digit = true;
	}

	//lock_guard<mutex> guard(iQueueMutex);
	//if(!x)
	//{
	//	iResultIn.push_back(new_digit);
	//	iAllResult++;
	//	cout << new_digit << " - new simple digit" << endl;
	//}
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

	curren.not_empty_digit = false;
	curren.simple_digit = 0;

	int iCountThreads = 0;

	cout << "Поиск всех простых чисел от  "<< LOWER_LIMIT_COUNTER << " до " << iUpperLimitCounter <<"\n" << endl;
	cout << "Ищем в один поток или несколько?\n";
	cout << "1. Один\n";
	cout << "2. Несколько\n";
	cin >> iCountThreads;

	thread wait_and_adding_sd(add_current_simple_digit_to_vector, std::ref(curren), std::ref(iResult));
	wait_and_adding_sd.detach();

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
			for(unsigned int i = LOWER_LIMIT_COUNTER; i <= iUpperLimitCounter; i+=2)
			{
				//cout << iQueue << "   ";
				if(check())
				{
					thread calcThread(multiple_calculate_simple_digit, std::ref(i), std::ref(curren), std::ref(iResult));
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

