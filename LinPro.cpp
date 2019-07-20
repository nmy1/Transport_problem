#include <iostream>
#include <string.h>
#include <fstream>
using namespace std;

#define MAX 500

class linPro
{
    int p;                //Number of suppliers
    int q;                //Number of markets
    double arr[MAX][MAX]; //Relaxation array
    int mark[MAX];        //Note the basic value
public:
    void input();        //Input data from file and make up relaxation array
    void check();        //Check if the data is acceptable
    void transport();    //Algorithm for occasions that initial solution is not feasible
    void calculate();    //Core algorithm
    void route_output(); //Output the route
    void cost_output();  //Output the total cost
};

void linPro::input() //Input data from file and make up relaxation array
{
    ifstream OpenFile("file.txt");
    if (OpenFile.fail())
    {
        cout << "Can not open the file!" << endl;
        exit(0);
    }
    OpenFile >> p;
    OpenFile >> q;
    memset(arr, 0, sizeof(arr));
    for (int i = 0; i < p + q; i++)
    {
        double input;
        OpenFile >> input;
        arr[i + 1][0] = -input;
    }
    int xuhao = 1;
    for (int i = 0; i < p; i++)
    {
        for (int j = 0; j < q; j++)
        {
            double input;
            OpenFile >> input;
            arr[0][xuhao] = input;
            xuhao++;
        }
    }
    int h = 1;
    for (int i = 1; i <= p; i++)
    {
        for (int j = 1; j <= q; j++)
        {
            arr[i][h] = -1;
            h++;
        }
    }
    for (int i = p + 1; i <= p + q; i++)
    {
        int f = i - p;
        for (int j = 1; j <= p; j++)
        {
            arr[i][f] = -1;
            f += q;
        }
    }
    int number = p * q + 1;
    for (int i = 1; i <= p + q; i++)
    {
        arr[i][number] = 1;
        number++;
    }
    OpenFile.close(); //Close the file
}

void linPro::check() //Check if the data is acceptable
{
    double sumf = 0, summ = 0, wrong = 0;
    for (int i = 1; i <= p; i++)
    {
        sumf += arr[i][0];
    }
    for (int i = p + 1; i <= p + q; i++)
    {
        summ += arr[i][0];
    }
    for (int i = 1; i <= p * q; i++)
    {
        if (arr[0][i] <= 0)
        {
            wrong = 1;
            break;
        }
    }
    if (sumf != summ || wrong == 1)
    {
        cout << "We have the wrong data!" << endl;
        exit(0);
    }
}

void linPro::transport() //Algorithm for occasion that initial solution is not feasible
{
    int m = p + q + 1;
    int n = p * q + p + q + 1;
    for (int i = 0; i < p + q; i++)
    {
        mark[i + 1] = p * q + i + 1;
    }
    int fushu = 0;
    //Judge if negative absolute term exists
    for (int i = 1; i < m; i++)
    {
        if (arr[i][0] < 0)
        {
            fushu = 1;
        }
    }
    //If negative absolute term exists，do the algorithm
    if (fushu == 1)
    {
        for (int i = 1; i < m; i++)
        {
            if (arr[i][0] < 0)
            {
                int sp = -1; //Record the position of the value whose coefficient is negative
                for (int j = 1; j < n; j++)
                {
                    if (arr[i][j] < 0)
                    {
                        sp = j;
                        mark[i] = j;
                        break;
                    }
                }
                //If no negative coefficient exists,then we have no solution
                if (sp == -1)
                {
                    cout << "no solution" << endl;
                    exit(0);
                }
                for (int s = 0; s < m; s++)
                {
                    if ((s != i) && (arr[s][sp] != 0))
                    {
                        double z = arr[s][sp] / arr[i][sp];
                        for (int t = 0; t < n; t++)
                        {
                            arr[s][t] = arr[s][t] - z * arr[i][t];
                        }
                    }
                }
                double y = arr[i][sp];
                for (int t = 0; t < n; t++)
                {
                    arr[i][t] = arr[i][t] / y;
                }
                i = 0;
            }
        }
    }
}

void linPro::calculate() //Core algorithm
{
    int m = p + q + 1;
    int n = p * q + p + q + 1;
    int i = 1;
    while (i < n)
    {
        if (arr[0][i] < 0)
        {
            int x = i;
            int min = 1000000;
            int flag = 0;
            //Find the most tight restrictions
            for (int s = 1; s < m; s++)
            {
                if ((arr[s][x] > 0) && ((arr[s][0] / arr[s][x]) < min))
                {
                    min = arr[s][0] / arr[s][x];
                    flag = s;
                }
            }
            mark[flag] = x;
            //Variable substitution
            for (int s = 0; s < m; s++)
            {
                if ((s != flag) && (arr[s][x] != 0))
                {
                    double z = arr[s][x] / arr[flag][x];
                    for (int t = 0; t < n; t++)
                    {
                        arr[s][t] = arr[s][t] - z * arr[flag][t];
                    }
                }
            }
            double y = arr[flag][i];
            for (int t = 0; t < n; t++)
            {
                arr[flag][t] = arr[flag][t] / y;
            }
            i = 0;
        }
        i++;
    }
}

void linPro::route_output() //Output the route
{
    double route[p * q];
    memset(route, 0, sizeof(route));
    for (int i = 1; i <= p + q; i++)
    {
        if (mark[i] <= p * q)
        {
            route[mark[i] - 1] = arr[i][0];
        }
    }
    int count = 0;
    for (int i = 0; i < p; i++)
    {
        for (int j = 0; j < q; j++)
        {
            cout << "factory No." << i + 1 << " to market No." << j + 1 << ": " << route[count] << endl;
            count++;
        }
    }
}

void linPro::cost_output() //Output the total cost
{
    cout << "total cost: " << -arr[0][0] << endl;
}

int main() //Main function
{
    linPro tran;
    tran.input();
    tran.check();
    tran.transport();
    tran.calculate();
    tran.route_output();
    tran.cost_output();
    return 0;
}