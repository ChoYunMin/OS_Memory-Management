#include <iostream>
#include <vector>
using namespace std;

// block의 갯수와 사이즈를 확인하는 함수 - 블럭에 대한 정보 벡터를 반환해줌
vector<int*> fnFindBlocks(int memorySize, vector<int> memory, int& NumOfBlocks) {
	bool flag = false;
	int numBlock = 0;
	// 메모리에 block hole이 몇 개인지 확인
	for (int i = 0; i < memorySize; i++) {
		if (memory[i] == -1 && flag == false) { // block의 시작이므로 count + 1;
			numBlock++;
			flag = true;
		}
		else if (memory[i] != -1) {
			flag = false;
		}
	}

	NumOfBlocks = numBlock; // block hole의 갯수

	vector<int*> BlockInfo;

	for (int i = 0; i < numBlock; i++) {
		// 0: block 시작 index, 1: block size 를 저장하는 BlockInfo 벡터
		int* arr = new int[2];
		BlockInfo.push_back(arr);
	}
	flag = false;
	int index = 0;
	// block의 정보를 저장. 0에는 해당 block의 시작 index, 1에는 해당 block의 size를 저장
	for (int i = 0; i < memorySize; i++) {
		if (memory[i] == -1 && flag == false) { // block 첫 발견
			BlockInfo[index][0] = i;
			BlockInfo[index][1] = 1;
			flag = true;
		}
		else if (memory[i] == -1 && flag == true) {
			BlockInfo[index][1]++;
		}
		else {
			if (flag == true) { // block 끝났음
				index++; // 다음 블럭
				flag = false;
			}
		}
	}

	return BlockInfo;
}

int main() {
	int iSizeofMemory; // 메모리 사이즈 입력받기
	cout << "Input an integer, the size of real memory in kilobytes: ";
	cin >> iSizeofMemory;
	
	// 메모리 할당 - 할당된 프로세스가 있으면 프로세스 아이디, 없으면 -1
	vector<int> ArrayOfMemory;
	for (int i = 0; i < iSizeofMemory; i++) {
		ArrayOfMemory.push_back(-1);
	}

	int iRequestNumber; // 몇 번의 명령을 수행할 지 입력 받기(예: 1 64 2 64 3 32 는 명령 3번)
	cout << "Input an integer that you will input numbers of memory requests: ";
	cin >> iRequestNumber;

	vector<int> ArrayOfRequest;// 명령 input 저장하는 배열
	cout << "Input pairs of integers describing memory requests: ";
	for (int i = 0; i < iRequestNumber * 2; i++) {
		int temp;
		cin >> temp;
		ArrayOfRequest.push_back(temp);
	}

	cout << endl << "MEMORY SIZE: " << iSizeofMemory << endl << endl;
	for (int count = 0; count < iRequestNumber; count++) { // 명령 수행

		if (ArrayOfRequest[count * 2 + 1] == 0) // memory free 명령이면
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
					if (flag == true) // 할당된 메모리 중 process id 찾기 끝
						break;
				}
			}
			cout << processSize << "K)" << endl;

			cout << "Best Fit: Freed at address ";
			flag = false;
			for (int i = 0; i < iSizeofMemory; i++) {
				if (ArrayOfMemory[i] == ArrayOfRequest[count * 2]) {
					if (flag == false) {
						cout << i << "K" << endl; // 해당 process가 할당된 메모리의 시작 주소 출력
					}
					
					ArrayOfMemory[i] = -1;
					flag = true;
				}
				else {
					if (flag == true) // process가 할당된 메모리 해제 끝!
						break;
				}
			}

			int NumOfBlocks;
			vector<int*> BlockInfo;
			BlockInfo = fnFindBlocks(iSizeofMemory, ArrayOfMemory, NumOfBlocks); // memory에 있는 block들에 대한 정보 불러오기

			cout << "\t";
			int sum = 0;
			for (int i = 0; i < NumOfBlocks; i++) {
				sum += BlockInfo[i][1];
			}
			cout << sum << "K free, " << NumOfBlocks << " block(s), average size = " << sum / NumOfBlocks << "K" << endl;
		}
		else { // memory 할당 명령어이면
			cout << "REQUEST " << ArrayOfRequest[count * 2] << ": " << ArrayOfRequest[count * 2 + 1] << "K" << endl;

			cout << "Best Fit: Allocated at address ";

			int NumOfBlocks;
			vector<int*> BlockInfo;
			int min, minIndex;
			BlockInfo = fnFindBlocks(iSizeofMemory, ArrayOfMemory, NumOfBlocks); // memory에 있는 block들에 대한 정보 불러오기
			for (int i = 0; i < NumOfBlocks; i++) {
				if (BlockInfo[i][1] >= ArrayOfRequest[count * 2 + 1]) { // block의 크기가 할당하려는 메모리보다 큰 경우만 고려
					// block size와 할당하려는 메모리 크기 차이가 가장 작은 block 찾기(Best Fit)
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

			cout << BlockInfo[minIndex][0] << "K" << endl; // 할당하는 block의 시작 좌표 출력

			// minIndex 번째의 block에 process size만큼 process id 할당
			for (int i = 0; i < ArrayOfRequest[count * 2 + 1]; i++) {
				ArrayOfMemory[BlockInfo[minIndex][0] + i] = ArrayOfRequest[count * 2];
			}

			BlockInfo = fnFindBlocks(iSizeofMemory, ArrayOfMemory, NumOfBlocks); // memory에 있는 block들에 대한 정보 불러오기
			
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