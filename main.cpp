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
