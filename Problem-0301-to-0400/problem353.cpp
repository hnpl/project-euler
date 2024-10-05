// Question: https://projecteuler.net/problem=353

/*
    - Strategy:
        + Find the coordinates of all stations (coordinates that are all integer.)
        + Divide the coordinates to sectors (a cube of side "step" covering a certain part of a sphere.)
            + We won't build the adjacency list as when the sphere radius reaches 2^15-1, there'll be O(10^6) nodes, and there'll be O(10^9) edges with some heuristics. We cannot store all of them.
        + Apply the Dijkstra's algorithm to find the shortest path from the North Pole to the South Pole.
        + Fine tune the size of the cube to get the correct result.
        + A lot of heuristic won't work,
            + The sequence of stations in the path doesn't always have the z-coordinates monotocally increasing.
            + 
        + Some heuristic works,
            + By using the "sector" concept, we allow the z-coordinates to decrease to some extent while searching for the path.
            + Choosing "step" size affects the result.
*/

#include <iostream>
#include <vector>
#include <cmath>
#include <queue>
#include <utility>
#include <string>
#include <sstream>
#include <bitset>
#include <cstring>
#include <limits>
#include <unordered_set>
#include <unordered_map>
#include <iomanip>
#include <algorithm>

using namespace std;

typedef int32_t li;
typedef uint64_t ui;
typedef uint64_t Hash;
typedef __uint128_t SectorHash;
#define endl "\n"
#define pi 3.14159265358979323846

string ui128_to_string(__uint128_t n)
{
    stringstream stream;
    while (n > 0)
    {
        stream << char('0' + n % 10);
        n = n / 10;
    }

    string str = stream.str();
    reverse(str.begin(), str.end());

    if (str == "")
        return "0";
    return str;
}

ostream& operator<<(ostream& os, const __uint128_t& n)
{
    os << ui128_to_string(n);
    return os;
}

constexpr li pow2(li exp)
{
    return (1LL << exp);
};

class Coordinate3D
{
    public:
        li x, y, z;
        Coordinate3D()
        {
            x = 0;
            y = 0;
            z = 0;
        }
        Coordinate3D(li x, li y, li z)
        {
            this->x = x;
            this->y = y;
            this->z = z;
        }
        Coordinate3D(Hash hash)
        {
            this->x = li(hash >> 32LL & 0xFFFF);
            this->y = li((hash >> 16LL) & 0xFFFF);
            this->z = li(hash & 0xFFFF);
        }
        Hash get_hash() const
        {
            //return ((Hash(this->x) & 0xFFFF) << 32LL) | ((Hash(this->y) & 0xFFFF) << 16LL) | Hash(this->z & 0xFFFF);
            // https://stackoverflow.com/questions/40538108/convert-c-type-int16-t-to-int64-t-without-modifying-the-underlying-binary
            int16_t arr[4] = { this->z, this->y, this->x, 0 };
            Hash res;
            memcpy(&res, arr, sizeof(res));
            return res;
        }
        double get_distance(const Coordinate3D& other, const li& r) const
        {
            return r * acos((1.0 * this->x * other.x + 1.0 * this->y * other.y + 1.0 * this->z * other.z) / (1.0*r*r));
        }
        string to_string() const
        {
            stringstream s;
            s << "(" << this->x << ", " << this->y << ", " << this->z << ")";
            return s.str();
        }
};

class Sector
{
    public:
        int64_t x, y, z;
        static const int64_t step = 500;
        Sector()
        {
            x = 0;
            y = 0;
            z = 0;
        }
        Sector(int64_t x, int64_t y, int64_t z)
        {
            this->x = x;
            this->y = y;
            this->z = z;
        }
        Sector(const Coordinate3D& pos)
        {
            this->x = pos.x / step;
            this->y = pos.y / step;
            this->z = pos.z / step;
        }
        SectorHash get_hash() const
        {
            int32_t arr[4] = { this->z, this->y, this->x, 0 };
            SectorHash res;
            memcpy(&res, arr, sizeof(res));
            return res;
        }
        string to_string() const
        {
            stringstream s;
            s << "(" << this->x << ", " << this->y << ", " << this->z << ")";
            return s.str();
        }
};

