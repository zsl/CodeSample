#include <iostream>
#include <cstdlib>

using namespace std;

int main(int argc, char *argv[])
{
    if (argc < 3){
        cout << "invalid args.\n";
        exit(1);
    }

    int h = strtol(argv[1], NULL, 10);
    int m = strtol(argv[2], NULL, 10);

    h = h % 12; // Из13 -> 1

    double angle = h * 30 + m * 0.5 - m * 6;
    if (angle < 0) angle = -angle;
    if (angle > 180) angle = 360 - angle;

    cout << angle;
}
