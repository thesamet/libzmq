/*
    Copyright (c) 2009-2011 250bpm s.r.o.
    Copyright (c) 2007-2011 Other contributors as noted in the AUTHORS file

    This file is part of 0MQ.

    0MQ is free software; you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    0MQ is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef __ZMQ_XREQ_HPP_INCLUDED__
#define __ZMQ_XREQ_HPP_INCLUDED__

#include "socket_base.hpp"
#include "session_base.hpp"
#include "fq.hpp"
#include "lb.hpp"

namespace zmq
{

    class ctx_t;
    class msg_t;
    class pipe_t;
    class io_thread_t;
    class socket_base_t;

    class xreq_t :
        public socket_base_t
    {
    public:

        xreq_t (zmq::ctx_t *parent_, uint32_t tid_);
        ~xreq_t ();

    protected:

        //  Overloads of functions from socket_base_t.
        void xattach_pipe (zmq::pipe_t *pipe_, bool icanhasall_);
        int xsend (zmq::msg_t *msg_, int flags_);
        int xrecv (zmq::msg_t *msg_, int flags_);
        bool xhas_in ();
        bool xhas_out ();
        void xread_activated (zmq::pipe_t *pipe_);
        void xwrite_activated (zmq::pipe_t *pipe_);
        void xterminated (zmq::pipe_t *pipe_);

    private:

        //  Messages are fair-queued from inbound pipes. And load-balanced to
        //  the outbound pipes.
        fq_t fq;
        lb_t lb;

        //  Have we prefetched a message.
        bool prefetched;

        //  Holds the prefetched message.
        msg_t prefetched_msg;

        xreq_t (const xreq_t&);
        const xreq_t &operator = (const xreq_t&);
    };

    class xreq_session_t : public session_base_t
    {
    public:

        xreq_session_t (zmq::io_thread_t *io_thread_, bool connect_,
            zmq::socket_base_t *socket_, const options_t &options_,
            const char *protocol_, const char *address_);
        ~xreq_session_t ();

    private:

        xreq_session_t (const xreq_session_t&);
        const xreq_session_t &operator = (const xreq_session_t&);
    };

}

#endif
