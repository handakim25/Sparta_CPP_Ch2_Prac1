#include <iostream>
#include <algorithm>
#include <sstream>
#include <functional>

using namespace std;

const int kInputCount = 5; // �Է� �޴� ������ ����

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

// command�� ����ϰ� ����ڷκ��� �Է��� �޾� command�� �ε����� ��ȯ�Ѵ�.
// commands : Ŀ�ǵ� ����Ʈ
// commandCount : Ŀ�ǵ� ����Ʈ�� ������
// return : ����ڷκ��� �Է� ���� command�� �ε���
int GetCommandInput(const string* commands, int commandCount)
{
    if (commandCount <= 0) {
        return -1;
    }

    ostringstream oss;

    oss << "Ŀ�ǵ� : ";
    for (int i = 0; i < commandCount; i++) {
        oss << commands[i] << ":" << i;
        if (i < commandCount - 1) {
            oss << ", ";
        }
    }
    oss << endl;
    oss << "Ŀ�ǵ带 ���ڷ� �Է����ּ���.(0-" << commandCount - 1 << "): ";

    return GetIntInput(oss.str(), "��ȿ�� �Է��� ���ּ���.", 0, commandCount - 1);
}

// ����ڷκ��� ���� �Է��� �޴´�.
// promptMessage : ����ڿ��� �Է��� �ޱ� ���� ����� �ؽ�Ʈ
// errorMessage : �߸��� �Է��� �޾��� ��� ����� �ؽ�Ʈ. empty�� ��� ������� �ʴ´�.
// return : ����ڷκ��� �Է� ���� ����
int GetIntInput(const string& promptMessage, const string& errorMessage)
{
    return GetIntInput(promptMessage, errorMessage, numeric_limits<int>::min(), numeric_limits<int>::max());
}

