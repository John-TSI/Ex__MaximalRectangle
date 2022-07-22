#include<algorithm>
#include<stack>
#include"../inc/solution.hpp"


// SOLUTION ONE: RECURSION
int Solution::maximalRectangle(std::vector<std::vector<char>>& matrix)
{
    int rows = matrix.size(), cols = matrix[0].size(), area = rows*cols;
    if(area == 1){ return (int) (matrix[0][0] == '1'); }

    // check whether matrix is all ones / all zeros
    bool allOnes{true}, allZeros{false};
    for(auto row : matrix)
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
    return (cols>1) ? std::max({maximalRectangle(left), maximalRectangle(right)})
                    : std::max({maximalRectangle(top),  maximalRectangle(bottom)});
}


// SOLUTION TWO: ACCUMULATION AND STACK
int Solution::maximalRectangle2(std::vector<std::vector<char>>& matrix)
{
    int rows = matrix.size(), cols = matrix[0].size(), maxArea{0};
    if(rows*cols == 1){ return (int) (matrix[0][0] == '1'); }
    std::vector<int> rowData(cols,0);

    for(int row{0}; row<rows; ++row)
    {
        std::stack<int> idxStack{{-1}};
        for(int col{0}; col<cols; ++col)
        { 
            rowData[col] += (matrix[row][col] - '0');
            
            if(idxStack.top() == -1 || rowData[col] >= rowData[idxStack.top()]){ idxStack.push(col); }
            else
            {
                while(idxStack.top() != -1 && rowData[idxStack.top()] >= rowData[col])
                {
                    int currIdx = idxStack.top(); idxStack.pop(); int leftIdx = idxStack.top(); 
                    int area = rowData[currIdx]*(col - leftIdx - 1);
                    if(area > maxArea){ maxArea = area; }
                }
                idxStack.push(col);
            } 
        }
    }
    return maxArea;
}