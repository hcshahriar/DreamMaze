#include <iostream>
#include <vector>
#include <stack>
#include <ctime>
#include <cstdlib>
#include <conio.h>
#include <windows.h>
using namespace std;

const int WIDTH = 21;
const int HEIGHT = 21;

struct Cell {
    bool visited;
    bool topWall, bottomWall, leftWall, rightWall;
    bool inPath;
};

vector<vector<Cell>> maze(HEIGHT, vector<Cell>(WIDTH));
int px = 1, py = 1;

void initMaze() {
    for (int y = 0; y < HEIGHT; ++y)
        for (int x = 0; x < WIDTH; ++x) {
            maze[y][x].visited = false;
            maze[y][x].topWall = maze[y][x].bottomWall = true;
            maze[y][x].leftWall = maze[y][x].rightWall = true;
            maze[y][x].inPath = false;
        }
}

bool isValid(int x, int y) {
    return x > 0 && x < WIDTH - 1 && y > 0 && y < HEIGHT - 1;
}

vector<pair<int, int>> getNeighbours(int x, int y) {
    vector<pair<int, int>> n;
    if (isValid(x, y - 2) && !maze[y - 2][x].visited) n.push_back({x, y - 2});
    if (isValid(x + 2, y) && !maze[y][x + 2].visited) n.push_back({x + 2, y});
    if (isValid(x, y + 2) && !maze[y + 2][x].visited) n.push_back({x, y + 2});
    if (isValid(x - 2, y) && !maze[y][x - 2].visited) n.push_back({x - 2, y});
    return n;
}

void removeWall(int x1, int y1, int x2, int y2) {
    if (x1 == x2) {
        if (y1 > y2) maze[y1][x1].topWall = maze[y2][x2].bottomWall = false;
        else maze[y1][x1].bottomWall = maze[y2][x2].topWall = false;
    } else {
        if (x1 > x2) maze[y1][x1].leftWall = maze[y2][x2].rightWall = false;
        else maze[y1][x1].rightWall = maze[y2][x2].leftWall = false;
    }
}

void generateMaze(int x, int y) {
    stack<pair<int, int>> s;
    s.push({x, y});
    maze[y][x].visited = true;
    while (!s.empty()) {
        auto [cx, cy] = s.top();
        auto neighbours = getNeighbours(cx, cy);
        if (!neighbours.empty()) {
            auto [nx, ny] = neighbours[rand() % neighbours.size()];
            removeWall(cx, cy, nx, ny);
            maze[ny][nx].visited = true;
            s.push({nx, ny});
        } else {
            s.pop();
        }
    }
}

void drawMaze() {
    system("cls");
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            if (x == px && y == py) cout << "P";
            else if (x == WIDTH - 2 && y == HEIGHT - 2) cout << "E";
            else if (maze[y][x].inPath) cout << ".";
            else if (maze[y][x].topWall || maze[y][x].leftWall ||
                     maze[y][x].bottomWall || maze[y][x].rightWall)
                cout << "#";
            else cout << " ";
        }
        cout << "\n";
    }
}

bool canMove(int x, int y) {
    return isValid(x, y) && (!maze[y][x].topWall || !maze[y][x].bottomWall ||
                             !maze[y][x].leftWall || !maze[y][x].rightWall);
}

void movePlayer(char dir) {
    int nx = px, ny = py;
    if (dir == 'w' && !maze[py][px].topWall) ny--;
    else if (dir == 's' && !maze[py][px].bottomWall) ny++;
    else if (dir == 'a' && !maze[py][px].leftWall) nx--;
    else if (dir == 'd' && !maze[py][px].rightWall) nx++;
    if (canMove(nx, ny)) {
        px = nx;
        py = ny;
        maze[py][px].inPath = true;
    }
}

void showInstructions() {
    cout << "DreamMaze: A Surreal ASCII Maze Game\n";
    cout << "Use W A S D to move. Find the E (exit).\n";
    cout << "Press any key to begin...\n";
    _getch();
}

int main() {
    srand(time(0));
    initMaze();
    generateMaze(1, 1);
    maze[1][1].inPath = true;
    showInstructions();
    while (true) {
        drawMaze();
        if (px == WIDTH - 2 && py == HEIGHT - 2) {
            cout << "You escaped the DreamMaze!\n";
            break;
        }
        if (_kbhit()) {
            char ch = _getch();
            movePlayer(ch);
        }
        Sleep(50);
    }
    return 0;
}
