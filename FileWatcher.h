//
// Created by aloussase on 7/12/25.
//

#ifndef FILEWATCHER_H
#define FILEWATCHER_H

#include <functional>
#include <string>
#include <thread>


class FileWatcher {
public:
    explicit FileWatcher(const std::string &);

    using FileChangedCallback = std::function<void()>;

    /**
     * Watched the file for changes and invokes callback when changes are detected.
     */
    [[nodiscard]] std::thread watch(FileChangedCallback) noexcept;

private:
    const std::string &m_filename;
};


#endif //FILEWATCHER_H
