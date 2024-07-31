#include <iostream>
#include <fstream>
#include <array>

void parseInput(std::string &signal)
{
  std::ifstream input("input.txt");
  
  if(input.is_open())
  {	
  	getline(input, signal);
  }
  input.close();
}

std::string FFT(std::string signal)
{
	std::string result;
	
	std::array<int,4> pattern = {0,1,0,-1};
	
	for(unsigned count=0; count<100; count++)
	{
		std::string newSignal = signal;
		for(unsigned i=0; i<signal.size(); i++)
		{
			unsigned patternDigit = 0;
			unsigned patternCounter = 1;
			
			int newSignalDigit = 0;
			for(unsigned j=0; j<signal.size(); j++)
			{
				if((patternCounter % (i+1)) == 0)
				{
					patternDigit = (patternDigit+1)%4;
					patternCounter = 1;
				}
				else
				{
					patternCounter++;
				}
				newSignalDigit += pattern[patternDigit]*(signal[j]-'0');
			}
			newSignal[i] = (abs(newSignalDigit)%10) + '0';
		}
		signal = newSignal;
	}
	
	result = signal.substr(0,8);
	return result;
}

std::string FFTRealSignal(std::string baseSignal)
{
	std::string result;

	unsigned offset = std::stoi(baseSignal.substr(0,7));
	
	std::string signal;
	signal.reserve(baseSignal.size()*10000);
	for(unsigned i=0; i<10000; i++)
	{
		signal += baseSignal;
	}
	
	signal = signal.substr(offset);
	for(unsigned count=0; count<100; count++)
	{
		std::string newSignal = signal;
		int64_t newNumber = 0;
		for(int64_t i=signal.size()-1; i>=0; i--)
		{
			newNumber += signal[i]-'0';
			newSignal[i] = (newNumber%10) +'0';
		}
		signal = newSignal;
	}
	
	result = signal.substr(0,8);
	return result;
}


int main()
{
	std::string resultA;
	std::string resultB;
	
	std::string signal;
   
  parseInput(signal);
	
 	resultA = FFT(signal);
	resultB = FFTRealSignal(signal);
	
  std::cout << "resultA: " << resultA << '\n';
  std::cout << "resultB: " << resultB << std::endl;
 	
  return 0;
}
