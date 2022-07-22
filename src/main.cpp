// https://leetcode.com/problems/maximal-rectangle/

#include<iostream>
#include"../inc/solution.hpp"


int main()
{
	//std::vector<std::vector<char>> matrix{{'1'}};  // 1
	//std::vector<std::vector<char>> matrix{{'1','1','0'},{'1','1','0'}, {'0','0','1'}};  // 4
	std::vector<std::vector<char>> matrix{{'1','0','1','0','0'},{'1','0','1','1','1'},
										{'1','1','1','1','1'},{'1','0','0','1','0'}};  // 6
	Solution sol;
	std::cout << sol.maximalRectangle2(matrix);
	return 0;
}