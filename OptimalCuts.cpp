#include <bits/stdc++.h>
using namespace std;

#define INF 10000
using Interval = pair <int, int>;

struct return2findOptimalCuts * findOptimalCuts(vector <struct Rectangle *> rects, set <int> x, set <int> y, struct Rectangle * reg, vector <int> seq, map <struct Rectangle *, int> * memo);

// Rectangle represented as a struct; bottomLeft and topRight as pair <int, int>
struct Rectangle
{
    Interval bottomLeft;
    Interval topRight;

    Rectangle(Interval bottomLeft, Interval topRight) : bottomLeft(bottomLeft), topRight(topRight) {}
};
struct AxisParallelLine
{
    int point;
    int axis;      // might wanna use 1/0 instead of 'x'/'y'
    // convention : x = 0, y = 1;
};

struct return1OptimalCuts
{
    bool flag;
    vector <int> vec;
    // vector <Rectangle> vectorRectangles;
    // vector <AxisParallelLine> vectorLines;
    int count;
};
struct return2findOptimalCuts
{
    vector <int> vec;
    int val;
};

bool intervalsIntersect(Interval int1, Interval int2)
{
    return ! ((int1.first >= int2.second) || (int2.first >= int1.second));
}
bool lineIntersectsRectangle(struct AxisParallelLine *line, struct Rectangle *rect)
{
    if (line->axis == 0)
    {
        pair <int, int> p1 = make_pair(rect->bottomLeft.first, rect->topRight.first);   // maybe change the parameters of intervalsIntersect
        pair <int, int> p2 = make_pair(line->point, line->point);                       // to accomodate more instead of wasting space by creating pairs?
        return intervalsIntersect(p1, p2);
    }
    else
    {
        pair <int, int> p1 = make_pair(rect->bottomLeft.second, rect->topRight.second);
        pair <int, int> p2 = make_pair(line->point, line->point);
        return intervalsIntersect(p1, p2);
    }
}
bool isDisjoint(struct Rectangle *rect1, struct Rectangle * rect2)
{
    pair <int, int> xInterval1 = make_pair(rect1->bottomLeft.first, rect1->topRight.first);
    pair <int, int> xInterval2 = make_pair(rect2->bottomLeft.first, rect2->topRight.first);
    pair <int, int> yInterval1 = make_pair(rect1->bottomLeft.second, rect1->topRight.second);
    pair <int, int> yInterval2 = make_pair(rect2->bottomLeft.second, rect2->topRight.second);
    return ! ((intervalsIntersect(xInterval1, xInterval2)) && (intervalsIntersect(yInterval1, yInterval2)));
}

struct return1OptimalCuts * optimalCuts(vector <struct Rectangle *> rects, struct Rectangle * reg)
{
    struct return1OptimalCuts * r; // = new struct return1OptimalCuts();
    vector <int> v;

    for (int i = 0; i < rects.size(); i++)
    {
        for (int j = i+1; j < rects.size(); j++)
        {
            if ( ! isDisjoint(rects[i], rects[j]) )
            {
                r->flag = false;
                r->vec = v;
                r->count = 0;
                return r;
            }
        }
    }

    map <struct Rectangle *, int> * memo;

    set <int> x;
    set <int> y;

    for (auto rectangle = rects.begin(); rectangle != rects.end(); ++rectangle)
    {
        x.insert((*rectangle)->bottomLeft.first);
        x.insert((*rectangle)->topRight.first);
        y.insert((*rectangle)->bottomLeft.second);
        y.insert((*rectangle)->topRight.second);
    }

    // the ordered set sorts the elements by default

    vector <int> seq;

    struct return2findOptimalCuts * r2;
    r2 = findOptimalCuts(rects, x, y, reg, seq, memo);
    
    r->flag = true;
    r->vec = r2->vec;
    r->count = r2->val;
    return r;
}

struct return2findOptimalCuts * findOptimalCuts(vector <struct Rectangle *> rects, set <int> x, set <int> y, struct Rectangle * reg, vector <int> seq, map <struct Rectangle *, int> * memo)
{   
    struct return2findOptimalCuts * r;
    vector <int> returnVec;
    r->vec = returnVec;

