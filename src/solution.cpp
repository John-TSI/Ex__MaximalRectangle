#include<algorithm>
#include"../inc/solution.hpp"


int Solution::maximalSubregion(std::vector<std::vector<char>>& matrix/* , int maxArea */)
{
    int rows = matrix.size(), cols = matrix[0].size(), area = rows*cols;
    bool allOnes{true};
    for(auto row : matrix)
    {
        if( !std::all_of(row.begin(), row.end(), [](const char& c){ return c == '1'; }) ){ allOnes = false; break; }
    }
    if(allOnes/*  && area > maxArea */){ return area; } 
    
    // generate the four subregions and recursively call maximalSubregion for each
    std::vector<std::vector<char>> left(rows,std::vector<char>(cols-1));
    std::vector<std::vector<char>> right(rows,std::vector<char>(cols-1));
    if(cols>1)
    {
        for(int row{0}; row<rows; ++row)
        {
            for(int col{0}; col < cols-1; ++col){ left[row][col] = matrix[row][col]; }
            for(int col{1}; col < cols; ++col){ right[row][col-1] = matrix[row][col]; }
        }
    }
    std::vector<std::vector<char>> top(rows-1,std::vector<char>(cols));
    std::vector<std::vector<char>> bottom(rows-1,std::vector<char>(cols));
    if(rows>1)
    {
        for(int col{0}; col<cols; ++col)
        {
            for(int row{0}; row < rows-1; ++row){ top[row][col] = matrix[row][col]; }
            for(int row{1}; row < rows; ++row){ bottom[row-1][col] = matrix[row][col]; }
        }
    }
    if(cols>1 && rows>1)
    {
        return std::max({maximalSubregion(left/* ,maxArea */), maximalSubregion(right/* ,maxArea */), maximalSubregion(top/* ,maxArea */), maximalSubregion(bottom/* ,maxArea */)});
    }
    else if(cols>1)
    {
        return std::max({maximalSubregion(left/* ,maxArea */), maximalSubregion(right/* ,maxArea */)});
    }
    else if(rows>1)
    {
        return std::max({maximalSubregion(top/* ,maxArea */), maximalSubregion(bottom/* ,maxArea */)});
    }
    else{ return (int) matrix[0][0]; }
}

int Solution::maximalRectangle(std::vector<std::vector<char>>& matrix)
{
    //int maxArea{0};
    return maximalSubregion(matrix/* , maxArea */);
}