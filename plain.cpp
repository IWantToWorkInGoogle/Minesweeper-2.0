//
// Created by mufti on 19.12.2023.
//

#include "plain.h"

#define MINE (-1)
#define NO_NEIGHBOURS 0

#define HIDDEN 0
#define MARKED 1
#define VISIBLE 2

std::vector<std::pair<int16_t,int16_t>> pos = {
        {-1,-1},{0,-1},
        {1,-1},{1,0},
        {1,1},{0,1},
        {-1,1},{-1,0}
};

Plain::Plain(Level level)
{
    switch (level)
    {
        case Level::EASY:
            height = 9;
            width = 9;
            number_of_mines = 10;
            break;
        case Level::NORMAL:
            height = 16;
            width = 16;
            number_of_mines = 40;
            break;
        case Level::HARD:
            height = 16;
            width = 30;
            number_of_mines = 99;
            break;
        case Level::CUSTOM:
            height = 0;
            width = 0;
            number_of_mines = 0;
            break;
    }

    rest = height * width;

    plain = new int16_t*[height];
    visible = new int8_t*[height];
    for (uint16_t i = 0;i < height;i++)
    {
        plain[i] = new int16_t[width];
        visible[i] = new int8_t[width];
        std::fill(plain[i],plain[i] + width,0);
        std::fill(visible[i],visible[i] + width,0);
    }

    for (uint16_t i = 0;i < height;i++)
    {
        for (uint16_t j = 0;j < width;j++)
        {
            free_cells.emplace(i,j);
        }
    }
}

void Plain::show_revealed()
{
    for (size_t i = 0;i < height;i++)
    {
        for (size_t j = 0;j < width;j++)
        {
            std::cout << plain[i][j] << ' ';
        }
        std::cout << '\n';
    }
}

void Plain::show()
{
    std::cout << "Plain:\n";
    for (size_t i = 0;i < height;i++)
    {
        for (size_t j = 0;j < width;j++)
        {
            if (visibility(i,j) != VISIBLE)
            {
                std::cout << '[';
                switch (visibility(i,j))
                {
                    case HIDDEN:
                        std::cout << '.';
                        break;
                    case MARKED:
                        std::cout << 'X';
                        break;
                }
                std::cout << ']';
                continue;
            }

            std::cout << '[';
            switch (plain[i][j]) {
                case MINE:
                    std::cout << '*';
                    break;
                case NO_NEIGHBOURS:
                    std::cout << ' ';
                    break;
                default:
                    std::cout << char('0' + plain[i][j]);
                    break;
            }
            std::cout << ']';
        }
        std::cout << '\n';
    }
}

bool inside(int x,int y,int xs,int ys)
{
    return x >= 0 && x < xs && y >= 0 && y < ys;
}

void Plain::set_mines()
{
    std::mt19937 gen(std::chrono::steady_clock::now().time_since_epoch().count());
    for (size_t i = 0;i < number_of_mines;i++)
    {
        std::uniform_int_distribution<> distrib(0,(int)free_cells.size() - 1);
        auto it = free_cells.begin();
        auto rnd = distrib(gen);
        std::advance(it,rnd);
        plain[it->first][it->second] = -1;
        free_cells.erase(it);
    }

    for (int i = 0;i < height;i++)
    {
        for (int j = 0;j < width;j++)
        {
            if (plain[i][j] == MINE)
            {
                continue;
            }
            int16_t count = 0;
            for (auto p : pos)
            {
                int x = i + p.first;
                int y = j + p.second;
                if (inside(x,y,height,width) && plain[x][y] == MINE)
                {
                    ++count;
                }
            }
            plain[i][j] = count;
        }
    }
}

int16_t Plain::visibility(uint16_t x, uint16_t y)
{
    return visible[x][y];
}

void Plain::set_visibility(uint16_t x, uint16_t y, int8_t type) {
    visible[x][y] = type;
}

bool Plain::is_visible(uint16_t x, uint16_t y)
{
    return visibility(x,y) == VISIBLE;
}

int16_t Plain::cell(uint16_t x, uint16_t y)
{
    return plain[x][y];
}

bool Plain::open(uint16_t x, uint16_t y) {
    if (cell(x,y) == MINE)
    {
        return false;
    }
    std::queue<std::pair<int,int>> q;
    q.emplace(x,y);
    set_visibility(x,y,VISIBLE);
    while (!q.empty()) {
        auto p1 = q.front();
        q.pop();

        int x1 = p1.first, y1 = p1.second;
        std::cout << x1 << " " << y1 << '\n';
        std::cout << cell(x1,y1) << '\n';

        if (cell(x1,y1) != NO_NEIGHBOURS)
        {
            continue;
        }

        for (auto p2 : pos)
        {
            int x2 = x1 + p2.first;
            int y2 = y1 + p2.second;

            if (inside(x2,y2,height,width) && !is_visible(x2,y2) && (cell(x2,y2) != MINE))
            {
                q.emplace(x2,y2);
                set_visibility(x2,y2,VISIBLE);
            }
        }
    }
    return true;
}

bool Plain::put_flag(uint16_t x, uint16_t y)
{
    if (visibility(x,y) == VISIBLE || visibility(x,y) == MARKED)
    {
        return false;
    }



    if (cell(x,y) == MINE)
    {
        return true;
    }
    return false;
}

SinglePlayer::SinglePlayer(Level level) : plain(Plain(level)), game_over(false) {
    plain.set_mines();
}

void SinglePlayer::open_cell(uint16_t x, uint16_t y)
{
    if (plain.is_visible(x,y))
    {
        return;
    }
    game_over = !plain.open(x,y);
}

void SinglePlayer::put_flag(uint16_t x, uint16_t y) {
    if (plain.is_visible(x,y))
    {
        return;
    }

}

void SinglePlayer::show() {
    plain.show();
}

void SinglePlayer::show_revealed() {
    plain.show_revealed();
}

void SinglePlayer::run() {
    std::cout.flush();
    while (!game_over) {
        std::cout.flush();
        show();
        //show_revealed();
        std::cout << "Pick cell: \n";
        uint16_t x,y;
        std::cin >> x >> y;
        open_cell(x,y);
    }

    std::cout << "Game over\n";
}