    if (rects.size() <= 3)
    {
        r->vec = seq;
        r->val = 0;
        return r;
    }

    // bit iffy 
    if ((*memo).find(reg) != (*memo).end())
    {
        r->val = (*memo)[reg];
        return r;
    }

    int numCandidates = x.size() + y.size() - 4;
    // int cuts[numCandidates];
    vector <int> cuts(numCandidates);
    for (int i = 0; i < numCandidates; i++)
    {
        cuts[i] = INF;
    }
    vector <int> sequences;

    for (int i = 1; i < x.size() - 1; i++)
    {
        struct AxisParallelLine * proposedCut;
        // redo maybe 
        // Sets do not provide random access iterators, but if you frequently need random access, you might consider using a different container like std::vector (if you can manage without the unique and sorting properties of a set), or maintaining a parallel vector that mirrors the set's elements for indexing purposes. 
        auto x_it = x.begin();
        advance(x_it, i);   
        proposedCut->point = *x_it;     // check if this works 
        proposedCut->axis = 0;

        vector <struct Rectangle *> rectsLeft;
        vector <struct Rectangle *> rectsRight;
        bool rectStartingAtBoundary = false;
        int currentKilled = 0;

        for (int j = 0; j < rects.size(); j++)
        {
            if ( lineIntersectsRectangle( proposedCut, rects[j] ) )
            {   
                currentKilled++;
            }
            else if (rects[j]->topRight.first <= *x_it)
            {
                rectsLeft.push_back(rects[j]);
            }
            else
            {
                rectsRight.push_back(rects[j]);
            }

            if (rects[j]->bottomLeft.first == *x_it)
            {
                rectStartingAtBoundary = true;
            }
        } 

        set <int> xLeft, xRight;
        int index = 0;
        for (auto it = x.begin(); it != x.end(); ++it, ++index) 
        {
            if (index <= i) 
            {
                xLeft.insert(*it);
            }
            if (rectStartingAtBoundary && index >= i) 
            {
                xRight.insert(*it);
            } 
            else if (!rectStartingAtBoundary && index > i) {
                xRight.insert(*it);
            }
        }

        set <int> yLeft;
        for (int j = 0; j < rectsLeft.size(); j++)
        {
            yLeft.insert(rects[j]->bottomLeft.second);
            yLeft.insert(rects[j]->topRight.second);
        }

        set <int> yRight;
        for (int j = 0; j < rectsRight.size(); j++)
        {
            yRight.insert(rects[j]->bottomLeft.second);
            yRight.insert(rects[j]->topRight.second);
        }

        struct Rectangle * regionLeft;
        regionLeft->bottomLeft = reg->bottomLeft; 
        pair <int, int> regionLeftPair = make_pair(*x_it, reg->topRight.second);
        regionLeft->topRight = regionLeftPair;

        struct Rectangle * regionRight;
        pair <int, int> regionRightPair = make_pair(*x_it, reg->bottomLeft.second);
        regionRight->bottomLeft = regionRightPair;
        regionRight->topRight = reg->topRight;

        struct return2findOptimalCuts * returnLeft;
        returnLeft = findOptimalCuts(rectsLeft, xLeft, yLeft, regionLeft, seq, memo);
        vector <int> seqLeft = returnLeft->vec;
        int killedLeft = returnLeft->val;

        struct return2findOptimalCuts * returnRight;
        returnRight = findOptimalCuts(rectsRight, xRight, yRight, regionRight, seq, memo);
        vector <int> seqRight = returnRight->vec;
        int killedRight = returnRight->val;
        
        cuts[i-1] = killedLeft + killedRight + currentKilled;
        for (int k = 0; k < seq.size(); k++)
        {
            sequences.push_back(seq[k]);
        }
        for (int k = 0; k < seqLeft.size(); k++)
        {
            sequences.push_back(seqLeft[k]);
        }
        for (int k = 0; k < seqRight.size(); k++)
        {
            sequences.push_back(seqRight[k]);
        }
        // sequences.push_back(seq + seqLeft + seqRight);
    }

