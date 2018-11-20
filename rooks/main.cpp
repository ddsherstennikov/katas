#include <iostream>
#include <array>
#include <mutex>
#include <thread>
#include <vector>
#include <memory>
#include <random>
#include <utility>
#include <chrono>
#include <iomanip>
#include <shared_mutex>

// 0. На шахматной доске находятся в произвольной позиции N ладей (4-6)
// 1. Они все одновременно начинают ходить на случайные позиции (при этом перемещаться они, естественно, могут только горизонтально либо вертикально).
// 2. Между каждыми ходами каждая фигура делает паузу 200-300 миллисекунд.
// 3. Если на пути фигуры оказывается другая, она ждет, пока путь освободится. Если в течение 5 секунд проход не освободился, выбирается другая позиция аналогичным случайным образом.
// 4. Всё заканчивается, когда все фигуры сделают по 50 ходов
// * GUI никакое не требуется, достаточно будет понятного текстового вывода.


using Board = std::array<std::array<std::shared_timed_mutex, 8>, 8>;   // not RAII
using Pos = std::pair<int, int>;

thread_local uint rook_moves = 0;
std::mutex cout_mx;
Board board;

constexpr const std::array<char,8> ax1 = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H' };
constexpr const std::array<uint,8> ax2 = { 1, 2, 3, 4, 5, 6, 7, 8 };

int random_num(const int lo, const int hi)
{
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 eng(rd()); // seed the generator
    std::uniform_int_distribution<int> distr(lo, hi); // define the range

    return distr(eng); // generate numbers
}

Pos gen_random_rook_pos(Pos pos)
{
    Pos new_pos;

    do            // random choice of axis
    {
        new_pos = (random_num(0, 1) ? Pos{pos.first, random_num(0, 7)} : Pos{random_num(0, 7), pos.second});
    }
    while (new_pos == pos);

    return new_pos;
}


std::vector<Pos> get_rook_path(Pos pos, Pos new_pos)
{
    std::vector<Pos> path;

    if (pos == new_pos)
        throw std::runtime_error("Position generation error: pos == new pos.");

    if (pos.first == new_pos.first)
    {
        for (int ax2=std::min(pos.second, new_pos.second)+1; ax2<std::max(pos.second, new_pos.second); ax2++)
            path.emplace_back(pos.first, ax2);
    }
    else if (pos.second == new_pos.second)
    {
        for (int ax1=std::min(pos.first, new_pos.first)+1; ax1<std::max(pos.first, new_pos.first); ax1++)
            path.emplace_back(ax1, pos.second);
    }
    else
        throw std::runtime_error("Position generation error: both axes differ.");

    return path;
}


// 5sec time-lock exclusive ownership on new_rook_pos and shared ownership on path [rook_pos; new_rook_pos]
bool do_move_rook(Pos rook_pos, Pos new_rook_pos)
{
    using sec = std::chrono::seconds;

    auto deadline = std::chrono::steady_clock::now() + sec(5);
    auto rook_path = get_rook_path(rook_pos, new_rook_pos);
    int step = 0;
    bool result = false;

    for (; step<rook_path.size(); step++)                                                   // try shared-lock path in coordinate ascension order
    {
        auto pos = rook_path[step];
        if (!board[pos.first][pos.second].try_lock_shared_until(deadline))
            break;
    }

    if (step == rook_path.size() &&                                                         // whole path locked in shared mode until deadline
        board[new_rook_pos.first][new_rook_pos.second].try_lock_until(deadline))            // AND new pos locked exclusively until deadline
    {
        board[rook_pos.first][rook_pos.second].unlock();                                    // unlock old pos
        result = true;
    }

    for (step--; step>=0; step--)                                                           // shared-unlock path in coordinate descension order
    {
        auto pos = rook_path[step];
        board[pos.first][pos.second].unlock_shared();
    }

    return result;
}


void rook_func(int rook_id, Pos rook_pos)
{
    using msec = std::chrono::milliseconds;

    Pos new_rook_pos;

    while (rook_moves < 50)
    {
        do
        {
            new_rook_pos = gen_random_rook_pos(rook_pos);
        }
        while(!do_move_rook(rook_pos, new_rook_pos));

        {
            std::lock_guard<std::mutex> lk(cout_mx);
            std::cout << "rook" << rook_id << " move" << std::setw(2) << rook_moves+1 << "\t\t"
                      << ax1[rook_pos.first] << ax2[rook_pos.second] << '-'
                      << ax1[new_rook_pos.first] << ax2[new_rook_pos.second]
                      << std::endl << (rook_moves == 49 ? ("--- rook" + std::to_string(rook_id) + " FINISH! ---\n") : "")
                      << std::flush;
        }

        rook_pos = new_rook_pos;
        rook_moves++;

        if (rook_moves < 50)
            std::this_thread::sleep_for(msec(random_num(200, 300)));
    }

    board[rook_pos.first][rook_pos.second].unlock();                                    // unlock final pos
}


int main(int argc, char* argv[])
{
    int n_rooks = 4;

    if (argc == 2)
    try
    {
        int tmp = std::stoi(argv[1]);
        n_rooks = (4 <= tmp && tmp <= 6) ? tmp : n_rooks;
    }
    catch(...)
    {}

    std::vector<std::thread> rooks;
    for (int i=0; i<n_rooks; i++)
    {
        Pos i_start_pos;
        do
        {
            i_start_pos = std::make_pair(random_num(0,7), random_num(0,7));
        }
        while (!board[i_start_pos.first][i_start_pos.second].try_lock());

        std::thread th(rook_func, i, i_start_pos);
        rooks.push_back(std::move(th));
    }

    for (auto& rk : rooks)
        rk.join();

    return 0;
}