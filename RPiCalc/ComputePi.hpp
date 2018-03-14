#ifndef COMPUTE_PI_H
#define COMPUTE_PI_H

namespace pi
{
/** calculates pi out to a certain degree
 * @param[in] number - the degree to calcluate pi out to
 */
  unsigned int computeDigit(int number);

  int next_prime(int n);

  int is_prime(int n);

  int pow_mod(int a, int b, int m);

  int inv_mod(int x, int y);
}
#endif
