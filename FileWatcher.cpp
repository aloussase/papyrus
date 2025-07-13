//
// Created by aloussase on 7/12/25.
//

#include "FileWatcher.h"

#include <cstring>
#include <unistd.h>
#include <fcntl.h>

#include <chrono>
#include <thread>

FileWatcher::FileWatcher(const std::string &s)
    : m_filename(s) {
}

std::thread FileWatcher::watch(FileChangedCallback cb) noexcept {
    // TODO: Consider using stat to calculate buffer sizes.
    static char old[4096] = {0};
    static char current[4096] = {0};

    int fd = open(m_filename.c_str(), O_RDONLY);
    if (fd < 0) {
        fprintf(stderr, "failed to open file: %s\n", m_filename.c_str());
        return {};
    }

    if (read(fd, old, sizeof(old)) < 0) {
        fprintf(stderr, "failed to read file: %s\n", m_filename.c_str());
        return {};
    }

    return std::thread([&, fd] {
        try {
            while (true) {
                // We need to re-open the file descriptor because most modern editors write to a temp file and the
                // re-create the orginal file with the contents of the temporary file.

                close(fd);
                int fd = open(m_filename.c_str(), O_RDONLY);

                if (read(fd, current, sizeof(current)) < 0) {
                    fprintf(stderr, "failed to read file: %s\n", m_filename.c_str());
                    break;
                }

                // TODO: Consider using hashes.
                if (memcmp(old, current, sizeof(old)) != 0) {
                    memcpy(old, current, sizeof(old));
                    memset(current, 0, sizeof(current));
                    fprintf(stderr, "source file changed...invoking callback\n");
                    cb();
                }

                std::this_thread::sleep_for(std::chrono::seconds(2));
            }
        } catch (...) {
            close(fd);
        }
    });
}
