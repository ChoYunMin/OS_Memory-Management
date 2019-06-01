#include <iostream>
#include <vector>
using namespace std;

// block�� ������ ����� Ȯ���ϴ� �Լ� - ���� ���� ���� ���͸� ��ȯ����
vector<int*> fnFindBlocks(int memorySize, vector<int> memory, int& NumOfBlocks) {
	bool flag = false;
	int numBlock = 0;
	// �޸𸮿� block hole�� �� ������ Ȯ��
	for (int i = 0; i < memorySize; i++) {
		if (memory[i] == -1 && flag == false) { // block�� �����̹Ƿ� count + 1;
			numBlock++;
			flag = true;
		}
		else if (memory[i] != -1) {
			flag = false;
		}
	}

	NumOfBlocks = numBlock; // block hole�� ����

	vector<int*> BlockInfo;

	for (int i = 0; i < numBlock; i++) {
		// 0: block ���� index, 1: block size �� �����ϴ� BlockInfo ����
		int* arr = new int[2];
		BlockInfo.push_back(arr);
	}
	flag = false;
	int index = 0;
	// block�� ������ ����. 0���� �ش� block�� ���� index, 1���� �ش� block�� size�� ����
	for (int i = 0; i < memorySize; i++) {
		if (memory[i] == -1 && flag == false) { // block ù �߰�
			BlockInfo[index][0] = i;
			BlockInfo[index][1] = 1;
			flag = true;
		}
		else if (memory[i] == -1 && flag == true) {
			BlockInfo[index][1]++;
		}
		else {
			if (flag == true) { // block ������
				index++; // ���� ��
				flag = false;
			}
		}
	}

	return BlockInfo;
}

int main() {
	int iSizeofMemory; // �޸� ������ �Է¹ޱ�
	cout << "Input an integer, the size of real memory in kilobytes: ";
	cin >> iSizeofMemory;
	
	// �޸� �Ҵ� - �Ҵ�� ���μ����� ������ ���μ��� ���̵�, ������ -1
	vector<int> ArrayOfMemory;
	for (int i = 0; i < iSizeofMemory; i++) {
		ArrayOfMemory.push_back(-1);
	}

	int iRequestNumber; // �� ���� ����� ������ �� �Է� �ޱ�(��: 1 64 2 64 3 32 �� ��� 3��)
	cout << "Input an integer that you will input numbers of memory requests: ";
	cin >> iRequestNumber;

	vector<int> ArrayOfRequest;// ��� input �����ϴ� �迭
	cout << "Input pairs of integers describing memory requests: ";
	for (int i = 0; i < iRequestNumber * 2; i++) {
		int temp;
		cin >> temp;
		ArrayOfRequest.push_back(temp);
	}

	cout << endl << "MEMORY SIZE: " << iSizeofMemory << endl << endl;
	for (int count = 0; count < iRequestNumber; count++) { // ��� ����

		if (ArrayOfRequest[count * 2 + 1] == 0) // memory free ����̸�
		{
			cout << "FREE REQUEST " << ArrayOfRequest[count * 2] << " (";
			int processSize = 0;
			bool flag = false;
			for (int i = 0; i < iSizeofMemory; i++) {
				if (ArrayOfMemory[i] == ArrayOfRequest[count * 2]) {
					processSize++;
					flag = true;
				}
				else {
					if (flag == true) // �Ҵ�� �޸� �� process id ã�� ��
						break;
				}
			}
			cout << processSize << "K)" << endl;

			cout << "Best Fit: Freed at address ";
			flag = false;
			for (int i = 0; i < iSizeofMemory; i++) {
				if (ArrayOfMemory[i] == ArrayOfRequest[count * 2]) {
					if (flag == false) {
						cout << i << "K" << endl; // �ش� process�� �Ҵ�� �޸��� ���� �ּ� ���
					}
					
					ArrayOfMemory[i] = -1;
					flag = true;
				}
				else {
					if (flag == true) // process�� �Ҵ�� �޸� ���� ��!
						break;
				}
			}

			int NumOfBlocks;
			vector<int*> BlockInfo;
			BlockInfo = fnFindBlocks(iSizeofMemory, ArrayOfMemory, NumOfBlocks); // memory�� �ִ� block�鿡 ���� ���� �ҷ�����

			cout << "\t";
			int sum = 0;
			for (int i = 0; i < NumOfBlocks; i++) {
				sum += BlockInfo[i][1];
			}
			cout << sum << "K free, " << NumOfBlocks << " block(s), average size = " << sum / NumOfBlocks << "K" << endl;
		}
		else { // memory �Ҵ� ��ɾ��̸�
			cout << "REQUEST " << ArrayOfRequest[count * 2] << ": " << ArrayOfRequest[count * 2 + 1] << "K" << endl;

			cout << "Best Fit: Allocated at address ";

			int NumOfBlocks;
			vector<int*> BlockInfo;
			int min, minIndex;
			BlockInfo = fnFindBlocks(iSizeofMemory, ArrayOfMemory, NumOfBlocks); // memory�� �ִ� block�鿡 ���� ���� �ҷ�����
			for (int i = 0; i < NumOfBlocks; i++) {
				if (BlockInfo[i][1] >= ArrayOfRequest[count * 2 + 1]) { // block�� ũ�Ⱑ �Ҵ��Ϸ��� �޸𸮺��� ū ��츸 ���
					// block size�� �Ҵ��Ϸ��� �޸� ũ�� ���̰� ���� ���� block ã��(Best Fit)
					if (i == 0) {
						min = BlockInfo[i][1] - ArrayOfRequest[count * 2 + 1];
						minIndex = 0;
					}
					else {
						if (min > BlockInfo[i][1] - ArrayOfRequest[count * 2 + 1]) {
							min = BlockInfo[i][1] - ArrayOfRequest[count * 2 + 1];
							minIndex = i;
						}
					}
				}
			}

			cout << BlockInfo[minIndex][0] << "K" << endl; // �Ҵ��ϴ� block�� ���� ��ǥ ���

			// minIndex ��°�� block�� process size��ŭ process id �Ҵ�
			for (int i = 0; i < ArrayOfRequest[count * 2 + 1]; i++) {
				ArrayOfMemory[BlockInfo[minIndex][0] + i] = ArrayOfRequest[count * 2];
			}

			BlockInfo = fnFindBlocks(iSizeofMemory, ArrayOfMemory, NumOfBlocks); // memory�� �ִ� block�鿡 ���� ���� �ҷ�����
			
			cout << "\t";
			int sum = 0;
			for (int i = 0; i < NumOfBlocks; i++) {
				sum += BlockInfo[i][1];
			}
			cout << sum << "K free, " << NumOfBlocks << " block(s), average size = " << sum / NumOfBlocks << "K" << endl;
		}


		cout << endl;
	}
	

	system("pause");
	return 0;
}