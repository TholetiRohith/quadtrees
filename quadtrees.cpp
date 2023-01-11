#include <iostream>
#include <cmath>
using namespace std;


class quad_tree
{
    int value;
    int height;
    quad_tree* topLeft;
    quad_tree* topRight;
    quad_tree* botLeft;
    quad_tree* botRight;

public:

    quad_tree(int n)
    {
        height = n;
        value = 0;
        topLeft = NULL;
        topRight = NULL;
        botLeft = NULL;
        botRight = NULL;

    }

    quad_tree(quad_tree const& Q)
    {
        delete topLeft;
        delete topRight;
        delete botLeft;
        delete botRight;
        value = Q.value;
        height = Q.height;
        topLeft = Q.topLeft;
        topRight = Q.topRight;
        botLeft = Q.botLeft;
        botRight = Q.botRight;
    }

    int size() const
    {
        return height;
    }

    int get(long long int x, long long int y) const
    {
        if (!this->topLeft)
        {
            return value;
        }
        long long half = (1 << (height - 1));
        if (x < half && y < half)
        {
            return topLeft->get(x, y);
        }
        else if (x >= half && y < half)
        {
            return botLeft->get(x - half, y);
        }
        else if (x < half && y >= half)
        {
            return topRight->get(x, y - half);
        }
        else
        {
            return botRight->get(x - half, y - half);
        }
    }

    void set(long long int x1, long long int y1, long long int x2, long long int y2, int b)
    {
        if (height == 0)
        {
            value = b;

            delete topLeft;
            delete topRight;
            delete botLeft;
            delete botRight;
            topLeft = NULL;
            topRight = NULL;
            botLeft = NULL;
            botRight = NULL;
            return;
        }
        
        long long int half =(1<<(height-1));

        if (x1 == 0 && x2 == (2 * half - 1) && y1 == 0 && y2 == (2 * half - 1))
        {
            value = b;
            delete topLeft;
            delete topRight;
            delete botLeft;
            delete botRight;
            topLeft = NULL;
            topRight = NULL;
            botLeft = NULL;
            botRight = NULL;
            return;
        }

        if (!this->topLeft)
        {
            if (value == b) return;
            topRight = new quad_tree(height - 1);
            topLeft = new quad_tree(height - 1);
            botLeft = new quad_tree(height - 1);
            botRight = new quad_tree(height - 1);
            
            value = -1;
        }
        if ((x1 < half) && (x2 < half))
        {
            if (y1 < half && y2 < half)
            {
                topLeft->set(x1, y1, x2, y2, b);
            }
            else if (y1 < half && y2 >= half)
            {
                topLeft->set(x1, y1, x2, half-1, b);
                topRight->set(x1, 0, x2, y2 - half, b);
            }
            else if (y1 >= half && y2 >= half)
            {
                topRight->set(x1, y1 - half, x2, y2 - half, b);
            }
        }

        else if ((x1 < half) && (x2 >= half))
        {
            if (y1 < half && y2 < half)
            {
                topLeft->set(x1, y1, half-1, y2, b);
                botLeft->set(0, y1, x2 - half, y2, b);
            }
            else if (y1 < half && y2 >= half)
            {
                topLeft->set(x1, y1, half-1, half-1, b);
                topRight->set(x1, 0, half-1, y2 - half, b);
                botLeft->set(0, y1, x2 - half, half-1, b);
                botRight->set(0, 0, x2 - half, y2 - half, b);
            }
            else if (y1 >= half && y2 >= half)
            {
                topRight->set(x1, y1 - half, half-1, y2 - half, b);
                botRight->set(0, y1 - half, x2 - half, y2 - half, b);
            }
        }

        else if ((x1 >= half) && (x2 >= half))
        {
            if (y1 < half && y2 < half)
            {
                botLeft->set(x1 - half, y1, x2 - half, y2, b);
            }
            else if (y1 < half && y2 >= half)
            {
                botLeft->set(x1 - half, y1, x2 - half, half-1, b);
                botRight->set(0, 0, x2 - half, y2 - half, b);
            }
            else if (y1 >= half && y2 >= half)
            {
                topRight->set(x1 - half, y1 - half, x2 - half, y2 - half, b);
            }
        }
        this->merge();

    }
    void merge()
    {
        if (!this->topLeft)
            return;
        if (!this->topLeft->topLeft && !this->topRight->topLeft && !this->botLeft->topLeft && !this->botRight->topLeft)
        {
            if (topLeft->value == topRight->value && topLeft->value == botRight->value && topLeft->value == botLeft->value)
            {
                value = topLeft->value;
                delete topLeft;
                delete topRight;
                delete botLeft;
                delete botRight;
                topLeft = NULL;
                topRight = NULL;
                botLeft = NULL;
                botRight = NULL;
                return;
            }
        }
        return;
    }


