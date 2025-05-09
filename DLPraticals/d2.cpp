#include <iostream>
#include <omp.h>
#include <climits>  // For INT_MIN and INT_MAX

using namespace std;

int main() {
    const int size = 1000;
    int data[size];

    // Initialize array with sample values
    for (int i = 0; i < size; ++i) {
        data[i] = i + 1;  // 1 to 1000
    }

    int min_val = INT_MAX;
    int max_val = INT_MIN;
    long long sum = 0;

    // Parallel reduction
    #pragma omp parallel for reduction(min:min_val) reduction(max:max_val) reduction(+:sum)
    for (int i = 0; i < size; ++i) {
        if (data[i] < min_val) min_val = data[i];
        if (data[i] > max_val) max_val = data[i];
        sum += data[i];
    }

    double average = static_cast<double>(sum) / size;

    // Output results
    cout << "Minimum Value: " << min_val << endl;
    cout << "Maximum Value: " << max_val << endl;
    cout << "Sum: " << sum << endl;
    cout << "Average: " << average << endl;

    return 0;
}
