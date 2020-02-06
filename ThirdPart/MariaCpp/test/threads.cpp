/****************************************************************************
  Copyright (C) 2015 Karol Roslaniec <mariacpp@roslaniec.net>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not see <http://www.gnu.org/licenses>
  or write to the Free Software Foundation, Inc.,
  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*****************************************************************************/
#include <mariacpp/lib.hpp>
#include <mariacpp/connection.hpp>
#include <mariacpp/exception.hpp>
#include <mariacpp/uri.hpp>
#include <cstdlib>
#include <iostream>
#include <errno.h>
#include <stdio.h>


#define handle_error_en(en, msg)                                        \
    do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)

#define handle_error(msg)                                               \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)


struct thread_info {    /* Used as argument to thread_start() */
    pthread_t thread_id;        /* ID returned by pthread_create() */
    int       thread_num;       /* Application-defined thread # */
    const char *uri;
    const char *user;
    const char *passwd;
    unsigned long conn_id;
    std::string stats;
};

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
static int counter = 0;


void *
thread_start(void *arg)
{
    // It's very important to call thread_init() method
    MariaCpp::scoped_thread_init maria_thread;

    thread_info *tinfo = reinterpret_cast<thread_info *>(arg);
    const int tn = tinfo->thread_num;

    try {
        MariaCpp::Connection conn;
        // Connect to DB using MySQL Connector/C++ style host URI.
        // You can also use alternative connect() method with
        // the same arguments as C-API.
        conn.connect(MariaCpp::Uri(tinfo->uri), tinfo->user, tinfo->passwd);

        // Wait on condition for other threads to connect...
        pthread_mutex_lock(&mutex);
        counter++;
        std::clog << tn << ". Connection status: SUCESS" << std::endl;
        pthread_cond_wait(&cond, &mutex);
        pthread_mutex_unlock(&mutex);

        // Each thread has connection do DB.
        // You can put here whatever queries you want
        
        tinfo->conn_id = conn.thread_id();
        tinfo->stats = conn.stat();

        conn.close(); // optional
    } catch (MariaCpp::Exception &e) {
        std::cerr << e << std::endl;
        return (void *)1;
    }
    return 0;
}


int main()
{
    // Initialize MariaDB/MySQL library
    // Alternatively call methods:
    //   MariaCpp::library_init()
    //   ....
    //   MariaCpp::library_end()
    // In multithreaded MariaDB environment you MUST call library_init()
    // method from main thread before creating other threads!
    MariaCpp::scoped_library_init maria_lib_init;
    if (MariaCpp::thread_safe())
        std::clog << "MariaDB library is thread safe: OK" << std::endl;
    else
        std::cerr << "Ups... MariaDB library is NOT thread safe!" << std::endl;

    const char *uri = std::getenv("TEST_DB_URI");
    const char *user = std::getenv("TEST_DB_USER");
    const char *passwd = std::getenv("TEST_DB_PASSWD");
    if (!uri) uri = "tcp://localhost:3306/test";
    if (!user) user = "test";
    if (!passwd) passwd = "";
    std::clog << "DB uri: " << uri << std::endl;
    std::clog << "DB user: " << user << std::endl;
    std::clog << "DB passwd: " << passwd << std::endl;

    const unsigned num_threads = 5;
    struct thread_info *tinfo;
    tinfo = new thread_info[num_threads];

    // Create threads
    for (unsigned tnum = 0; tnum < num_threads; tnum++) {
        thread_info &t = tinfo[tnum];
        tinfo[tnum].thread_num = tnum + 1;
        tinfo[tnum].uri = uri;
        tinfo[tnum].user = user;
        tinfo[tnum].passwd = passwd;
        
        int s = pthread_create(&tinfo[tnum].thread_id, NULL,
                               &thread_start, &tinfo[tnum]);
        if (s != 0)
            handle_error_en(s, "pthread_create");
    }

    // Wait for threads to enter condition
    time_t now = time(NULL);
    while (true) {
        pthread_mutex_lock(&mutex);
        int c = counter;
        pthread_mutex_unlock(&mutex);
        if (c == num_threads) break;
        pthread_yield();
        if (now + 3 < time(NULL)) {
            std::cerr << "TIMEOUT" << std::endl;
            exit(EXIT_FAILURE);
        }
    }
    // trigger (start) all threads
    pthread_cond_broadcast(&cond);

    // Wait for threads and print results
    int result = 0;
    for (unsigned tnum = 0; tnum < num_threads; tnum++) {
        thread_info &t = tinfo[tnum];
        void *res;
        int s = pthread_join(tinfo[tnum].thread_id, &res);
        if (s != 0)
            handle_error_en(s, "pthread_join");
        result += (int)(long)res;

        std::clog << t.thread_num << ". Finished res=" << (int)(long)res
                  << std::endl;
        std::clog << "   conn id: " << t.conn_id << std::endl;
        std::clog << "   stats: " << t.stats << std::endl;
    }
    
    delete [] tinfo;
    return result;
}
