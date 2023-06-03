#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

struct InputData {
  int target;
  std::vector<int> nums;
};

bool subsetSum(vector<int> &nums, int target) {
  int n = nums.size();
  vector<vector<bool>> dp(n + 1, vector<bool>(target + 1, false));

  for (int i = 0; i <= n; i++) {
    dp[i][0] = true;
  }

  for (int i = 1; i <= n; i++) {
    for (int j = 1; j <= target; j++) {
      if (j < nums[i - 1]) {
        dp[i][j] = dp[i - 1][j];
      } else {
        dp[i][j] = dp[i - 1][j] || dp[i - 1][j - nums[i - 1]];
      }
    }
  }

  return dp[n][target];
}

struct InputData openfile(const char *path) {
  struct InputData data;
  std::ifstream inputFile(path);
  if (inputFile.is_open()) {
    std::string firstLine;
    std::getline(inputFile, firstLine);
    std::stringstream ss(firstLine);

    int n, target, _number;
    vector<int> nums;
    char comma;
    if (!(ss >> data.target >> comma >> n) || comma != ',') {
      std::cout << "Invalid format in the first line." << std::endl;
      exit(1);
    }
    for (int i = 0; i < n; i++) {
      if (!(inputFile >> _number)) {
        std::cout << "Invalid format in line " << i + 2 << "." << std::endl;
        exit(1);
      }
      data.nums.push_back(_number);
    }
    inputFile.close();
  } else {
    std::cout << "Failed to open file." << std::endl;
    exit(1);
  }
  return data;
}

int main(int argc, char *argv[]) {
  const char *defaultpath = "./samples/p06.txt";
  if (argc < 2) {
    std::cout << "Usage: ./program_name <file_path>\n" << std::endl;
    std::cout << "Now is example: main.exe " << defaultpath << std::endl;
  }
  const char *path = (argc > 1) ? argv[1] : defaultpath;
  struct InputData data;
  data = openfile(path);

  bool hasSubsetSum = subsetSum(data.nums, data.target);

  cout << "Subset with sum " << data.target << " ";
  if (hasSubsetSum) {
    cout << "exists." << endl;
  } else {
    cout << "does not exist." << endl;
  }

  return 0;
}
