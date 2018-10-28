#include <iostream>
#include <array>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <bitset>
#include <algorithm>


//Поля данных (по порядку в строке, слева-направо)
//IP:Port узла с которого отправлены данные
//MAC хоста с которого отправлены данные
//IP:Port узла на который отправлены данные
//MAC хоста на который отправлены данные
//Признак UDP: 'true' - UDP, 'false' - TCP
//Размер переданных данных, байт
//Время передачи, секунд
//
//Пример записи (одна строка): 94.239.149.85:46889;eF:Eb:4e:8:A3:08;161.223.117.183:7306;Ce:62:c5:00:BE:cd;false;10707;0.0941


struct TrafficDataPackage
{
	std::array<int, 5> IP_from {0, 0, 0, 0, 0};
	std::array<int, 6> MAC_from {0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
	std::array<int, 5> IP_to {0, 0, 0, 0, 0};
	std::array<int, 6> MAC_to {0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
	bool is_UDP {false};
	size_t bytes {0};
	double seconds {0};
};


// split string by all the delims
std::vector<std::string> split(const std::string& str, const std::string& delims)
{
    std::vector<std::string> tokens;
    size_t prev = 0, pos = 0;

    do
    {
    	size_t min_pos = str.length();
    	for (auto d : delims)
            min_pos = std::min(str.find(d, prev), min_pos);
        pos = min_pos;

        if (pos == std::string::npos)
        	pos = str.length();

        std::string token = str.substr(prev, pos-prev);

        if (!token.empty())
        	tokens.push_back(token);

        prev = pos + 1;
    }
    while (pos < str.length() && prev < str.length());

    return tokens;
}

std::array<int, 5> get_ip(const std::string& s)
{
	std::array<int, 5> ip {0,0,0,0,0};

	auto digits_s = split(s, ".:");
	if (digits_s.size() != 5)
		throw std::runtime_error("bad record");
	for (int i=0; i<4; i++)
		if (i<0 || i>255)
			throw std::runtime_error("bad record");
	if (ip[4] < 0 || ip[4] > 65535)
		throw std::runtime_error("bad record");

	for(int i=0; i<5; i++)
		ip[i] = std::stoi(digits_s[i]);

	return ip; // move
}

std::array<int, 6> get_mac(const std::string& s)
{
	std::array<int, 6> mac {0,0,0,0,0,0};

	auto digits_s = split(s, ":");
	if (digits_s.size() != 6)
		throw std::runtime_error("bad record");
	for (auto i : mac)
		if (i<0 || i>255)
			throw std::runtime_error("bad record");

	for(int i=0; i<6; i++)
		mac[i] = std::stoi(digits_s[i], nullptr, 16);

	return mac; // move
}


//                 5 ints             6 ints             4 ints         6 ints               bool size_t double
//              [         1       ];[        2     ];[      3      ];[       4            ];[ 5 ];[ 6 ];[  7 ]
// example:     94.239.149.85:46889;eF:Eb:4e:8:A3:08;161.223.117.183:7306;Ce:62:c5:00:BE:cd;false;10707;0.0941
TrafficDataPackage get_tdp(const std::string& s)
{
	TrafficDataPackage tdp;

	auto vs = split(s, ";");
	if (vs.size() != 7)
		throw std::runtime_error("bad record");

	tdp.IP_from = get_ip(vs[0]);
	tdp.MAC_from = get_mac(vs[1]);
	tdp.IP_to = get_ip(vs[2]);
	tdp.MAC_to = get_mac(vs[3]);
	tdp.is_UDP = (vs[4] == "true");
	tdp.bytes = std::stoul(vs[5]);
	tdp.seconds = std::stod(vs[6]);

	return tdp; // move
}

inline std::string ip_str(std::array<int,5> ip)
{
	return {std::to_string(ip[0]) + "." + std::to_string(ip[1]) + "." + std::to_string(ip[2]) + "." + std::to_string(ip[3])};
}

inline std::string subnet_str(std::array<int,5> ip)
{
	return {std::to_string(ip[0]) + "." + std::to_string(ip[1]) + "." + std::to_string(ip[2])};
}

//1. Сколько уникальных узлов в наблюдаемой сети (штук)?
// uniques in node_speed; ignore MAC
//2. Какова средняя скорость передачи данных всей наблюдаемой сети (байт/сек)?
// avg_speed
//3. Основываясь на данных о трафике, верно-ли утверждение "UDP используется для передачи данных с максимальной пиковой скоростью"?
// max_speed_is_UDP
//4. Укажите 10 узлов сети с самой высокой средней скоростью передачи данных (байт/сек).
// node_speed's top 10; ignore MAC
//5. Укажите 10 самых активных подсетей /24 (A.B.C.xxx) по критерию количества сессий передачи данных (штук).
// subnet_sessions top 10
//6. Есть-ли в сети узлы, которые могут являться посредниками (т.н. PROXY) между другими узлами?
// Укажите их, если они есть. Узел может считаться PROXY если он принимал и передавал данные как минимум двум другим узлам по
// одному и тому-же протоколу, каждому, т.е., например узлу 1 передавал и принимал по TCP, а узлу 2 по UDP.
// Прием по одному протоколу, а отправка по другому, одному и тому-же узлу, не допускается в работе PROXY.
// ((1 >tcp> 0 && 0 >tcp> 2) || (1 >udp> 0 && 0 >udp> 2)) && !((3 >tcp> 0 && 3 <udp< 0 ) || ( 3 >udp> 0 && 3 <tcp< 0)); ignore MAC

enum NodeRoles { UDP_source = 0, UDP_dest = 1, TCP_source = 2, TCP_dest = 3 }; // bits to set for each node role encountered


static std::unordered_map<std::string, double> node_speed;
static size_t packages;
static double avg_speed;
static std::pair<double, bool> max_speed_is_UDP;
static std::unordered_map<std::string, size_t> subnet_sessions;
static std::unordered_map<std::string, std::unordered_map<std::string, std::bitset<8>>> node_roles_with_neighbor;


// update formula: old_avg_speed * N == new_avg_speed * (N+1) - speed[N+1]
void upd_node_speed(const std::string& node, const TrafficDataPackage& tdp)
{
	double old_avg_speed = node_speed[node];
	node_speed[node] = (old_avg_speed*(packages-1) + (tdp.bytes/tdp.seconds)) / packages;
}

void aggregate(const TrafficDataPackage& tdp)
{
	auto ip_from_s = ip_str(tdp.IP_from);
	auto ip_to_s = ip_str(tdp.IP_to);

	upd_node_speed(ip_from_s, tdp);
	upd_node_speed(ip_to_s, tdp);

	packages++;

	double new_speed = tdp.bytes/tdp.seconds;
	double old_avg_speed = avg_speed;
	avg_speed = (old_avg_speed*(packages-1) + (new_speed)) / packages; // old_avg * N == new_avg * (N+1) - speed[N+1]

	if (new_speed > max_speed_is_UDP.first)
		max_speed_is_UDP = std::make_pair(new_speed, tdp.is_UDP);

	subnet_sessions[subnet_str(tdp.IP_from)]++;
	subnet_sessions[subnet_str(tdp.IP_to)]++;

	node_roles_with_neighbor[ip_from_s][ip_to_s][(tdp.is_UDP ? UDP_source : TCP_source)] = true;
	node_roles_with_neighbor[ip_to_s][ip_from_s][(tdp.is_UDP ? UDP_dest : TCP_dest)] = true;
}


struct TrafficAnalysisResult
{
	size_t unique_nodes {0};
	double avg_speed {0.0};
	bool max_speed_is_UDP {false};
	std::vector<std::pair<std::string, double>> node_speed_top10 {};
	std::vector<std::pair<std::string, size_t>> subnet_sessions_top10 {};
	std::vector<std::string> proxies {};
};

TrafficAnalysisResult prepare_results()
{
	TrafficAnalysisResult res;

	// 1
	res.unique_nodes = node_speed.size();

	// 2
	res.avg_speed = avg_speed;

	// 3
	res.max_speed_is_UDP = max_speed_is_UDP.second;

	// 4
	std::vector<std::pair<std::string, double>> speed_top10(10);
	std::partial_sort_copy(node_speed.begin(), node_speed.end(), speed_top10.begin(), speed_top10.end(),
		[](std::pair<const std::string, double> const& l, std::pair<const std::string, double> const& r) {
			return l.second > r.second;
		}
	);
	res.node_speed_top10 = std::move(speed_top10);

	// 5
	std::vector<std::pair<std::string, size_t>> sessions_top10(10);
	std::partial_sort_copy(subnet_sessions.begin(), subnet_sessions.end(), sessions_top10.begin(), sessions_top10.end(),
		[](std::pair<const std::string, size_t> const& l, std::pair<const std::string, size_t> const& r) {
			return l.second > r.second;
		}
	);
	res.subnet_sessions_top10 = std::move(sessions_top10);

	// 6
	for (auto& node : node_roles_with_neighbor)
	{
		bool red_flag = false;      // (3 >tcp> 0 && 3 <udp< 0) || (3 >udp> 0 && 3 <tcp< 0)
		bool green_flag = false;    // (1 >tcp> 0 && 0 >tcp> 2) || (1 >udp> 0 && 0 >udp> 2)

		for (auto& p : node.second)
		{
			green_flag = green_flag || ((p.second[0] && p.second[1]) || (p.second[2] && p.second[3]));
			red_flag = red_flag && !((p.second[0] && p.second[3]) || (p.second[1] && p.second[2]));

			if (red_flag) break;
		}

		if (green_flag && !red_flag)
			res.proxies.push_back(node.first);
	}

	return std::move(res);
}


int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		std::cout << "Input filename required as argument 1. Program aborted." << std::endl;
		exit(1);
	}

