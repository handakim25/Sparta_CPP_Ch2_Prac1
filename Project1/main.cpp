#include <iostream>
#include <algorithm>
#include <sstream>
#include <functional>

using namespace std;

const int kInputCount = 5; // 입력 받는 점수의 개수

const string commandList[] = { "Input", "Sum", "Average", "Sort", "Quit" };
const string divider = "------";

// Command

int GetCommandInput(const string* commands, int commandCount);
int GetIntInput(const string& promptMessage, const string& errorMessage);
int GetIntInput(const string& promptMessage, const string& errorMessage, int min, int max);

// Handle Command

void GetInputCommand(int* result, int size);
void PrintSumCommand(int* arr, int size);
void PrintAverageCommand(int* arr, int size);
void PrintSortCommand(int* arr, int size);
void HandleDefault(int command);

// Calculation

float CalcSum(int* arr, int size);
float CalcAverage(int* arr, int size);
void SortArr(int* arr, int size, function<bool(int, int)> comparer);
void QuickSort(int* arr, int low, int high, function<bool(int, int)> comparer);
int Partition(int* arr, int low, int high, function<bool(int, int)> comparer);

int main()
{
    int scores[kInputCount] = { 0, };

    int commandCount = sizeof(commandList) / sizeof(commandList[0]);
    int exitCommandIndex = 4;
    int command = -1;

    while ((command = GetCommandInput(commandList, commandCount)) != exitCommandIndex
        && command != -1)
    {
        switch (command)
        {
        case 0:
            GetInputCommand(scores, kInputCount);
            break;
        case 1:
            PrintSumCommand(scores, kInputCount);
            break;
        case 2:
            PrintAverageCommand(scores, kInputCount);
            break;
        case 3:
            PrintSortCommand(scores, kInputCount);
            break;
        default:
            HandleDefault(command);
            break;
        }
    }

    return 0;
}

// command를 출력하고 사용자로부터 입력을 받아 command의 인덱스를 반환한다.
// commands : 커맨드 리스트
// commandCount : 커맨드 리스트의 사이즈
// return : 사용자로부터 입력 받은 command의 인덱스
int GetCommandInput(const string* commands, int commandCount)
{
    if (commandCount <= 0) {
        return -1;
    }

    ostringstream oss;

    oss << "커맨드 : ";
    for (int i = 0; i < commandCount; i++) {
        oss << commands[i] << ":" << i;
        if (i < commandCount - 1) {
            oss << ", ";
        }
    }
    oss << endl;
    oss << "커맨드를 숫자로 입력해주세요.(0-" << commandCount - 1 << "): ";

    return GetIntInput(oss.str(), "유효한 입력을 해주세요.", 0, commandCount - 1);
}

// 사용자로부터 정수 입력을 받는다.
// promptMessage : 사용자에게 입력을 받기 전에 출력할 텍스트
// errorMessage : 잘못된 입력을 받았을 경우 출력할 텍스트. empty일 경우 출력하지 않는다.
// return : 사용자로부터 입력 받은 정수
int GetIntInput(const string& promptMessage, const string& errorMessage)
{
    return GetIntInput(promptMessage, errorMessage, numeric_limits<int>::min(), numeric_limits<int>::max());
}

// 사용자로부터 정수 입력을 받는다.
// promptMessage : 사용자에게 입력을 받기 전에 출력할 텍스트
// errorMessage : 잘못된 입력을 받았을 경우 출력할 텍스트. empty일 경우 출력하지 않는다.
// min, max : 입력 받을 정수 범위 : [min, max]
// return : 사용자로부터 입력 받은 정수
int GetIntInput(const string& promptMessage, const string& errorMessage, int min, int max)
{
    int userInput = numeric_limits<int>::max();
    bool isFirst = true;

    // do-while을 할 경우 반드시 한 번 호출이 되어서 continue가 제대로 동작하지 않기에 while 무한 루프를 사용하도록 했다.
    while (true) {
        if (!isFirst && !errorMessage.empty()) {
            cout << errorMessage << endl;
        }
        cout << promptMessage;
        cin >> userInput;

        // 입력 유효성 검사
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        else if (userInput >= min && userInput <= max) {
            break;
        }

        isFirst = false;
    }

    return userInput;
}

// 사용자로부터 점수를 입력 받아서 배열에 출력한다.
// result : 사용자 입력을 출력할 배열
// size : 배열의 개수
void GetInputCommand(int* result, int size)
{
    cout << "점수를 입력해주세요.(" << size << "개)(0 이상의 정수를 입력해주세요.)" << endl;

    int score = 0;
    for (int i = 0; i < size; i++) {
        ostringstream oss;
        oss << i + 1 << "번째: ";
        result[i] = GetIntInput(oss.str(), "유효한 입력을 해주세요.", 0, numeric_limits<int>::max());
    }

    cout << divider << endl;
}

