/**
 * This file tests insertion sort and quicksort measuring runtime
 *
 * Authors: Matthew Jacobs and Michael White
 * Version: 1.0.0
 * file: Main.cpp
 * Created: September 2022
 * Copyright: Matthew Jacobs and Michael White
 *
 * Description: This file has implementations of Insertion Sort and Quick sort
 * These implementations time their execution.
 * Quick sort has multiple partition function and partition selection options
 * These can be used to compare the efficiency of different methods of quick sort
 * and insertion sort.
 */

#include <fstream>
#include <chrono>
#include <cstdlib>

using namespace std;

// global constants

// const int used instead of #define to work with if statements
// partition selection options
const int LAST = 0;
const int MIDDLE = 1;
const int THRPTMED = 2;

// partiton method options
const int TWOPTR = 0;
const int STRTNLOW = 1;

// array generation options
const int FORWARD = 0;
const int REVERSE = 1;
const int RANDOM = 2;

// returns system time in milliseconds (time since jan 1 1970)
chrono::steady_clock::time_point GetCurrTime()
{
	return chrono::steady_clock::now();
}

// generates an appropriately sized array sorted as desired
int* ArrGen(int size, int sortOption)
{
	int* arr = new int[size];
	switch (sortOption)
	{
		// loops inside cases is more efficient
		case FORWARD:
			for (int i = 0; i < size; i++)
			{
				*(arr + i) = i;
			}
			break;
		case REVERSE:
			for (int i = 0; i < size; i++)
			{
				*(arr + i) = size - i;
			}
			break;
		case RANDOM:
			for (int i = 0; i < size; i++)
			{
				*(arr + i) = rand() % size;
			}
			break;
	}
	return arr;
}

// runs insertion sort on an array and returns the time taken
double InsertionSort(int* arr, int size) 
{
	chrono::steady_clock::time_point start = GetCurrTime();
	int i = 0;
	int j = 0;
	int key = 0;

	for (i = 1; i < size; i++) 
	{
		key = *(arr + i);
		j = i - 1;

		while ((j >= 0) && (*(arr + j) > key)) 
		{
			*(arr + j + 1) = *(arr + j);
			j--;
		}

		*(arr + j + 1) = key;
	}
	chrono::steady_clock::time_point end = GetCurrTime();
	return chrono::duration_cast<std::chrono::duration<double>>(end - start).count();
}

// makes the two sub arrays of the previous array, sorted in place for the selected partition method
int* QuickSortPartition(int* first, int* last, int partitionOption)
{
	if (partitionOption == TWOPTR)
	{
		// establish potential pointers
		int* pivot = last;
		int* lower = first;
		int* upper = last - 1;
		// move pointers toward each other and swap as necessary
		while (lower <= upper)
		{
			while (lower <= upper && *upper >= *pivot)
			{
				upper--;
			}
			while (lower <= upper && *lower <= *pivot)
			{
				lower++;
			}
			if (lower < upper)
			{
				swap(*lower, *upper);
			}
		}
		swap(*lower, *last); // put the pivot back in the middle of the array
		return lower;
	}	
	else
	{
		// low pointer straight through iteration from Algorithms

		// establish potential pointers (pivot is the last)
		int* curr = first;
		int* low = first - 1;
		// iterate through array
		while (curr != last)
		{
			if (*curr <= *last)
			{
				// move low pointer so it points to the last value of the lower division
				low++;
				swap(*low, *curr);
			}
			curr++;
		}
		// prepare and return split point
		low++;
		swap(*low, *last);
		return low;
	}
}

// runs and times quick sort on an array with options for partition 
void QuickSort(int* arr, int size, int partitionOption, int partitionSelectionOption)
{	
	int partitionIndex = size - 1;
	// determine partition value
	if (partitionSelectionOption == MIDDLE)
	{
		partitionIndex = size / 2;
	}
	else if (partitionSelectionOption == THRPTMED)
	{
		int first = *arr;
		int mid = *(arr + (size / 2));
		int last = *(arr + size - 1);
		// find middle of 3 values
		int firstRank = 0;
		int midRank = 0;
		int lastRank = 0;
		if (first > mid)
		{
			firstRank++;
		}
		else
		{
			midRank++;
		}
		if (first > last)
		{
			firstRank++;
		}
		else
		{
			lastRank++;
		}
		if (mid > last)
		{
			midRank++;
		}
		else
		{
			lastRank++;
		}
		if (firstRank == 1)
		{
			partitionIndex = 0;
		}
		else if (midRank == 1)
		{
			partitionIndex = size / 2;
		}
	}
	// already set to last by default

	// run sort recursively
	swap(*(arr + partitionIndex), *(arr + size - 1)); // place partition at end of array
	int* pivot = QuickSortPartition(arr, arr + size - 1, partitionOption);
	// recursive call
	int lowSize = pivot - arr; // find the size of the front array
	int highSize = size - lowSize - 1; // subtract 1 to not re-sort pivot
	if (lowSize > 1)
	{
		QuickSort(arr, lowSize, partitionOption, partitionSelectionOption);
	}
	if (highSize > 1)
	{
		QuickSort(pivot + 1, highSize, partitionOption, partitionSelectionOption);
	}
}

