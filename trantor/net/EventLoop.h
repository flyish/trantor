// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)

//Token from Muduo and modified
// Copyright 2016, Tao An.  All rights reserved.
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Tao An
#pragma once
#include <trantor/utils/NonCopyable.h>
#include <trantor/utils/Date.h>

#include <thread>
#include <memory>
#include <vector>
#include <mutex>
#include <queue>
namespace trantor
{
    class Poller;
    class TimerQueue;
    class Channel;
    typedef std::vector<Channel*> ChannelList;
    typedef std::function<void ()> Func;
    class EventLoop:NonCopyable
    {
    public:
        EventLoop();
        ~EventLoop();
        void loop();
        void quit();
        void assertInLoopThread()
        {
            if(!isInLoopThread())
            {
                abortNotInLoopThread();
            }
        };
        bool isInLoopThread() const { return threadId_ == std::this_thread::get_id();};
        static EventLoop* getEventLoopOfCurrentThread();
        void updateChannel(Channel *chl);
        void removeChannel(Channel *chl);
        void runInLoop(const Func &f);
        void runInLoop(Func &&f);
        void queueInLoop(const Func &f);
        void queueInLoop(Func &&f);
        void wakeup();
        void wakeupRead();
        ///
        /// Runs callback at 'time'.
        /// Safe to call from other threads.
        ///
        void runAt(const Date& time, const Func& cb);
        void runAt(const Date& time, Func&& cb);
        ///
        /// Runs callback after @c delay seconds.
        /// Safe to call from other threads.
        ///
        void runAfter(double delay, const Func& cb);
        void runAfter(double delay, Func&& cb);
        ///
        /// Runs callback every @c interval seconds.
        /// Safe to call from other threads.
        ///
        void runEvery(double interval, const Func& cb);
        void runEvery(double interval, Func&& cb);
        //int getAioEventFd();
        //io_context_t getAioContext() {return ctx_;};
    private:
        void abortNotInLoopThread();
        bool looping_;
        const std::thread::id threadId_;
        bool quit_;
        std::unique_ptr<Poller> poller_;

        ChannelList activeChannels_;
        Channel *currentActiveChannel_;

        bool eventHandling_;

        std::mutex funcsMutex_;

        std::vector<Func> funcs_;
        std::unique_ptr<TimerQueue> timerQueue_;
        bool callingFuncs_=false;
        int wakeupFd_;
        std::unique_ptr<Channel> wakeupChannelPtr_;

        void doRunInLoopFuncs();
    };
}