// 배열의 합을 출력한다.
// arr : 합할 배열
// size : 배열의 사이즈
void PrintSumCommand(int* arr, int size)
{
    if (size <= 0) {
        cout << "Error: Size가 잘못되었습니다. Size: " << size;
        return;
    }

    float sum = CalcSum(arr, size);
    cout << "점수 총합 : " << sum << endl;
    cout << divider << endl;
}

// 배열의 평균을 출력한다.
// arr : 평균을 구할 배열
// size : 배열의 사이즈
void PrintAverageCommand(int* arr, int size)
{
    if (size <= 0) {
        cout << "Error: Size가 잘못되었습니다. Size: " << size;
        return;
    }

    float average = CalcAverage(arr, size);
    cout << "점수 평균 : " << average << endl;
    cout << divider << endl;
}

// 배열을 오름차순 혹은 내림차순으로 정렬한다.
// arr : 정렬할 배열
// size : 크기
void PrintSortCommand(int* arr, int size)
{
    cout << "정렬 방법을 입력해주세요. 내림차순 : 1, 오름차순 : 2" << endl;
    string promptMessage = "정렬 방법 : ";
    int sortOrder = GetIntInput(promptMessage, "유효한 입력을 해주세요.", 1, 2);
    function<bool(int, int)> comparer;
    if (sortOrder == 1) {
        comparer = greater<int>();
    }
    else {
        comparer = less<int>();
    }

    SortArr(arr, size, comparer);
    for (int i = 0; i < size; i++) {
        cout << i + 1 << "번째 : " << arr[i] << endl;
    }

    cout << divider;
}

// 잘못된 입력을 처리할 함수
// command : 커맨드 인덱스
void HandleDefault(int command)
{
    cout << "잘못된 입력입니다. Comamnd : " << command << endl;
}

// 배열의 합을 계산한다.
// arr : 합할 배열
// size : 배열의 사이즈
// return : 합
float CalcSum(int* arr, int size)
{
    float sum = 0;
    for (int i = 0; i < size; i++) {
        sum += arr[i];
    }

    return sum;
}

// 배열의 평균을 계산한다.
// arr : 평균을 구할 배열
// size : 배열의 사이즈
// return : 평균
float CalcAverage(int* arr, int size)
{
    if (size <= 0) {
        cout << "Invalid Average: Size : " << size << endl;
        return 0.0f;
    }

    return CalcSum(arr, size) / size;
}

// 배열을 정렬한다. 내부적으로는 Quick Sort를 이용한다.
// arr : 정렬할 배열
// size : 배열의 사이즈
// comparer : 정렬 방향을 결정할 비교 연산자
void SortArr(int* arr, int size, function<bool(int, int)> comparer)
{
    QuickSort(arr, 0, size - 1, comparer);
}

// 퀵소트 한다.
// arr : 정렬할 배열
// low, high : 배열 중에서 정렬할 인덱스 범위 [low, high]
// comparer : 정렬 방향을 결정할 비교 연산자
void QuickSort(int* arr, int low, int high, function<bool(int, int)> comparer)
{
    if (low < high) {
        int pivot_idx = Partition(arr, low, high, comparer);

        QuickSort(arr, low, pivot_idx - 1, comparer);
        QuickSort(arr, pivot_idx + 1, high, comparer);
    }
}

// 임의의 피벗을 기준으로 피벗보다 작은 값은 왼쪽으로, 피벗보다 큰 값은 오른쪽으로 배치한다.
// [low, high] 구간을 정렬하게 된다.
// arr : 정렬할 배열
// low, high : 파티션할 배열의 범위, [low, high]
// comparer : 정렬 방향을 결정할 비교 연산자
// return : 피벗의 인덱스
int Partition(int* arr, int low, int high, function<bool(int, int)> comparer)
{
    // 피벗을 가장 오른쪽 값으로 설정
    int pivot = arr[high];
    // 피벗보다 작은 배열들 중에서 가장 마지막을 가리키는 인덱스
    int i = low - 1;

    // pivot보다 작으면 왼쪽으로 복사하고 아니면 넘어간다.
    for (int j = low; j < high; j++) {
        // if(arr[j] < pivot) {
        if (comparer(arr[j], pivot)) {
            // 복사할 곳의 index를 갱신
            i++;
            swap(arr[i], arr[j]);
        }
    }

    swap(arr[i + 1], arr[high]);
    return i + 1;
}
