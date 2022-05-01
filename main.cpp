#include <iostream>
#include <vector>
#include <queue>
#include <deque>

using namespace std;
using j_id_t = int;
using path_t = deque<int>;
using j_vec_t = vector<j_id_t>;
using j_vec_vec_t = vector<j_vec_t>;

deque<int> find_shortest_path(j_vec_vec_t& junctions, int n) {
    bool visited_junctions[n];
    for (int i = 0; i < n; ++i) {
        visited_junctions[i] = false;
    }
    int from[n];

    queue<int> queue1 = {};
    queue1.push(1);
    visited_junctions[0] = true;
    from[0] = -1;
    bool found_n = false;

    while(!queue1.empty() && !found_n) {
        for (int i : junctions[queue1.front() - 1]) {
            if (!visited_junctions[i - 1]) {
                visited_junctions[i - 1] = true;
                from[i - 1] = queue1.front();
                queue1.push(i);
                if (i == n) {
                    found_n = true;
                }
            }
        }
        queue1.pop();
    }

    if (found_n) {
        deque<int> sequence;
        sequence.push_back(n);
        int previous = from[n - 1];
        while(previous != -1) {
            sequence.push_front(previous);
            previous = from[previous - 1];
        }

        return sequence;
    } else {
        return {};
    }
}

j_vec_vec_t read_graph(int n, int m) {
    j_vec_vec_t j_vec;
    for (int i = 0; i < n; ++i) {
        j_vec_t empty_vector = {};
        j_vec.push_back(empty_vector);
    }

    int from;
    int to;

    for (int i = 0; i < m; ++i) {
        cin >> from;
        cin >> to;

        j_vec[from - 1].push_back(to);
        j_vec[to - 1].push_back(from);
    }

    return j_vec;
}

void print_graph(j_vec_vec_t& j_vec) {
    for (int i = 0; i < j_vec.size(); ++i) {
        cout << i + 1 << " : ";
        for (int j : j_vec[i]) {
            cout << j << " ";
        }
        cout << "\n";
    }
}

void print_path(path_t & v) {
    if (!v.empty()) {
        for (int i : v) {
            cout << i << " ";
        }
    } else {
        cout << "-1";
    }

    cout << "\n";
}

void print_vector(vector<int>& v) {
    for (int i : v) {
        cout << i << " ";
    }

    cout << "\n";
}

vector<bool> read_allowed_charge_values(int p, int z) {
    vector<bool> charge_values;
    charge_values.reserve(p + 1);
    for (int i = 0; i <= p; ++i) {
        charge_values.push_back(true);
    }
    int new_value;
    for (int i = 0; i < z; ++i) {
        cin >> new_value;
        charge_values[new_value] = false;
    }

    return charge_values;
}

vector<int> read_energy_values(int n) {
    vector<int> energy_values;
    int input;
    for (int i = 0; i < n; ++i) {
        cin >> input;
        energy_values.push_back(input);
    }
    return energy_values;
}

bool valid_end_charge(int n, vector<bool>& allowed_charge_values) {
    if (n >= allowed_charge_values.size() || n < 0) {
        return false;
    } else {
        return allowed_charge_values[n];
    }
}

vector<bool> calc_where_to_charge(path_t& path, int* energy_at_end, int p, int k, size_t path_length,
                                  vector<bool>& allowed_charge_values, vector<int>& energy_values) {
    bool can_reach_dest[p + 1][path_length];
    bool if_charge[p + 1][path_length];
    int max_dest_energy[p + 1][path_length];

    for (int i = path_length - 1; i >= 0; --i) {
        for (int j = 0; j <= p; ++j) {
            if ( i == path_length - 1) {
                can_reach_dest[j][i] = true;
                if (valid_end_charge(j + energy_values[i], allowed_charge_values)) {
                    max_dest_energy[j][i] = j + energy_values[i];
                    if_charge[j][i] = true;
                } else {
                    max_dest_energy[j][i] = j;
                    if_charge[j][i] = false;
                }
            } else {
                int first_option = -1;
                int second_option = -1;

                if (valid_end_charge(j + energy_values[i], allowed_charge_values) && j + energy_values[i] >= k) {
                    if (can_reach_dest[j + energy_values[i] - k][i + 1]) {
                        first_option = max_dest_energy[j + energy_values[i] - k][i + 1];
                    }
                }

                if (j >= k) {
                    if (can_reach_dest[j - k][i + 1]) {
                        second_option = max_dest_energy[j - k][i + 1];
                    }
                }

                if (first_option == -1 && second_option == -1) {
                    can_reach_dest[j][i] = false;
                } else if (first_option > second_option) {
                    if_charge[j][i] = true;
                    can_reach_dest[j][i] = true;
                    max_dest_energy[j][i] = first_option;
                } else {
                    can_reach_dest[j][i] = true;
                    if_charge[j][i] = false;
                    max_dest_energy[j][i] = second_option;
                }
            }
        }
    }

    cout << "   ";
    for (int i = 0; i < path_length; ++i) {
        cout << path[i] << "     ";
    }
    cout << "\n";

    for (int j = 0; j <= p ; ++j) {
        cout << j << " ";
        for (int i = 0; i < path_length; ++i) {
            if (can_reach_dest[j][i]) {
                if (if_charge[j][i]) {
                    cout << "+," << max_dest_energy[j][i] << " | ";
                } else {
                    cout << "-," << max_dest_energy[j][i] << " | ";
                }
            } else {
                cout << " X  | ";
            }
        }
        cout << "\n";
    }

    vector<bool> where_to_charge;
    where_to_charge.reserve(path_length);
    for (int i = 0; i < path_length; ++i) {
        where_to_charge.push_back(false);
    }

    if (can_reach_dest[p][0]) {
        *energy_at_end = max_dest_energy[p][0];

        int j = p;
        for (int i = 0; i < path_length; ++i) {
            where_to_charge[i] = if_charge[j][i];
            if (if_charge[j][i]) {
                j += energy_values[i] - k;
            } else {
                j -= k;
            }
        }

    } else {
        where_to_charge.push_back(false);
    }

    return where_to_charge;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int p, k, z;
    cin >> p;
    cin >> k;
    cin >> z;

    vector<bool> allowed_charge_values = read_allowed_charge_values(p, z);

    int n, m;
    cin >> n;
    cin >> m;

    j_vec_vec_t junctions = read_graph(n, m);
    //print_graph(junctions);

    vector<int> energy_values = read_energy_values(n);

    path_t path = find_shortest_path(junctions, n);
    //print_path(path);

    if (path.empty()) {
        cout << "-1";
    } else {
        int energy_at_end;
        vector<bool> where_to_charge = calc_where_to_charge(path, &energy_at_end, p, k, path.size(), allowed_charge_values, energy_values);

        if (where_to_charge.size() > path.size()) {
            cout << "-1";
        } else {
            j_vec_t charging_junctions;
            for (int i = 0; i < where_to_charge.size(); ++i) {
                if (where_to_charge[i]) {
                    charging_junctions.push_back(path[i]);
                }
            }

            cout << path.size() << " " << energy_at_end << " " << charging_junctions.size() << "\n";
            print_path(path);
            print_vector(charging_junctions);
        }
    }

    return 0;
}
