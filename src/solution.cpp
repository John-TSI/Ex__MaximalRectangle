#include<algorithm>
#include<stack>
#include"../inc/solution.hpp"


// SOLUTION ONE: RECURSION
int Solution::maximalRectangle(std::vector<std::vector<char>>& matrix)
{
    int rows = matrix.size(), cols = matrix[0].size(), area = rows*cols;
    if(area == 1){ return (int) (matrix[0][0] == '1'); }  // handle single element case

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
    if(rows*cols == 1){ return (int) (matrix[0][0] == '1'); }  // handle single element case

    std::vector<int> rowData(cols,0);  // stores cumulative sums of matrix cols, acts as a histogram of # of consecutive '1's as function of column index.
                                    // from here, a column's 'bar' refers to its cumulative sum --> higher bar == greater total sum   

    for(int row{0}; row<rows; ++row)
    {
        std::stack<int> idxStack{{-1}};  // stores indices of columns with bar at least as high as previous column (its 'neighbour')
        for(int col{0}; col<cols; ++col)
        { 
            // bars are updated per row
            if(matrix[row][col] == '0'){ rowData[col] = 0; }  // reset column total if interrupted by a '0'
            else{ rowData[col] += (matrix[row][col] - '0'); }
            
            if(idxStack.top() == -1 || rowData[col] >= rowData[idxStack.top()]){ idxStack.push(col); }   // if first col to be added to stack, or col has bar not lower than col at top of stack
            else  // col has lower bar than neighbour, preventing extension of rectangle of neighbour's bar height
            {
                // step back through the stack to determine the maximum area rectangle of height equal to neighbour's bar
                while(idxStack.top() != -1 && rowData[idxStack.top()] >= rowData[col])
                {
                    int nbrIdx = idxStack.top(); idxStack.pop(); int leftIdx = idxStack.top();  // nbrIdx is index of neighbour
                    int area = rowData[nbrIdx]*(col - leftIdx - 1);  // width of rectangle is the index distance between previous stack bar (leftIdx) and the current lower bar (col), minus 1
                    if(area > maxArea){ maxArea = area; }
                }
                idxStack.push(col);  // after all the stack pops, current bar becomes the next highest stack bar
            } 
        }
        // reached end of row, but also need to consider possible rectangles using bars still on the stack
        while(idxStack.top() != -1)
        {
            int rightIdx{cols}, nbrIdx = idxStack.top(); idxStack.pop(); int leftIdx = idxStack.top();  // only difference compared to above: rectangles extend to include last column
            int area = rowData[nbrIdx]*(rightIdx - leftIdx - 1);
            if(area > maxArea){ maxArea = area; }
        }
    }
    return maxArea;
}