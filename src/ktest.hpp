//
// Created by cyan on 4/17/25.
//

#ifndef KTEST_HPP
#define KTEST_HPP

#include <string>
#include <vector>
#include <functional>
#include <unistd.h>
#include <cerrno>
#include <cstring>
#include <csignal>
#include <sys/wait.h>
#include <cstdlib>

namespace ktest {
    class KTestTest;

    static std::vector<KTestTest> tests;

    class KTestTest {
        std::string name_;
        std::function<void()> fn_;

    public:
        KTestTest(const std::string &name, const std::function<void()> &fn)
            : name_(name),
              fn_(fn) {
            tests.push_back(*this);
        }

        KTestTest(const KTestTest &other) = default;

        KTestTest(KTestTest &&other) noexcept
            : name_(std::move(other.name_)),
              fn_(std::move(other.fn_)) {
        }

        KTestTest &operator=(const KTestTest &other) {
            if (this == &other)
                return *this;
            name_ = other.name_;
            fn_ = other.fn_;
            return *this;
        }

        KTestTest &operator=(KTestTest &&other) noexcept {
            if (this == &other)
                return *this;
            name_ = std::move(other.name_);
            fn_ = std::move(other.fn_);
            return *this;
        }

        std::string name() const {
            return name_;
        }

        void operator()() const {
            this->fn_();
        }
    };

#define KTEST(name) \
    void __ktest_fn_##name(); \
    class __KTest_##name : public ktest::KTestTest { \
    public: \
        __KTest_##name() : ktest::KTestTest(#name, __ktest_fn_##name) { \
        } \
    }; \
    static __KTest_##name __ktest_##name; \
    void __ktest_fn_##name()

    inline void runAllTests() {
        const char *forkEnv = std::getenv("KTEST_FORK");
        const bool shouldFork = forkEnv != nullptr && !std::strcmp(forkEnv, "1");
        for (const auto &test: tests) {
            std::cout << "Running test: \033[1;36m" << test.name() << "\033[0m\n";
            if (shouldFork) {
                const pid_t child = fork();
                if (child == 0) {
                    // we're the child process
                    test();
                    exit(0);
                }
                if (child == -1) {
                    std::cerr << "Error starting test " << test.name() << ": " << std::strerror(errno) << '\n';
                } else {
                    // we're the parent process
                    int status;
                    waitpid(child, &status, 0);

                    if (WIFSIGNALED(status)) {
                        const int signal = WSTOPSIG(status);
                        std::cout << "Test \033[1;36m" << test.name() << "\033[0m \033[1;31mfailed\033[0m. Signal: " << strsignal(signal) << '\n';
                    } else {
                        std::cout << "Test \033[1;36m" << test.name() << "\033[0m \033[1;32msucceeded\033[0m.\n";
                    }
                }
            } else {
                try {
                    test();
                    std::cout << "Test \033[1;36m" << test.name() << "\033[0m \033[1;32msucceeded\033[0m.\n";
                } catch (const std::exception &err) {
                    std::cout << "Test \033[1;36m" << test.name() << "\033[0m \033[1;31mfailed\033[0m. Error: " << err.what() << '\n';
                }
            }
        }
    }
}

#endif //KTEST_HPP
