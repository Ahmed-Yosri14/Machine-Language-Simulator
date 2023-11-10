#include <iostream>
#include <bits/stdc++.h>
#define all(v) v.begin(), v.end()
#define each auto &
using namespace std;

void __1(string ins);
void __2(string ins);
void __3(string ins);
void __4(string ins);
void __5(string ins);
void __6(string ins);


string dec_to_base(int value, int base)
{
    int val = value;
    string new_value;
    while (val)
    {
        int rem = val % base;
        val /= base;
        char di = (rem < 10 ? rem + '0' : 'A' + rem - 10);
        new_value.push_back(di);
    }
    reverse(all(new_value));
    return new_value;
}
int base_to_dec(string value, int base)
{
    reverse(all(value));
    int pos{1}, num{};
    for (auto &c: value)
    {
        int di = (isalpha(c) ? c - 'A' + 10 : c - '0');
        num += di * pos;
        pos *= base;
    }
    return num;
}
int base_to_dec(char c)
{
    int di = (isalpha(c) ? c - 'A' + 10 : c - '0');
    return di;
}
string float_to_bi(double d)
{
    string val;
    for (int i = 0; i < 8; i++)
    {
        d *= 2;
        if (d > 1) val.push_back('1'), d--;
        else val.push_back('0');
    }
    return val;
}



class Memory_Cell
{
    protected:
    int value {0};
    bool visited {0};

    public:
    void set_value(int val)
    {
        value = val;
        visited = 1;
    }
    int get_value() const
    {
        return value;
    }
    string get_bi_value() const
    {
        string bi_val = dec_to_base(value, 2);
        while (bi_val.size() < 8) bi_val = '0' + bi_val;
        return bi_val;
    }
    int twos_comp_value() const
    {
        string bi_val = get_bi_value();
        int num{};
        bool rev{};
        if (bi_val[0] == '1')
        {
            for (int i{bi_val.size() - 1}; i >= 0; i--)
            {
                if (rev) bi_val[i] = !(bi_val[i] - '0') + '0';
                if (bi_val[i] - '0') rev = true;
            }
            return base_to_dec(bi_val, 2) * -1;
        }
        return value;
    }
    double float_value() const
    {
        string bi_val = get_bi_value(), man = bi_val.substr(4, 4);
        double num{};
        int dot_idx = base_to_dec(bi_val.substr(1, 3), 2) - 4;
        while (dot_idx < 0) man.insert(0, "0"), dot_idx++;
        int exp = dot_idx - 1;
        for (each c: man)
        {
            num += (c - '0') * pow(2, exp);
            exp--;
        }
        if (bi_val[0] - '0') num *= -1;
        return num;
    }
    bool is_visited()
    {
        return visited;
    }
    Memory_Cell()
    {

    }
    Memory_Cell(int value) : value(value)
    {
        visited = 1;
    }
    bool operator==(const Memory_Cell &rhs) const
    {
        return value == rhs.value;
    }
    Memory_Cell operator=(const Memory_Cell &rhs)
    {
        value = rhs.value;
        return *this;
    }
};

class Register: public Memory_Cell
{
    public:
    
    Register operator+=(const Register rhs)
    {
        value += rhs.value;
        return *this;
    }
    Register operator+(const Register rhs)
    {
        Register tmp = *this;
        tmp += rhs;
        return tmp;
    }
    Register operator=(const Memory_Cell &rhs)
    {
        value = rhs.get_value();
        return *this;
    }
};

ostream& operator<<(ostream& os, const Memory_Cell &r)
{
    os << r.get_value();
    return os;
}


template<typename T>
class Memory_Manager
{
    T *memory;
    int size;
    public:
    Memory_Manager(int sz) : size(sz)
    {
        memory = new T[size];
    }
    Memory_Manager(const Memory_Manager &src) : size(src.size)
    {
        memory = new T[size];
        for (int i{}; i < size; i++) memory[i] = src.memory[i];
    }
    T& operator[](int idx)
    {
        if (idx < 0 || idx >= size)
        {
            cout << "INVALID INSTRUCTION";
            throw(this);
        }
        return memory[idx];
    }
    bool valid(int idx)
    {
        return idx >= 0 && idx < size;
    }
    ~Memory_Manager()
    {
        delete [] memory;
    }
};


Memory_Manager<Memory_Cell> M(256);
Memory_Manager<Register> R(16);
int ptr{};

int main() 
{
    int op;
    cout << "Welcome Ya A7la User ^^\n";
    cout << "====================================\n";
    cout << "1-Load a new program from a file\n";
    cout << "2-Manulally insert instructions\n";
    cout << "====================================\n";
    cout << "Response: ";
    cin >> op;
    if (op == 1)
    {
        string file_name, ins;
        char a, b;
        int idx{};
        cout << "Filename: ";
        cin >> file_name;
        freopen(file_name.c_str(), "r", stdin);
        while (cin >> a >> b)
        {
            ins.push_back(a), ins.push_back(b);
            int val = base_to_dec(ins, 16);
            M[idx] = val;
            idx++;
            ins.clear();
        }
        for (int i{}; i <= idx; i++) cout << dec_to_base(M[i].get_value(), 16) << endl;
    }
} 

// copy from memory to register
void __1(string ins)
{
    int register_idx = base_to_dec(ins[1]), memory_cell_idx = base_to_dec(ins.substr(2, 2), 16);
    R[register_idx] = M[memory_cell_idx];
}

// assign value to register
void __2(string ins)
{
    int register_idx = base_to_dec(ins[1]), value = base_to_dec(ins.substr(2, 2), 16);
    R[register_idx] = value;
}


// copy from register to memory
void __3(string ins)
{
    int register_idx = base_to_dec(ins[1]), memory_cell_idx = base_to_dec(ins.substr(2, 2), 16);
    M[memory_cell_idx] = R[register_idx];
}

// copy from register to register
void __4(string ins)
{
    int register_idx_1 = base_to_dec(ins[2]), register_idx_2 = base_to_dec(ins[3]);
    R[register_idx_2] = R[register_idx_1];
}


// add (2's complement)
void __5(string ins)
{
    int r[3];
    for (int i{}; i < 3; i++) r[i] = base_to_dec(ins[i + 1]);
    int sum = R[r[1]].get_value() + R[r[2]].get_value();
    string bi_sum = dec_to_base(sum, 2);
    if (bi_sum.size() > 8) bi_sum = bi_sum.substr(bi_sum.size() - 8, 8);
    R[r[0]] = base_to_dec(bi_sum, 2);
}

// add (floating point)
void __6(string ins)
{
    int r[3], exp{4};
    for (int i{}; i < 3; i++) r[i] = base_to_dec(ins[i + 1]);
    string bi_val;
    double sum = R[r[1]].float_value() + R[r[2]].float_value();
    if (sum < 0) sum *= -1, bi_val.push_back('1');
    else bi_val.push_back('0');
    string man = float_to_bi(sum);
    while (man[0] == '0')
    {
        exp--;
        man.erase(0, 1);
    }
    bi_val += dec_to_base(exp, 2) + man.substr(0, 4);
    r[0] = base_to_dec(bi_val, 2);
}