// times outermost quick sort call without timing inside calls
double TimeQuickSort(int* arr, int size, int partitionOption, int partitionSelectionOption)
{
	// program start time
	chrono::steady_clock::time_point start = GetCurrTime();

	// run quick sort
	QuickSort(arr, size, partitionOption, partitionSelectionOption);

	// program end time
	chrono::steady_clock::time_point end = GetCurrTime();
	return chrono::duration_cast<std::chrono::duration<double>>(end - start).count();
}

int main() 
{
	// set up randomizer seed
	srand(chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count());
	
	// set up output file
	fstream csv;

	csv.open("sortingSpeedsResults.csv", ios::out | ios::app);

	if (!csv.is_open()) 
	{
		return 1;
	}

	// top rows to organize data
	csv << "Array Size" << ", "
		<< "Insertion Sort" << ", " << "-" << ", " << "-" << ", "
		<< "Quick Sort" << ", " << "-" << ", " << "-" << ", "
		<< "Partition Method" << ", " << "-" << ", "
		<< "Partition Selection" << ", " << "-" << ", " << "-" 
		<< endl;
	csv << "-" << ", "
		<< "Forward" << ", " << "Reverse" << ", " << "Random" << ", " 
		<< "Forward" << ", " << "Reverse" << ", " << "Random" << ", "
		<< "2-Pointer" << ", " << "Start and Low" << ", " 
		<< "Last" << ", " << "Middle" << ", " << "3-Point median" 
		<< endl;

	// run tests

	// number of different method combinations being tested
	const int NUM_TESTS = 9;

	// incremented by 10
	for (int i = 1; i <= 100; i++)
	{
		// data creation info
		int sampNum = i * 10;
		int* testArrays[NUM_TESTS];

		// create data to be sorted
		for (int j = 0; j < NUM_TESTS; j++)
		{
			if (j >= 5 || j == 2)
			{
				*(testArrays + j) = ArrGen(sampNum, RANDOM);
			}
			else if (j == 0 || j == 3)
			{
				*(testArrays + j) = ArrGen(sampNum, FORWARD);
			}
			else
			{
				*(testArrays + j) = ArrGen(sampNum, REVERSE);
			}
		}

		// get time results
		double runTimes[NUM_TESTS];
		*runTimes = InsertionSort(*testArrays, sampNum);
		*(runTimes + 1) = InsertionSort(*(testArrays + 1), sampNum);
		*(runTimes + 2) = InsertionSort(*(testArrays + 2), sampNum);
		*(runTimes + 3) = TimeQuickSort(*(testArrays + 3), sampNum, TWOPTR, LAST);
		*(runTimes + 4) = TimeQuickSort(*(testArrays + 4), sampNum, TWOPTR, LAST);
		*(runTimes + 5) = TimeQuickSort(*(testArrays + 5), sampNum, TWOPTR, LAST);
		*(runTimes + 6) = TimeQuickSort(*(testArrays + 6), sampNum, STRTNLOW, LAST);
		*(runTimes + 7) = TimeQuickSort(*(testArrays + 7), sampNum, TWOPTR, MIDDLE);
		*(runTimes + 8) = TimeQuickSort(*(testArrays + 8), sampNum, TWOPTR, THRPTMED);

		// clean up to prevent memory leaks
		for (int j = 0; j < NUM_TESTS; j++)
		{
			delete *(testArrays + j);
		}

		// output to file
		csv << sampNum << ", " << *runTimes << ", " << *(runTimes + 1) << ", " << *(runTimes + 2) 
			<< ", " << *(runTimes + 3) << ", " << *(runTimes + 4) << ", "  << *(runTimes + 5)
			<< ", "  << *(runTimes + 5)  << ", "  << *(runTimes + 6) << ", "  << *(runTimes + 5)
			<< ", " << *(runTimes + 7) << ", " << *(runTimes + 8) << endl;
	}

	// incremented by 1000
	for (int i = 2; i <= 10; i++)
	{
		/// data creation info
		int sampNum = i * 1000;
		int* testArrays[NUM_TESTS];

		// create data to be sorted
		for (int j = 0; j < NUM_TESTS; j++)
		{
			if (j >= 5 || j == 2)
			{
				*(testArrays + j) = ArrGen(sampNum, RANDOM);
			}
			else if (j == 0 || j == 3)
			{
				*(testArrays + j) = ArrGen(sampNum, FORWARD);
			}
			else
			{
				*(testArrays + j) = ArrGen(sampNum, REVERSE);
			}
		}

		// get time results
		double runTimes[NUM_TESTS];
		*runTimes = InsertionSort(*testArrays, sampNum);
		*(runTimes + 1) = InsertionSort(*(testArrays + 1), sampNum);
		*(runTimes + 2) = InsertionSort(*(testArrays + 2), sampNum);
		*(runTimes + 3) = TimeQuickSort(*(testArrays + 3), sampNum, TWOPTR, LAST);
		*(runTimes + 4) = TimeQuickSort(*(testArrays + 4), sampNum, TWOPTR, LAST);
		*(runTimes + 5) = TimeQuickSort(*(testArrays + 5), sampNum, TWOPTR, LAST);
		*(runTimes + 6) = TimeQuickSort(*(testArrays + 6), sampNum, STRTNLOW, LAST);
		*(runTimes + 7) = TimeQuickSort(*(testArrays + 7), sampNum, TWOPTR, MIDDLE);
		*(runTimes + 8) = TimeQuickSort(*(testArrays + 8), sampNum, TWOPTR, THRPTMED);

				// clean up to prevent memory leaks
		for (int j = 0; j < NUM_TESTS; j++)
		{
			delete* (testArrays + j);
		}

		// output to file
		csv << sampNum << ", " << *runTimes << ", " << *(runTimes + 1) << ", " << *(runTimes + 2) 
			<< ", " << *(runTimes + 3) << ", " << *(runTimes + 4) << ", " << *(runTimes + 5)
			<< ", "  << *(runTimes + 5)  << ", "  << *(runTimes + 6) << ", "  << *(runTimes + 5)
			<< ", " << *(runTimes + 7) << ", " << *(runTimes + 8) << endl;
	}

	// increment by 10000 (no insertion sort)
	for (int i = 2; i <= 100; i++)
	{
		// data creation info
		int sampNum = i * 10000;
		int* testArrays[NUM_TESTS];

		// create data to be sorted
		for (int j = 3; j < NUM_TESTS; j++)
		{
			if (j >= 5 || j == 2)
			{
				*(testArrays + j) = ArrGen(sampNum, RANDOM);
			}
			else if (j == 0 || j == 3)
			{
				*(testArrays + j) = ArrGen(sampNum, FORWARD);
			}
			else
			{
				*(testArrays + j) = ArrGen(sampNum, REVERSE);
			}
		}

		// get time results
		double runTimes[NUM_TESTS];
		*(runTimes + 3) = TimeQuickSort(*(testArrays + 3), sampNum, TWOPTR, LAST);
		*(runTimes + 4) = TimeQuickSort(*(testArrays + 4), sampNum, TWOPTR, LAST);
		*(runTimes + 5) = TimeQuickSort(*(testArrays + 5), sampNum, TWOPTR, LAST);
		*(runTimes + 6) = TimeQuickSort(*(testArrays + 6), sampNum, STRTNLOW, LAST);
		*(runTimes + 7) = TimeQuickSort(*(testArrays + 7), sampNum, TWOPTR, MIDDLE);
		*(runTimes + 8) = TimeQuickSort(*(testArrays + 8), sampNum, TWOPTR, THRPTMED);

		// clean up to prevent memory leaks
		for (int j = 3; j < NUM_TESTS; j++)
		{
			delete* (testArrays + j);
		}

		// output to file
		csv << sampNum << ", " << "-" << ", " << "-" << ", " << "-"
			<< ", " << *(runTimes + 3) << ", " << *(runTimes + 4) << ", "  << *(runTimes + 5) 
			<< ", "  << *(runTimes + 5)  << ", "  << *(runTimes + 6) << ", " << *(runTimes + 5)
			<< ", " << *(runTimes + 7) << ", " << *(runTimes + 8) << endl;
	}

	csv.close();
	return 0;
}
