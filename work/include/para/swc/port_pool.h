///////////////////////////////////////////////////////////////////////////////////////////////////////////
///                                                                                                        
/// Copyright, 2020 PopcornSAR Co., Ltd. All rights reserved.                                              
/// This software is copyright protected and proprietary to PopcornSAR Co., Ltd.                           
/// PopcornSAR Co., Ltd. grants to you only those rights as set out in the license conditions.             
///                                                                                                        
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// AUTOSAR VERSION                   : R20-11
/// API TYPE                          : LIBPARA
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// File name                         : concurrency/thread_pool.h
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///                                                                                                        
/// CAUTION!! DO NOT EDIT                                                   
///                                                                                                        
///////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef PARA_SWC_PORT_POOL_H
#define PARA_SWC_PORT_POOL_H

#include <atomic>
#include <condition_variable>
#include <cstdint>
#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <utility>

namespace para
{
namespace swc
{

class PortPool
{
public:
    /// @brief type aliases
    using TaskType         = std::function<void()>;
    using PackagedTaskType = std::packaged_task<void()>;
    /// @brief default constructor with hardware contexts
    PortPool();

    /// @brief constructor with defined number of pool
    PortPool(std::uint32_t threadCount);

    /// @brief destructor
    ~PortPool();

    /// @brief run thread
    template<typename Function, typename... Args>
    inline std::shared_future<void> Async(Function&& F, Args&&... ArgList)
    {
        auto Task = std::bind(std::forward<Function>(F), std::forward<Args>(ArgList)...);
        return AsyncImpl(std::move(Task));
    }

    /// @brief run thread
    template<typename Function>
    inline std::shared_future<void> Async(Function&& F)
    {
        return AsyncImpl(std::forward<Function>(F));
    }

    /// @brief wait all threads until terminated
    void Wait();

    /// @brief return how many threads are active
    std::uint32_t CurrentActivates() const;

private:
    /// @brief function
    std::shared_future<void> AsyncImpl(TaskType F);
    
private:
    /// @brief pool of threads
    std::vector<std::thread> m_threads;
    /// @brief queue for tasks
    std::queue<PackagedTaskType> m_tasks;
    /// @brief mutex for queue
    std::mutex m_queueLock;
    /// @brief condition for queue
    std::condition_variable m_queueCondition;
    /// @brief mutex for completion
    std::mutex m_completionLock;
    /// @brief condition for completion
    std::condition_variable m_completionCondition;
    /// @brief number of active threads
    std::atomic<std::uint32_t> m_activeThreads;
    /// @brief flag
    bool m_enable;
};

} // namespace swc
} // namespace para

#endif // PARA_SWC_PORT_POOL_H