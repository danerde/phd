/*
 * sys_time.h
 *
 *  Created on: Jul 7, 2014
 *      Author: dan
 */

#ifndef SYS_TIME_H_
#define SYS_TIME_H_

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/date.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
inline
boost::posix_time::ptime get_first_time(){
  using namespace boost;
  using namespace posix_time;
  using namespace boost::posix_time;
  using namespace boost::gregorian;
  return ptime(date(2002,Jan,10), seconds(0));
}
static boost::posix_time::ptime start_time = get_first_time();
inline
double get_sec() {
    boost::posix_time::ptime now =
            boost::posix_time::microsec_clock::local_time();
    return (now - start_time).total_nanoseconds() / 1000000000.0;
}


#endif /* SYS_TIME_H_ */