vector<Coordinate3D> discover_stations(li r)
{
    vector<Coordinate3D> station_pos;

    li r_2 = r*r;
    for (li z = 0; z <= r; z++)
    {
        li z_2 = z*z;
        for (li y = 0; y <= r; y++)
        {
            li x_2 = r_2 - z_2 - y*y;
            if (x_2 < 0)
                break;
            li x = li(sqrt(x_2));
            if (x_2 == x*x)
            {
                station_pos.push_back(Coordinate3D(x, y, z));
                if (x > 0)
                    station_pos.push_back(Coordinate3D(-x, y, z));
                if (y > 0)
                    station_pos.push_back(Coordinate3D(x, -y, z));
                if (z > 0)
                    station_pos.push_back(Coordinate3D(x, y, -z));
                if ((x > 0) && (y > 0))
                    station_pos.push_back(Coordinate3D(-x, -y, z));
                if ((x > 0) && (z > 0))
                    station_pos.push_back(Coordinate3D(-x, y, -z));
                if ((y > 0) && (z > 0))
                    station_pos.push_back(Coordinate3D(x, -y, -z));
                if ((x > 0) && (y > 0) && (z > 0))
                    station_pos.push_back(Coordinate3D(-x, -y, -z));
            }
        }
    }

    return station_pos;
}

class Comparator
{
    public:
        bool operator()(const pair<ui, double>& left, const pair<ui, double>& right)
        {
            return left.second > right.second;
        }
};

class Graph
{
    public:
        vector<li> node_z;
        vector<Coordinate3D> nodes;
        vector<vector<pair<ui, double>>> neighbors;
        li r;
        ui start_index;
        ui end_index;
        
