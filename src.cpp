#include <bits/stdc++.h>
using namespace std;

struct Stats {
    double sum = 0.0;
    double mean = 0.0;
    double minv = numeric_limits<double>::infinity();
    double maxv = -numeric_limits<double>::infinity();
    size_t count = 0;
};

void print_usage(const string& prog) {
    cerr << "Usage:\n"
         << "  " << prog << " [numbers...]\n"
         << "  " << prog << " --file <path-to-text-file-with-numbers>\n\n"
         << "Options:\n"
         << "  --file PATH   Read whitespace-separated numbers from a file\n"
         << "  --help        Show this help message\n";
}

bool parse_numbers_from_file(const string& path, vector<double>& out) {
    ifstream in(path);
    if (!in) return false;

    string token;
    while (in >> token) {
        try {
            size_t parsed = 0;
            double value = stod(token, &parsed);
            if (parsed != token.size()) throw invalid_argument("trailing characters");
            out.push_back(value);
        } catch (const exception&) {
            cerr << "warning: skipped non-numeric value '" << token << "' in file '" << path
                 << "'\n";
        }
    }
    return true;
}

Stats compute_stats(const vector<double>& a) {
    Stats s;
    for (double x : a) {
        s.sum += x;
        s.minv = min(s.minv, x);
        s.maxv = max(s.maxv, x);
    }
    s.count = a.size();
    if (s.count) s.mean = s.sum / static_cast<double>(s.count);
    return s;
}

int main(int argc, char** argv) {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string prog = (argc > 0 && argv[0]) ? string(argv[0]) : "app";
    if (argc == 1) {
        print_usage(prog);
        return 0;
    }

    vector<double> numbers;
    string file_path;
    bool from_file = false;

    for (int i = 1; i < argc; ++i) {
        string arg = argv[i];
        if (arg == "--help" || arg == "-h") {
            print_usage(prog);
            return 0;
        } else if (arg == "--file") {
            if (i + 1 >= argc) {
                cerr << "error: --file requires a path\n\n";
                print_usage(prog);
                return 1;
            }
            from_file = true;
            file_path = argv[++i];
        } else {
            try {
                size_t pos = 0;
                double val = stod(arg, &pos);
                if (pos != arg.size()) throw invalid_argument("trailing");
                numbers.push_back(val);
            } catch (const exception&) {
                cerr << "warning: skipped non-numeric argument '" << arg << "'\n";
            }
        }
    }

    if (from_file) {
        vector<double> file_numbers;
        if (!parse_numbers_from_file(file_path, file_numbers)) {
            cerr << "error: could not open file: " << file_path << "\n";
            return 1;
        }
        numbers.insert(numbers.end(), file_numbers.begin(), file_numbers.end());
    }

    if (numbers.empty()) {
        cerr << "error: no numbers provided\n\n";
        print_usage(prog);
        return 1;
    }

    Stats s = compute_stats(numbers);

    cout.setf(std::ios::fixed); 
    cout << setprecision(6);
    cout << "count: " << s.count << "\n"
         << "sum:   " << s.sum << "\n"
         << "mean:  " << s.mean << "\n"
         << "min:   " << s.minv << "\n"
         << "max:   " << s.maxv << "\n";

    return 0;
}