    void complement()
    {
        if (!this->topLeft)
        {
            if (value)
                value = 0;
            else value = 1;
            return;
        }
        topRight->complement();
        topLeft->complement();
        botLeft->complement();
        botRight->complement();
        return;
    }


    void overlap(quad_tree const& Q)
    {
        if (!this->topLeft)
        {
            if (value == 0)
            {
                delete topLeft;
                delete topRight;
                delete botLeft;
                delete botRight;
                value = Q.value;
                height = Q.height;
                this->botLeft = Q.botLeft;
                this->botRight = Q.botRight;
                this->topLeft = Q.topLeft;
                this->topRight = Q.topRight;
                return;

            }
            else return;
        }
        if (!Q.botLeft)
        {
            if (Q.value == 0)
            {
                return;
            }
            else
            {
                delete topLeft;
                delete topRight;
                delete botLeft;
                delete botRight;
                this->botLeft = Q.botLeft;
                this->botRight = Q.botRight;
                this->topLeft = Q.topLeft;
                this->topRight = Q.topRight;
                value = 1;
                return;
            }
        }
        topLeft->overlap(*Q.topLeft);
        botLeft->overlap(*Q.botLeft);
        botRight->overlap(*Q.botRight);
        topRight->overlap(*Q.topRight);
        merge();
    }

    void intersect(quad_tree const& Q)
    {
        if (!topLeft)
        {
            if (value == 1)
            {
                delete topLeft;
                delete topRight;
                delete botLeft;
                delete botRight;
                value = Q.value;
                height = Q.height;
                this->botLeft = Q.botLeft;
                this->botRight = Q.botRight;
                this->topLeft = Q.topLeft;
                this->topRight = Q.topRight;
                return;

            }
            else return;
        }
        if (!Q.botLeft)
        {
            if (Q.value == 1)
            {
                return;
            }
            else
            {
                delete topLeft;
                delete topRight;
                delete botLeft;
                delete botRight;
                this->botLeft = Q.botLeft;
                this->botRight = Q.botRight;
                this->topLeft = Q.topLeft;
                this->topRight = Q.topRight;
                value = 0;
                return;
            }
        }
        topLeft->intersect(*Q.topLeft);
        botLeft->intersect(*Q.botLeft);
        botRight->intersect(*Q.botRight);
        topRight->intersect(*Q.topRight);
        merge();
    }

    long long int diff()
    {
        if (!topLeft)
        {
            if (value == 0) return -(1 << height);
            else return (1 << height);
        }
        else
        {
            return topLeft->diff() + topRight->diff() + botRight->diff() + botLeft->diff();
        }

    }

    void resize(int m)
    {

        if (m >= height)
        {
            if (height == 0)
            {
                height = m;
                return;
            }
            height = m;
            if (!topLeft)
            {
                return;
            }
            topLeft->resize(m - 1);
            topRight->resize(m - 1);
            botLeft->resize(m - 1);
            botRight->resize(m - 1);
            merge();

            return;
        }
        if (m < height)
        {
            if (m == 0)
            {
                if (!topLeft)
                {
                    height = 0;
                    return;
                }

                if (this->diff() > 0)
                {
                    value = 1;
                    height = 0;
                    delete topLeft;
                    delete topRight;
                    delete botLeft;
                    delete botRight;
                    topLeft = NULL;
                    topRight = NULL;
                    botLeft = NULL;
                    botRight = NULL;
                    return;
                }
                value = 0;
                height = 0;
                delete topLeft;
                delete topRight;
                delete botLeft;
                delete botRight;
                topLeft = NULL;
                topRight = NULL;
                botLeft = NULL;
                botRight = NULL;
                return;
            }
            if (!topLeft)
            {
                height = m;
                return;
            }
            height = m;
            topLeft->resize(m - 1);
            topRight->resize(m - 1);
            botLeft->resize(m - 1);
            botRight->resize(m - 1);
            merge();
        }

    }


    ~quad_tree()
    {
        delete topLeft;
        delete topRight;
        delete botLeft;
        delete botRight;
    }

};
