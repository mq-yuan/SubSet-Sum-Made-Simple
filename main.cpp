#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <unordered_set>
#include <vector>

// 用于存储子集和与相应基数的结构体
struct SubsetSum {
  int sum;
  int cardinality;

  // 重载 < 运算符
  bool operator<(const SubsetSum &other) const {
    if (sum == other.sum)
      return cardinality < other.cardinality;
    return sum < other.sum;
  }

  // 重载 == 运算符
  bool operator==(const SubsetSum &other) const {
    return (sum == other.sum) && (cardinality == other.cardinality);
  }
};

struct InputData {
  int target;
  std::vector<int> S;
};

struct SubsetSumHasher {
  std::size_t operator()(const SubsetSum &subsetSum) const {
    // 根据 SubsetSum 结构体的成员计算哈希值
    std::size_t hash = std::hash<int>()(subsetSum.sum);
    hash ^= std::hash<int>()(subsetSum.cardinality) + 0x9e3779b9 + (hash << 6) +
            (hash >> 2);
    return hash;
  }
};

std::vector<SubsetSum>
removeDuplicates(const std::vector<SubsetSum> &subsetSums) {
  std::unordered_set<SubsetSum, SubsetSumHasher> uniqueSubsetSums;

  // 遍历 subsetSums，将不重复的元素添加到 uniqueSubsetSums
  for (const SubsetSum &subsetSum : subsetSums) {
    uniqueSubsetSums.insert(subsetSum);
  }

  // 将 uniqueSubsetSums 转换回 std::vector
  std::vector<SubsetSum> result(uniqueSubsetSums.begin(),
                                uniqueSubsetSums.end());

  return result;
}

// 去重函数，将重复的元素去除
void removeDuplicates(std::vector<int> &vec) {
  std::sort(vec.begin(), vec.end()); // 先对向量进行排序
  vec.erase(std::unique(vec.begin(), vec.end()),
            vec.end()); // 再利用std::unique函数去除重复元素
}

// 两个子集和与基数XOR操作
std::vector<SubsetSum> XORSubsetSums(std::vector<SubsetSum> &S1,
                                     std::vector<SubsetSum> &S2, int u) {
  std::vector<SubsetSum> result;

  for (const auto &subset1 : S1) {
    for (const auto &subset2 : S2) {
      int sum = subset1.sum + subset2.sum;
      if (sum <= u) {
        SubsetSum subset;
        subset.sum = sum;
        subset.cardinality = subset1.cardinality + subset2.cardinality;
        result.push_back(subset);
      }
    }
  }

  return result;
}

// 两个子集和XOR操作
std::vector<int> XORSubsetSums(std::vector<int> &S1, std::vector<int> &S2,
                               int u) {
  std::vector<int> result;

  for (const auto &s1 : S1) {
    for (const auto &s2 : S2) {
      int sum = s1 + s2;
      if (sum <= u) {
        result.push_back(sum);
      }
    }
  }
  return result;
}

// 计算子集和并保存基数信息, 基本思想为分治
std::vector<SubsetSum> AllSubsetSumsHash(std::vector<int> &S, int u) {
  int n = S.size();

  if (n == 1) {
    // n=1的边界条件
    SubsetSum subset1;
    subset1.sum = 0;
    subset1.cardinality = 0;
    SubsetSum subset2;
    subset2.sum = S[0];
    subset2.cardinality = 1;
    return {subset1, subset2};
  } else if (n == 0) {
    // n=0的边界条件
    SubsetSum subset1;
    subset1.sum = 0;
    subset1.cardinality = 0;
    return {subset1};
  }

  // 子集划分
  std::vector<int> T(S.begin(), S.begin() + n / 2);

  // 分别计算 T 和 S\T 子集和
  std::vector<SubsetSum> subsetSumsT = AllSubsetSumsHash(T, u);
  std::vector<int> ST(S.begin() + n / 2, S.end());
  std::vector<SubsetSum> subsetSumsST = AllSubsetSumsHash(ST, u);

  // 分治法合并
  std::vector<SubsetSum> result = XORSubsetSums(subsetSumsT, subsetSumsST, u);
  return removeDuplicates(result);
}

// 计算所有子集和的可能情况
std::vector<int> AllSubsetSums(std::vector<int> &S, int u) {
  int n = S.size();
  int b = static_cast<int>(std::sqrt(n * std::log(n)));
  std::vector<int> R;

  for (int l = 0; l <= b - 1; l++) {
    // 选取满足 x ≡ l (mod b) 的元素
    std::vector<int> Sl;
    for (const auto &x : S) {
      if (x % b == l) {
        Sl.push_back(x);
      }
    }

    // 计算 Ql
    std::vector<int> Ql;
    for (const auto &x : Sl) {
      Ql.push_back((x - l) / b);
    }

    // 计算 S^#_{u/b}(Ql)
    std::vector<SubsetSum> SHash = AllSubsetSumsHash(Ql, u / b);

    // 计算 Rl
    std::vector<int> Rl;
    for (const auto &pair : SHash) {
      int z = pair.sum;
      int j = pair.cardinality;
      Rl.push_back(z * b + l * j);
    }
    removeDuplicates(Rl);

    if (l == 0) {
      for (const auto &x : Rl) {
        if (x <= u)
          R.push_back(x);
      }
    } else {
      R = XORSubsetSums(R, Rl, u);
    }
  }
  removeDuplicates(R);
  return R;
}

// 处理读入文件
struct InputData openfile(const char *file_path) {
  struct InputData data;
  std::ifstream inputFile(file_path);

  if (inputFile.is_open()) {
    std::string firstLine;
    std::getline(inputFile, firstLine);
    std::stringstream ss(firstLine);

    int num, number;
    char comma;
    if (!(ss >> data.target >> comma >> num) || comma != ',') {
      std::cout << "Invalid format in the first line." << std::endl;
      exit(1);
    }
    for (int i = 0; i < num; i++) {
      if (!(inputFile >> number)) {
        std::cout << "Invalid format in line " << i + 2 << "." << std::endl;
        exit(1);
      }
      data.S.push_back(number);
    }
    inputFile.close();
  } else {
    std::cout << "Failed to open file." << std::endl;
    exit(1);
  }
  return data;
}

int main(int argc, char *argv[]) {
  const char *defaultPath = "./samples/p06.txt";
  if (argc < 2) {
    std::cout << "Usage: ./program_name <file_path>\n" << std::endl;
    std::cout << "Now is example: main.exe " << defaultPath << std::endl;
  }
  const char *path = (argc > 1) ? argv[1] : defaultPath;

  struct InputData data;
  data = openfile(path);

  // 输入数据验证
  std::cout << "Target: " << data.target << std::endl;
  std::cout << "Set: { ";
  for (int x : data.S) {
    std::cout << x << " ";
  }
  std::cout << "}" << std::endl;

  // 主函数计算子集和数
  std::vector<int> result = AllSubsetSums(data.S, data.target);

  // 输出所有可能的子集和数
  std::cout << "All possible Subset Sum of not greater than Target is "
            << std::endl;
  for (int sum : result) {
    std::cout << sum << " ";
  }
  std::cout << std::endl
            << "Length of Subset Sums: " << result.size() << std::endl;

  if (find(result.begin(), result.end(), data.target) != result.end())
    std::cout << "Exist Subset Sum is " << data.target << std::endl;
  else
    std::cout << "Not Exist Subset Sum is " << data.target
              << "The Most nearly Subset Sum is " << result[result.size() - 1]
              << std::endl;
  return 0;
}