        Graph()
        {
            nodes.reserve(100000);
            neighbors.reserve(100000);
        }
        Graph(li r)
        {
            nodes.reserve(100000);
            neighbors.reserve(100000);
            this->r = r;
        }
        void add_node(const Coordinate3D& pos)
        {
            node_z.push_back(pos.z);
            nodes.push_back(pos);
            neighbors.push_back(vector<pair<ui, double>>());
            neighbors.back().reserve(1000);
            ui n_nodes = nodes.size();
            for (ui k = 0; k < n_nodes-1; k++)
            {
                double dist = pos.get_distance(nodes[k], this->r);
                //if (node_z[k] > pos.z)
                //{
                    neighbors.back().push_back(make_pair(k, dist*dist));
                //}
                //else
                //{
                    neighbors[k].push_back(make_pair(n_nodes-1, dist*dist));
                //}
            }
            if ((pos.x == 0) && (pos.y == 0) && (pos.z == -r))
                start_index = nodes.size() - 1;
            if ((pos.x == 0) && (pos.y == 0) && (pos.z == r))
                end_index = nodes.size() - 1;
        }
        void add_node_heuristic(const Coordinate3D& pos)
        {
            node_z.push_back(pos.z);
            nodes.push_back(pos);
            neighbors.push_back(vector<pair<ui, double>>());
            neighbors.back().reserve(1000);
            ui n_nodes = nodes.size();

            li z_lower_bound = pos.z - 100;
            li z_upper_bound = pos.z + 100;
            ui node_index = lower_bound(node_z.begin(), node_z.end(), z_lower_bound) - node_z.begin();
            ui curr_z = node_z[node_index];
            while (curr_z <= z_upper_bound && node_index < n_nodes)
            {
                double dist = pos.get_distance(nodes[node_index], this->r);
                if (node_z[node_index] > pos.z)
                    neighbors.back().push_back(make_pair(node_index, dist*dist));
                else
                    neighbors[node_index].push_back(make_pair(n_nodes - 1, dist*dist));
                node_index += 1;
            }

            if ((pos.x == 0) && (pos.y == 0) && (pos.z == -r))
                start_index = nodes.size() - 1;
            if ((pos.x == 0) && (pos.y == 0) && (pos.z == r))
                end_index = nodes.size() - 1;
        }
        double Dijkstra(ui start_index, ui end_index)
        {
            ui n_nodes = nodes.size();

            vector<double> dist(n_nodes, numeric_limits<double>::infinity());
            //vector<double> dist(n_nodes, 100000000000000000000.0);
            dist[start_index] = 0;

            vector<ui> prev(n_nodes, -1ULL);

            priority_queue<pair<ui, double>, vector<pair<ui, double>>, Comparator> pq;
            unordered_set<ui> visited;

            for (ui k = 0; k < n_nodes; k++)
                pq.push(make_pair(k, dist[k]));

            ui u_index = start_index;
            ui n = 0;
            while (u_index != end_index)
            {
                //cout << "Iter " << n << endl;
                n++;
                double u_dist = 0;
                do
                {
                    auto u = pq.top();
                    u_index = u.first;
                    u_dist = u.second;
                    pq.pop();
                }
                while (visited.find(u_index) != visited.end());

                visited.insert(u_index);
                for (auto v: neighbors[u_index])
                {
                    ui v_index = v.first;
                    double u_v_dist = v.second;
                    double alt_v_dist = u_dist + u_v_dist;
                    if (alt_v_dist < dist[v_index])
                    {
                        dist[v_index] = alt_v_dist;
                        prev[v_index] = u_index;
                        pq.push(make_pair(v_index, alt_v_dist));
                    }
                }
            }
/*
            ui w = end_index;
            while (w != start_index)
            {
                cout << w << "<-";
                w = prev[w];
            }
            cout << endl;
*/
            return dist[end_index] / (pi*pi*r*r);
        }

        string to_string() const
        {
            stringstream s;
            li n_nodes = nodes.size();
            for (ui k = 0; k < n_nodes; k++)
            {
                s << "index " << k << ": " << nodes[k].to_string() << endl;
                for (auto p: this->neighbors[k])
                    s << "-->" << p.first << " " << nodes[p.first].to_string() << " " << p.second << endl;
            }
            return s.str();
        }
};

class SectorBasedGraph
{
    public:
        vector<Coordinate3D> nodes;
        unordered_map<SectorHash, vector<ui>> sectors;
        li r;
        ui start_index;
        ui end_index;
        
        SectorBasedGraph()
        {
            nodes.reserve(100000);
            sectors.reserve(50000);
        }

        SectorBasedGraph(li r)
        {
            nodes.reserve(100000);
            sectors.reserve(50000);
            this->r = r;
        }

        void add_node(const Coordinate3D& pos)
        {
            nodes.push_back(pos);
            Sector sector(pos);
            SectorHash sector_hash = sector.get_hash();
            if (sectors.find(sector_hash) == sectors.end())
            {
                sectors[sector_hash] = vector<ui>();
                sectors[sector_hash].reserve(10000);
            }
            sectors[sector_hash].push_back(nodes.size() - 1);
            if ((pos.x == 0) && (pos.y == 0) && (pos.z == -r))
                start_index = nodes.size() - 1;
            if ((pos.x == 0) && (pos.y == 0) && (pos.z == r))
                end_index = nodes.size() - 1;
        }