    for (int i = 1; i < y.size() - 1; i++)
    {
        struct AxisParallelLine * proposedCut;
        // redo maybe 
        // Sets do not provide random access iterators, but if you frequently need random access, you might consider using a different container like std::vector (if you can manage without the unique and sorting properties of a set), or maintaining a parallel vector that mirrors the set's elements for indexing purposes. 
        auto y_it = y.begin();
        advance(y_it, i);   
        proposedCut->point = *y_it;     // check if this works 
        proposedCut->axis = 1;

        vector <struct Rectangle *> rectsBelow;
        vector <struct Rectangle *> rectsAbove;
        bool rectStartingAtBoundary = false;
        int currentKilled = 0;

        for (int j = 0; j < rects.size(); j++)
        {
            if ( lineIntersectsRectangle( proposedCut, rects[j] ) )
            {   
                currentKilled++;
            }
            else if (rects[j]->topRight.second <= *y_it)
            {
                rectsBelow.push_back(rects[j]);
            }
            else
            {
                rectsAbove.push_back(rects[j]);
            }

            if (rects[j]->bottomLeft.second == *y_it)
            {
                rectStartingAtBoundary = true;
            }
        } 

        set <int> yBelow, yAbove;
        int index = 0;
        for (auto it = x.begin(); it != x.end(); ++it, ++index) 
        {
            if (index <= i) 
            {
                yBelow.insert(*it);
            }
            if (rectStartingAtBoundary && index >= i) 
            {
                yAbove.insert(*it);
            } 
            else if (!rectStartingAtBoundary && index > i) {
                yAbove.insert(*it);
            }
        }

        set <int> xBelow;
        for (int j = 0; j < rectsBelow.size(); j++)
        {
            xBelow.insert(rects[j]->bottomLeft.first);
            xBelow.insert(rects[j]->topRight.first);
        }

        set <int> xAbove;
        for (int j = 0; j < rectsAbove.size(); j++)
        {
            xAbove.insert(rects[j]->bottomLeft.first);
            xAbove.insert(rects[j]->topRight.first);
        }

        struct Rectangle * regionBelow;
        regionBelow->bottomLeft = reg->bottomLeft; 
        pair <int, int> regionBelowPair = make_pair(reg->topRight.first, *y_it);
        regionBelow->topRight = regionBelowPair;

        struct Rectangle * regionAbove;
        pair <int, int> regionAbovePair = make_pair(reg->bottomLeft.first, *y_it);
        regionAbove->bottomLeft = regionAbovePair;
        regionAbove->topRight = reg->topRight;

        struct return2findOptimalCuts * returnBelow;
        returnBelow = findOptimalCuts(rectsBelow, xBelow, yBelow, regionBelow, seq, memo);
        vector <int> seqBelow = returnBelow->vec;
        int killedBelow = returnBelow->val;

        struct return2findOptimalCuts * returnAbove;
        returnAbove = findOptimalCuts(rectsAbove, xAbove, yAbove, regionAbove, seq, memo);
        vector <int> seqAbove = returnAbove->vec;
        int killedAbove = returnAbove->val;
        
        cuts[x.size() - 2 + i - 1] = killedBelow  + killedAbove + currentKilled;
        for (int k = 0; k < seq.size(); k++)
        {
            sequences.push_back(seq[k]);
        }
        for (int k = 0; k < seqBelow.size(); k++)
        {
            sequences.push_back(seqBelow[k]);
        }
        for (int k = 0; k < seqAbove.size(); k++)
        {
            sequences.push_back(seqAbove[k]);
        }
    }

    int minPtr = 0;

    for (int i = 0; i < numCandidates; i++)
    {
        if (cuts[i] < cuts[minPtr])
        {
            minPtr = i;
        }
    }

    struct AxisParallelLine * newLine;
    newLine->point = INF;
    newLine->axis = 0;

    if (minPtr < x.size() - 2)
    {
        auto newLine_x_it = x.begin();
        advance(newLine_x_it, 1 + minPtr);
        newLine->point = *newLine_x_it;
        newLine->axis = 0;
    }
    else
    {
        auto newLine_y_it = y.begin();
        advance(newLine_y_it, minPtr + 3 - y.size());
        newLine->point = *newLine_y_it;
        newLine->axis = 1;
    }

