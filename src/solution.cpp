#include<algorithm>
#include"../inc/solution.hpp"


int Solution::maximalRectangle(std::vector<std::vector<char>>& matrix)
{
    int rows = matrix.size(), cols = matrix[0].size(), area = rows*cols;
    bool allOnes{true}, allZeros{false};
    for(auto row : matrix)    // check whether matrix is all ones / all zeros
    {
        if( !std::all_of(row.begin(), row.end(), [](const char& c){ return c == '1'; }) ){ allOnes = false; }
        if( std::all_of(row.begin(), row.end(), [](const char& c){ return c == '0'; }) ){ allZeros = true; }
    }
    if(allOnes || allZeros){ return (allOnes) ? area : 0; }
    
    // generate four maximal overlapping subregions of the matrix, recursively call maximalRectangle for each
    std::vector<std::vector<char>> left(rows,std::vector<char>(cols-1)), right(rows,std::vector<char>(cols-1));
    if(cols>1)  // "left" subregion drops last column of matrix, "right" drops first column; both require at least two columns
    {
        for(int row{0}; row<rows; ++row)
        {
            for(int col{0}; col < cols-1; ++col){ left[row][col] = matrix[row][col]; }
            for(int col{1}; col < cols; ++col){ right[row][col-1] = matrix[row][col]; }
        }
    }
    std::vector<std::vector<char>> top(rows-1,std::vector<char>(cols)), bottom(rows-1,std::vector<char>(cols));
    if(rows>1)  // "top" subregion drops last row of matrix, "bottom" drops first row; both require at least two rows
    {
        for(int col{0}; col<cols; ++col)
        {
            for(int row{0}; row < rows-1; ++row){ top[row][col] = matrix[row][col]; }
            for(int row{1}; row < rows; ++row){ bottom[row-1][col] = matrix[row][col]; }
        }
    }
    // recursive call with appropriate matrices
    if(cols>1 && rows>1)
    {
        return std::max({maximalRectangle(left), maximalRectangle(right), maximalRectangle(top), maximalRectangle(bottom)});
    }
    else if(cols>1 || rows>1)
    {
        return (cols>1)
            ? std::max({maximalRectangle(left), maximalRectangle(right)})
            : std::max({maximalRectangle(top),  maximalRectangle(bottom)});
    }
    else{ return (int) matrix[0][0]; }
}