// ����ڷκ��� ���� �Է��� �޴´�.
// promptMessage : ����ڿ��� �Է��� �ޱ� ���� ����� �ؽ�Ʈ
// errorMessage : �߸��� �Է��� �޾��� ��� ����� �ؽ�Ʈ. empty�� ��� ������� �ʴ´�.
// min, max : �Է� ���� ���� ���� : [min, max]
// return : ����ڷκ��� �Է� ���� ����
int GetIntInput(const string& promptMessage, const string& errorMessage, int min, int max)
{
    int userInput = numeric_limits<int>::max();
    bool isFirst = true;

    // do-while�� �� ��� �ݵ�� �� �� ȣ���� �Ǿ continue�� ����� �������� �ʱ⿡ while ���� ������ ����ϵ��� �ߴ�.
    while (true) {
        if (!isFirst && !errorMessage.empty()) {
            cout << errorMessage << endl;
        }
        cout << promptMessage;
        cin >> userInput;

        // �Է� ��ȿ�� �˻�
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

// ����ڷκ��� ������ �Է� �޾Ƽ� �迭�� ����Ѵ�.
// result : ����� �Է��� ����� �迭
// size : �迭�� ����
void GetInputCommand(int* result, int size)
{
    cout << "������ �Է����ּ���.(" << size << "��)(0 �̻��� ������ �Է����ּ���.)" << endl;

    int score = 0;
    for (int i = 0; i < size; i++) {
        ostringstream oss;
        oss << i + 1 << "��°: ";
        result[i] = GetIntInput(oss.str(), "��ȿ�� �Է��� ���ּ���.", 0, numeric_limits<int>::max());
    }

    cout << divider << endl;
}

// �迭�� ���� ����Ѵ�.
// arr : ���� �迭
// size : �迭�� ������
void PrintSumCommand(int* arr, int size)
{
    if (size <= 0) {
        cout << "Error: Size�� �߸��Ǿ����ϴ�. Size: " << size;
        return;
    }

    float sum = CalcSum(arr, size);
    cout << "���� ���� : " << sum << endl;
    cout << divider << endl;
}

// �迭�� ����� ����Ѵ�.
// arr : ����� ���� �迭
// size : �迭�� ������
void PrintAverageCommand(int* arr, int size)
{
    if (size <= 0) {
        cout << "Error: Size�� �߸��Ǿ����ϴ�. Size: " << size;
        return;
    }

    float average = CalcAverage(arr, size);
    cout << "���� ��� : " << average << endl;
    cout << divider << endl;
}

// �迭�� �������� Ȥ�� ������������ �����Ѵ�.
// arr : ������ �迭
// size : ũ��
void PrintSortCommand(int* arr, int size)
{
    cout << "���� ����� �Է����ּ���. �������� : 1, �������� : 2" << endl;
    string promptMessage = "���� ��� : ";
    int sortOrder = GetIntInput(promptMessage, "��ȿ�� �Է��� ���ּ���.", 1, 2);
    function<bool(int, int)> comparer;
    if (sortOrder == 1) {
        comparer = greater<int>();
    }
    else {
        comparer = less<int>();
    }

    SortArr(arr, size, comparer);
    for (int i = 0; i < size; i++) {
        cout << i + 1 << "��° : " << arr[i] << endl;
    }

    cout << divider;
}

// �߸��� �Է��� ó���� �Լ�
// command : Ŀ�ǵ� �ε���
void HandleDefault(int command)
{
    cout << "�߸��� �Է��Դϴ�. Comamnd : " << command << endl;
}

// �迭�� ���� ����Ѵ�.
// arr : ���� �迭
// size : �迭�� ������
// return : ��
float CalcSum(int* arr, int size)
{
    float sum = 0;
    for (int i = 0; i < size; i++) {
        sum += arr[i];
    }

    return sum;
}

// �迭�� ����� ����Ѵ�.
// arr : ����� ���� �迭
// size : �迭�� ������
// return : ���
float CalcAverage(int* arr, int size)
{
    if (size <= 0) {
        cout << "Invalid Average: Size : " << size << endl;
        return 0.0f;
    }

    return CalcSum(arr, size) / size;
}

// �迭�� �����Ѵ�. ���������δ� Quick Sort�� �̿��Ѵ�.
// arr : ������ �迭
// size : �迭�� ������
// comparer : ���� ������ ������ �� ������
void SortArr(int* arr, int size, function<bool(int, int)> comparer)
{
    QuickSort(arr, 0, size - 1, comparer);
}

// ����Ʈ �Ѵ�.
// arr : ������ �迭
// low, high : �迭 �߿��� ������ �ε��� ���� [low, high]
// comparer : ���� ������ ������ �� ������
void QuickSort(int* arr, int low, int high, function<bool(int, int)> comparer)
{
    if (low < high) {
        int pivot_idx = Partition(arr, low, high, comparer);

        QuickSort(arr, low, pivot_idx - 1, comparer);
        QuickSort(arr, pivot_idx + 1, high, comparer);
    }
}

// ������ �ǹ��� �������� �ǹ����� ���� ���� ��������, �ǹ����� ū ���� ���������� ��ġ�Ѵ�.
// [low, high] ������ �����ϰ� �ȴ�.
// arr : ������ �迭
// low, high : ��Ƽ���� �迭�� ����, [low, high]
// comparer : ���� ������ ������ �� ������
// return : �ǹ��� �ε���
int Partition(int* arr, int low, int high, function<bool(int, int)> comparer)
{
    // �ǹ��� ���� ������ ������ ����
    int pivot = arr[high];
    // �ǹ����� ���� �迭�� �߿��� ���� �������� ����Ű�� �ε���
    int i = low - 1;

    // pivot���� ������ �������� �����ϰ� �ƴϸ� �Ѿ��.
    for (int j = low; j < high; j++) {
        // if(arr[j] < pivot) {
        if (comparer(arr[j], pivot)) {
            // ������ ���� index�� ����
            i++;
            swap(arr[i], arr[j]);
        }
    }

    swap(arr[i + 1], arr[high]);
    return i + 1;
}
