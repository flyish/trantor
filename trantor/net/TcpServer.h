#pragma once

#include <trantor/net/callbacks.h>
#include <trantor/utils/NonCopyable.h>
#include <trantor/utils/Logger.h>
#include <trantor/net/EventLoopThreadPool.h>
#include <trantor/net/InetAddress.h>
#include <trantor/net/TcpConnection.h>
#include <string>
#include <memory>
#include <set>
#include <signal.h>
namespace trantor
{
    class Acceptor;

    class TcpServer:NonCopyable
    {
    public:
        TcpServer(EventLoop *loop,const InetAddress &address,const std::string &name);
        ~TcpServer();
        void start();
        void setIoLoopNum(size_t num);
        void setRecvMessageCallback(const RecvMessageCallback &cb){recvMessageCallback_=cb;}
        void setConnectionCallback(const ConnectionCallback &cb){connectionCallback_=cb;}
        void setWriteCompleteCallback(const WriteCompleteCallback &cb){writeCompleteCallback_=cb;}

        void setRecvMessageCallback(RecvMessageCallback &&cb){recvMessageCallback_=std::move(cb);}
        void setConnectionCallback(ConnectionCallback &&cb){connectionCallback_=std::move(cb);}
        void setWriteCompleteCallback(WriteCompleteCallback &&cb){writeCompleteCallback_=std::move(cb);}
        
    private:
        EventLoop *loop_;
        std::unique_ptr<Acceptor> acceptorPtr_;
        void newConnection(int fd,const InetAddress &peer);
        std::string serverName_;
        std::set<TcpConnectionPtr>connSet_;

        RecvMessageCallback recvMessageCallback_;
        ConnectionCallback connectionCallback_;
        WriteCompleteCallback writeCompleteCallback_;

        void connectionClosed(const TcpConnectionPtr &connectionPtr);
        std::unique_ptr<EventLoopThreadPool> loopPoolPtr_;
        class IgnoreSigPipe
        {
        public:
            IgnoreSigPipe()
            {
                ::signal(SIGPIPE, SIG_IGN);
                LOG_TRACE << "Ignore SIGPIPE";
            }
        };

        IgnoreSigPipe initObj;
    };
}