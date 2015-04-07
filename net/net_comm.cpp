#include <iostream>
#include <tr1/memory>
#include "net_comm.h"

class NetControl {

};

class NetConnect {
    private:
        int state;
        string client_ip;

};

class NetBase {
    public:
        NetBase();
        ~NetBase();
    public:
        int recv(char* buf, size_t len, int flags); 
        int bufer_recv(int fd, char* buf, size_t len);
        int init_epoll(int event_num);

    private:
        int epoll_fd;
        int epoll_size;
        struct epoll_event* evs;
        struct timeval tv;
        list<std::tr1::shared_ptr<NetControl> > list_control;
        vector<EpollEvent> wait_events;
};

int NetBase::recv(char* buf, size_t len, int flags)
{
begin:
    int ret = rev(sd, buf, len, flags);
    if(ret < 0)
    {
        if(errno == EAGAIN || errno == EINTR){
            goto begin:
        }
        return -1;
    }
    if(ret == 0) 
        return -1;
    return ret;
}

int NetBase::bufer_recv(int fd, char* buf, size_t len)
{
    int r;
    do
    {
        r = recv(fd, buf + offset, left);
        if(r < 0)
            return -1;
        else if( r == static_cast<int>(left))
            return 0;
        offset += r;
        left -= r;
    }while(r > 0)
}

int NetBase::init_epoll(int event_num)
{
    int event_nums = sizeof(struct epoll_event) * event_num;
    evs = (struct epoll_event*)malloc(event_nums);
    if(!evs)
        return -errno;

    memset(evs, 0, event_nums);
    epoll_fd = epoll_create(1024); /* just an hint for the kernel? */
    if(epoll_fd == -1)
    {
        return -errno;
    }
    epoll_size = event_num;
    return 0;
}

int NetBase::add_event(int fd, int cur_mark, int oper_mark)
{
    struct epoll_event ep;
    ep.events = EPOLLET;
    
    int oper = cur_mark == EVENT_NONE ? EPOLL_CTL_ADD : EPOLL_CTL_MOD;

    if(oper_mark & EPOLL_READ)
        ee.events |= EPOLLIN;

    if(oper_mark & EPOLL_WRITE)
        ee.events |= EPOLLOUT;

    ep.data.u64 = 0;
    ep.data.fd = fd;
    if(epoll_ctl(epoll_fd, op, fd, &ep) == -1)
    {
        return -errno;
    }
    return 0;
}


int NetBase::event_wait()
{
   int ret; 
   ret = epoll_wait(epoll_fd, evs, epoll_size, tv ? : -1);
   if(ret > 0)
   {
       wait_events.resize(ret);
       for(int j = 0; j < num; j++)
       {
           int mark = 0;
           struct epoll_event *ep = events + j;

           if(ep->events & EPOLLIN) mark |= EVENT_READ;
           if(ep->events & EPOLLOUT) mark |= EVENT_WRITE;
           if(ep->events & EPOLLERR) mark |= EVENT_WRITE;
           if(ep->events & EPOLLHUP) mark |= EVENT_WRITE;

           wait_events[j].fd = ep->data.fd;
           wait_events[j].mark = mark;
       }
   }
   return ret;
}
