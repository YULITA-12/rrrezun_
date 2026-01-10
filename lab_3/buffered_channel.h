#ifndef BUFFERED_CHANNEL_H
#define BUFFERED_CHANNEL_H

#include <queue>
#include <mutex>
#include <condition_variable>
#include <stdexcept>
#include <utility>

template <typename ElementType>
class BufferedChannel {
public:
    explicit BufferedChannel(int capacity) : max_capacity_(capacity), is_shutdown_(false) {}

    void Send(ElementType item) {
        std::unique_lock<std::mutex> locker(protection_mtx_);

        space_available_.wait(locker, [this]() {
            return buffer_.size() < max_capacity_ || is_shutdown_;
        });

        if (is_shutdown_) {
            throw std::runtime_error("Channel is closed");
        }

        buffer_.push(std::move(item));
        data_available_.notify_one();
    }

    std::pair<ElementType, bool> Recv() {
        std::unique_lock<std::mutex> locker(protection_mtx_);

        data_available_.wait(locker, [this]() {
            return !buffer_.empty() || is_shutdown_;
        });

        if (buffer_.empty() && is_shutdown_) {
            return {ElementType(), false};
        }

        ElementType element = std::move(buffer_.front());
        buffer_.pop();

        space_available_.notify_one();

        return {std::move(element), true};
    }

    void Close() {
        std::unique_lock<std::mutex> locker(protection_mtx_);
        is_shutdown_ = true;

        space_available_.notify_all();
        data_available_.notify_all();
    }

private:
    size_t max_capacity_;
    std::queue<ElementType> buffer_;
    bool is_shutdown_;
    std::mutex protection_mtx_;
    std::condition_variable space_available_;
    std::condition_variable data_available_;
};

#endif //BUFFERED_CHANNEL_H
