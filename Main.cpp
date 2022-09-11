#include <fstream>
#include <chrono>
using namespace std;


void Insertion_Sort(int arr[], int size) {
	int i = 0;
	int j = 0;
	int key = 0;

	for (i = 1; i < size; i++) {
		key = arr[i];
		j = i - 1;

		while ( (j >= 0) && (arr[j] > key) ) {
			arr[j + 1] = arr[j--];
		}

		arr[j + 1] = key;
	}
}


int main() {
	fstream csv;

	csv.open("sortingSpeeds.csv", ios::out | ios::app);

	if (!csv.is_open()) {
		return 1;
	}

	for (int i = 1; i < 16; i++) {
		csv << i << ", " << i * 2 << endl;
	}

	csv.close();
	return 0;
}