        double Dijkstra(ui start_index, ui end_index)
        {
            ui n_nodes = nodes.size();

            vector<double> dist(n_nodes, numeric_limits<double>::infinity());
            dist[start_index] = 0;

            vector<ui> prev(n_nodes, -1ULL);

            priority_queue<pair<ui, double>, vector<pair<ui, double>>, Comparator> pq;
            unordered_set<ui> visited;

            //for (ui k = 0; k < n_nodes; k++)
            //    pq.push(make_pair(k, dist[k]));
            pq.push(make_pair(start_index, 0.0));

            ui u_index = start_index;
            while (u_index != end_index)
            {
                double u_dist = 0;
                do
                {
                    auto u = pq.top();
                    u_index = u.first;
                    u_dist = u.second;
                    pq.pop();
                }
                while (visited.find(u_index) != visited.end());


                visited.insert(u_index);
                /*
                for (auto v: neighbors[u_index])
                {
                    ui v_index = v.first;
                    double u_v_dist = v.second;
                    double alt_v_dist = u_dist + u_v_dist;
                    if (alt_v_dist < dist[v_index])
                    {
                        dist[v_index] = alt_v_dist;
                        prev[v_index] = u_index;
                        pq.push(make_pair(v_index, alt_v_dist));
                    }
                }*/
                Coordinate3D curr_pos = nodes[u_index];
                Sector curr_sector = Sector(curr_pos);
                for (int64_t dx = -1; dx <= 1; dx++)
                {
                    for (int64_t dy = -1; dy <= 1; dy++)
                    {
                        //for (int64_t dz = -1; dz <= 1; dz++)
                        for (int64_t dz = 0; dz <= 1; dz++)
                        {
                            Sector neighbor_sector(curr_sector.x + dx, curr_sector.y + dy, curr_sector.z + dz);
                            SectorHash neighbor_sector_hash = neighbor_sector.get_hash();
                            if (sectors.find(neighbor_sector_hash) == sectors.end())
                                continue;
                            for (auto v_index: sectors[neighbor_sector_hash])
                            {
                                Coordinate3D neighbor_pos = nodes[v_index];
                                //if (neighbor_pos.z < curr_pos.z)
                                //    continue;
                                double u_v_dist = curr_pos.get_distance(neighbor_pos, r);
                                u_v_dist *= u_v_dist;
                                double alt_v_dist = u_dist + u_v_dist;
                                if (alt_v_dist < dist[v_index])
                                {
                                    dist[v_index] = alt_v_dist;
                                    pq.push(make_pair(v_index, alt_v_dist));
                                }
                            }
                        }
                    }
                }
            }

/*
            ui w = end_index;
            while (w != start_index)
            {
                cout << w << "<-";
                w = prev[w];
            }
            cout << endl;
*/
            return dist[end_index] / (pi*pi*r*r);
        }
        string to_string() const
        {
            stringstream s;
            li n_nodes = nodes.size();
            for (ui k = 0; k < n_nodes; k++)
            {
                for (auto p: sectors)
                {
                    cout << "Sector Hash: " << hex << bitset<128>(p.first) << endl;
                    for (auto index: p.second)
                        cout << "--> " << index << endl;
                }
            }
            return s.str();
        }
};

bool comp_func(const Coordinate3D& left, const Coordinate3D& right)
{
    return left.z < right.z;
}

double M(ui n, bool use_sector_based_graph)
{
    double ans = 0.0;

    cout << n << endl;
    int64_t r = pow2(n) - 1;
    auto station_pos = discover_stations(r);

    if (use_sector_based_graph)
    {
        auto g = SectorBasedGraph(r);
        for (auto pos: station_pos)
            g.add_node(pos);
        ans = g.Dijkstra(g.start_index, g.end_index);
    }
    else
    {
        auto g = Graph(r);
        for (auto pos: station_pos)
            g.add_node(pos);
        ans = g.Dijkstra(g.start_index, g.end_index);
    }

    return ans;
}

double sum(ui max_n)
{
    double ans = 0;

    for (ui n = 1; n <= max_n; n++)
        ans += M(n, true);

    return ans;
}

int main()
{
    //cout << setprecision(11) << M(3, true) << endl << M(3, false) << endl;
    auto ans = sum(15);
    cout << setprecision(15) << ans;
    cout << endl;
    return 0;
}