	std::string filename = argv[1];
	std::ifstream f(filename, std::ios::in);
	std::string s;
	TrafficDataPackage tdp {};

	if (!f)
	{
		std::cout << "Invalid filename. Program aborted." << std::endl;
		exit(2);
	}

	while(std::getline(f, s))
	{
		try {
			tdp = get_tdp(s);
		} catch(std::runtime_error& e) {
			// std::cerr << "broken traffic record" << std::endl;
		} catch(std::invalid_argument& e) {
			//
		} catch(std::out_of_range& e) {
			//
		} catch(std::exception& e) {
			//
		}

		aggregate(tdp);

		s.clear();
	}

	TrafficAnalysisResult res = prepare_results();

	std::cout << "1. Unique nodes count: " << res.unique_nodes << " unique IP-s" << std::endl;
	std::cout << "2. Average speed of data transmission in network: " << res.avg_speed << " bytes/second" << std::endl;
	std::cout << "3. Does UDP protocol provide max speed in network: " << (res.max_speed_is_UDP ? "yes" : "no") << std::endl;
	std::cout << "4. Top 10 nodes with highest average speed of transmission:" << std::endl;
	for (auto& n : res.node_speed_top10)
		std::cout << "\t" << n.first << std::endl;
	std::cout << "5. Top 10 most active /24 subnets:" << std::endl;
	for (auto& s : res.subnet_sessions_top10)
		std::cout << "\t" << s.first << ".X" << std::endl;
	std::cout << "6. Are there any proxies in network: " << (res.proxies.size() ? "yes" : "no") << std::endl;
	for (auto& p : res.proxies)
		std::cout << "\t" << p << std::endl;

	return 0;
}