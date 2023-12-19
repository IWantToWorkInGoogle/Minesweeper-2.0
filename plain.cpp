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
        case Level::SMALL:
            height = 9;
            width = 9;
            number_of_mines = 10;
            break;
        case Level::NORMAL:
            height = 16;
            width = 16;
            number_of_mines = 40;
            break;
        case Level::LARGE:
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

    exploded_mine = false;
    unmarked_mines = number_of_mines;
    rest = height * width - number_of_mines;

    plain = new int16_t*[height];
    visible = new int8_t*[height];
    for (uint16_t i = 0;i < height;i++)
    {
        plain[i] = new int16_t[width];
        visible[i] = new int8_t[width];
        std::fill(plain[i],plain[i] + width,0);
        std::fill(visible[i],visible[i] + width,HIDDEN);
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
    std::cout << "Revealed plain";
    for (size_t i = 0;i < width;i++)
    {
        std::cout << ' ' << i / 10 << ' ';
    }
    std::cout << '\n';
    for (size_t i = 0;i < width;i++)
    {
        std::cout << ' ' << i % 10 << ' ';
    }
    std::cout << '\n';

    for (size_t i = 0;i < height;i++)
    {
        for (size_t j = 0;j < width;j++)
        {
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
        std::cout << ' ' << i << '\n';
    }
}

void Plain::show_visibility() {
    for (size_t i = 0;i < height;i++)
    {
        for (size_t j = 0;j < width;j++)
        {
            std::cout << (int)visible[i][j] << ' ';
        }
        std::cout << '\n';
    }
}

void Plain::show()
{
    std::cout << "Plain:\n";
    for (size_t i = 0;i < width;i++)
    {
        std::cout << ' ' << i / 10 << ' ';
    }
    std::cout << '\n';
    for (size_t i = 0;i < width;i++)
    {
        std::cout << ' ' << i % 10 << ' ';
    }
    std::cout << '\n';


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
        std::cout << ' ' << i << '\n';
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

void Plain::open(uint16_t x, uint16_t y) {
    if (visibility(x,y) == VISIBLE || visibility(x,y) == MARKED)
    {
        return;
    }
    if (cell(x,y) == MINE)
    {
        exploded_mine = true;
        return;
    }
    std::queue<std::pair<int,int>> q;
    q.emplace(x,y);
    set_visibility(x,y,VISIBLE);
    --rest;
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
                --rest;
            }
        }
    }
}

void Plain::put_mark(uint16_t x, uint16_t y)
{
    if (visibility(x,y) == VISIBLE || visibility(x,y) == MARKED)
    {
        return;
    }

    set_visibility(x,y,MARKED);
    if (cell(x,y) == MINE)
    {
        --unmarked_mines;
    }
}

void Plain::remove_mark(uint16_t x, uint16_t y) {
    if (visibility(x,y) == VISIBLE || visibility(x,y) == HIDDEN)
    {
        return;
    }

    set_visibility(x,y,HIDDEN);
    if (cell(x,y) == MINE)
    {
        ++unmarked_mines;
    }
}

Status Plain::result() {
    if (!unmarked_mines && !rest)
    {
        return Status::WIN;
    }
    if (exploded_mine)
    {
        return Status::LOSE;
    }
    return Status::RUNNING;
}

void Plain::show_stats() {
    std::cout << "Rest: " << rest << "\nUnmarked mines: " << unmarked_mines << "\nExploded mine: " << exploded_mine << '\n';
}

SinglePlayer::SinglePlayer(Level level) : plain(Plain(level)), game_over(false) {
    plain.set_mines();
}

void SinglePlayer::open_cell(uint16_t x, uint16_t y)
{
    plain.open(x,y);
}

void SinglePlayer::put_mark(uint16_t x, uint16_t y) {
    plain.put_mark(x,y);
}

void SinglePlayer::remove_mark(uint16_t x, uint16_t y) {
    plain.remove_mark(x,y);
}

void SinglePlayer::update() {
    std::cout <<    "Operations:\n" <<
                    "1.Open cell\n" <<
                    "2.Put mark on cell\n" <<
                    "3.Remove mark from cell\n" <<
                    "Choose: ";
    int op;
    std::cin >> op;
    std::cout << "Pick cell: \n";
    uint16_t x,y;
    std::cin >> x >> y;
    switch (op) {
        case 1:
            open_cell(x,y);
            break;
        case 2:
            put_mark(x,y);
            break;
        case 3:
            remove_mark(x,y);
            break;
        default:
            break;
    }
}

void SinglePlayer::show() {
    plain.show();
}

void SinglePlayer::show_revealed() {
    plain.show_revealed();
}

void SinglePlayer::show_visibility() {
    plain.show_visibility();
}

void SinglePlayer::end_of_game() {
    Status status = plain.result();
    if (status == Status::RUNNING)
    {
        return;
    }
    game_over = true;
    std::cout << "Game over.\n";
    if (status == Status::WIN)
    {
        std::cout << "YOU WIN!!!!!\n";
    }
    else
    {
        std::cout << "You lose.\n";
    }
}

void SinglePlayer::run() {
    while (!game_over) {
        system("CLS");
        show();
        update();
        end_of_game();
    }
}