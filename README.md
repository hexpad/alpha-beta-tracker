# 🛰️ Alpha-Beta Motion Tracker

The aim of this project is to demonstrate how the position and velocity of an object can be estimated from noisy measurements, and how its location in the near future can be predicted.

<img width="1252" height="709" alt="0 85 , 0 05" src="https://github.com/user-attachments/assets/0aa5cb82-44ad-4c30-8ba2-3b447fb8817f" />

# ✨ What does it do?

In the real world, sensors rarely provide perfectly accurate coordinates. If you rely solely on raw data, the path an object follows will appear choppy and irregular.

In short, the program:

-Reads location measurements from a text file.

-Smooths these measurements using an Alpha-Beta filter.

-Estimates the speed of the object.

-Predicts the object's position after 5 seconds.

-Writes all the results to a CSV file so that they can be plotted or analyzed.

# What is an Alpha-Beta Filter?

This filter assumes the object is moving at a constant speed.

The idea is :

1) By looking at the previous movement, it predicts where the object should be.

2) Compares this estimate to the actual measurement.

3) Corrects position and speed in small amounts.

It uses two tuning parameters:

Alpha (α) – how much we trust the new measurement

Beta (β) – how quickly we update the estimated speed

# 1. Prepare your data
Create a file named measurements.txt in your project folder. The format should be as follows:

```cpp
time   measured_x   measured_y
```

# Example

```cpp
0.0   0.1   0.0
1.0   1.0   1.1
2.0   2.1   2.0
3.0   3.0   3.1
4.0   4.0   4.1
5.0   5.1   5.0
6.0   6.0   6.1
7.0   7.1   7.0
8.0   8.0   8.1
9.0   9.1   9.0
10.0 10.0  10.1
11.0 11.0  11.8
12.0 12.0  13.0
13.0 13.0  14.5
14.0 14.0  16.2
15.0 15.0  18.0
16.0 16.0  19.5
17.0 17.0  20.8
18.0 18.0  21.8
19.0 19.0  22.6
20.0 20.0  23.2

```

Initially, the motion is almost linear. Then, the motion in the y-direction accelerates, which makes it easier to observe the tracking behavior.

# 2. Compile & Run

```cpp
#include <iostream>
#include <fstream>

class AlphaBetaTracker
{
private:
    double alpha, beta;

    double x, y;
    double vx, vy;

    double lastT;
    bool started;

public:
    AlphaBetaTracker(double a, double b)
        : alpha(a),
        beta(b),
        x(0.0),
        y(0.0),
        vx(0.0),
        vy(0.0),
        lastT(0.0),
        started(false)
    {

    }

    void update(double t, double mx, double my)
    {
        // first data
        if (!started)
        {
            x = mx;
            y = my;
            lastT = t;
            started = true;
            return;
        }

        double dt = t - lastT;
        if (dt <= 0)
            return;

        // prediction
        double px = x + vx * dt;
        double py = y + vy * dt;

        // error
        double ex = mx - px;
        double ey = my - py;

        // update position
        x = px + alpha * ex;
        y = py + alpha * ey;

        // update velocity
        vx = vx + (beta / dt) * ex;
        vy = vy + (beta / dt) * ey;

        lastT = t;
    }

    void predict(double sec, double& fx, double& fy)
    {
        fx = x + vx * sec;
        fy = y + vy * sec;
    }

    double getX() { return x; }
    double getY() { return y; }
    double getVx() { return vx; }
    double getVy() { return vy; }
};

int main()
{
    double alpha, beta;
    std::cout << "alpha: ";
    std::cin >> alpha;
    std::cout << "beta: ";
    std::cin >> beta;

    AlphaBetaTracker tracker(alpha, beta);

    std::ifstream file("measurements.txt");
    if (!file)
    {
        std::cout << "file error\n";
        return 1;
    }

    std::ofstream out("tracker_output.csv");
    out << "t,mx,my,x,y,vx,vy,fx,fy\n";

    double t, mx, my;

    while (file >> t >> mx >> my)
    {
        tracker.update(t, mx, my);

        double fx, fy;
        tracker.predict(5.0, fx, fy);

        out << t << ","
            << mx << "," << my << ","
            << tracker.getX() << "," << tracker.getY() << ","
            << tracker.getVx() << "," << tracker.getVy() << ","
            << fx << "," << fy << "\n";
    }

    std::cout << "done\n";
    return 0;
}

```

It will ask you to enter the alpha and beta values:
```cpp
alpha: 0.85
beta: 0.05
```
These values controls how responsive the tracker behaves.

# 3. Analyze the Results
Generates a file named tracker_output.csv. You can open this file with Excel or Python to visualize the raw data, filtered data, and future location predictions.