    ////////
    ////////
    ////////
    try {
        // Using find to check if the key exists in the map
        auto it = memo->find(reg);
        if (it != memo->end()) {
            // Key found, update the value
            it->second = cuts[minPtr];
        } else {
            // Key not found, handle accordingly
            // For example, you can insert a new key-value pair or handle it as an error
            std::cout << "Key not found in the map." << std::endl;
        }
    } catch (const std::exception& e) {
        std::cout << "Exception caught: " << e.what() << "\n"
                << "minPtr is " << minPtr 
                << " and sequences list has size " << sequences.size() 
                << " and cuts has size " << cuts.size() << std::endl;
    }

    
    r->val = cuts[minPtr];
    // r->vec =    

    // try
    // {
    //     memo->find(reg) = cuts[minPtr];
    // }
    // catch
    // {

    // }

    // try 
    // {
    //     int minPtr; // assuming minPtr is defined and initialized somewhere earlier in your code
    //     std::vector<std::string> sequence = sequences[minPtr];
    //     sequence.insert(sequence.begin(), {reg, newLine});

    //     memo[reg] = std::make_pair(sequence, cuts[minPtr]);
    // } 
    // catch (const std::exception& e) 
    // {
    //     std::cout << "Exception occurred: " << e.what() << std::endl;
    //     std::cout << "minPtr is " << minPtr << " and sequences list has size " << sequences.size() << " and cuts has size " << cuts.size() << std::endl;
    // }
}

// int main()
// {
//     vector <struct Rectangle *> testRectangles;

//     testRectangles.push_back(&Rectangle(Interval(0,0), Interval(1,4)));
//     testRectangles.push_back(&Rectangle(Interval(0,4), Interval(4,5)));
//     testRectangles.push_back(&Rectangle(Interval(1,0), Interval(2,3)));
//     testRectangles.push_back(&Rectangle(Interval(2,0), Interval(5,1)));
//     testRectangles.push_back(&Rectangle(Interval(2,1), Interval(4,2)));
//     testRectangles.push_back(&Rectangle(Interval(2,2), Interval(3,3)));
//     testRectangles.push_back(&Rectangle(Interval(3,2), Interval(4,4)));
//     testRectangles.push_back(&Rectangle(Interval(4,1), Interval(5,5)));
//     testRectangles.push_back(&Rectangle(Interval(1,3), Interval(3,4)));

//     struct Rectangle * inputRectangle;
//     inputRectangle->bottomLeft = make_pair(0, 0);
//     inputRectangle->topRight = make_pair(100, 100);

//     cout << optimalCuts(testRectangles, inputRectangle);
// }
int main() {
    vector<Rectangle*> testRectangles;

    // Allocate memory for Rectangle objects
    testRectangles.push_back(new Rectangle(Interval(0,0), Interval(1,4)));
    testRectangles.push_back(new Rectangle(Interval(0,4), Interval(4,5)));
    testRectangles.push_back(new Rectangle(Interval(1,0), Interval(2,3)));
    testRectangles.push_back(new Rectangle(Interval(2,0), Interval(5,1)));
    testRectangles.push_back(new Rectangle(Interval(2,1), Interval(4,2)));
    testRectangles.push_back(new Rectangle(Interval(2,2), Interval(3,3)));
    testRectangles.push_back(new Rectangle(Interval(3,2), Interval(4,4)));
    testRectangles.push_back(new Rectangle(Interval(4,1), Interval(5,5)));
    testRectangles.push_back(new Rectangle(Interval(1,3), Interval(3,4)));

    // Allocate memory for inputRectangle and initialize it
    Rectangle* inputRectangle = new Rectangle(Interval(0, 0), Interval(100, 100));

    // Call the optimalCuts function
    struct return1OptimalCuts * returning = optimalCuts(testRectangles, inputRectangle);
    cout << returning->count << endl;

    // Output the result
    // cout << result << endl;

    // Free the allocated memory for each rectangle
    for (auto rect : testRectangles) {
        delete rect;
    }

    // Free the allocated memory for inputRectangle
    delete inputRectangle;

    return 0;
}